#include <Prism/Render/ShadowMapRenderer.h>
#include <Prism/Context.h>
#include <Prism/Utils/Log.h>
#include <glm/gtc/matrix_transform.hpp>

namespace {
	const char* SRC_DEPTH_VERT = "\
#version 330 core\n\
\
layout( location = 0 ) in vec3 a_Position;\
\
uniform mat4 u_Light;\
uniform mat4 u_World;\
\
void main( ) {\
	gl_Position = u_Light * u_World * vec4( a_Position, 1.0 );\
}";

	const char* SRC_DEPTH_FRAG = "\
#version 330 core\n\
void main( ) {}";

	const char* SRC_SHADOW_VERT = "\
#version 330 core\n\
\
layout( location = 0 ) in vec3 a_Position;\
layout( location = 1 ) in vec3 a_Normal;\
\
uniform mat4 u_World;\
uniform mat4 u_Camera;\
uniform mat4 u_Light;\
\
out VS_OUT {\
	vec3 normal;\
	vec3 shadowCoord;\
} vs_out;\
\
void main( ) {\
	gl_Position = u_Camera * u_World * vec4( a_Position, 1.0 );\
\
	vs_out.normal = (u_World * vec4( a_Normal, 0.0 )).xyz;\
\
	// Shadow\n\
	mat4 biasMatrix = mat4(\
		0.5, 0.0, 0.0, 0.0,\
		0.0, 0.5, 0.0, 0.0,\
		0.0, 0.0, 0.5, 0.0,\
		0.5, 0.5, 0.5, 1.0\
	);\
\
	vs_out.shadowCoord = (biasMatrix * u_Light * u_World * vec4( a_Position, 1.0 )).xyz;\
}";

	const char* SRC_SHADOW_FRAG = "\
#version 330 core\n\
\
uniform float u_Bias;\
uniform float u_AngleBias;\
\
uniform sampler2D u_DepthSampler;\
uniform vec3 u_LightDirection;\
\
in VS_OUT{\
	vec3 normal;\
	vec3 shadowCoord;\
} fs_in;\
\
out float o_Value;\
\
void main( ) {\
	// Bias tangent\n\
	float bias = clamp( u_Bias, 0.0, 0.01 );\
\
	// Shadow depth checking\n\
	// Facing away from light\n\
	if (dot( u_LightDirection, fs_in.normal ) >= -u_AngleBias) {\
		o_Value = 1.0;\
	}\
	else {\
		float depth = fs_in.shadowCoord.z;\
		float bufferDepth = texture( u_DepthSampler, fs_in.shadowCoord.xy ).z;\
\
		o_Value = (depth > bufferDepth + bias) ? 1.0 : 0.0;\
	}\
} ";
}

/**	Constructor
*******************************************************************************/
PR_CShadowMapRenderer::PR_CShadowMapRenderer( ) :
	m_DepthShader( NULL ), m_ShadowShader( NULL ),
	m_DepthFramebuffer( NULL ), m_DepthTexture( NULL ),
	m_ShadowFramebuffer( NULL ), m_ShadowTexture( NULL ) {
}

/**	Render
*******************************************************************************/
void PR_CShadowMapRenderer::Render( PR_CRenderScene& scene ) {
	LoadResources( );

	m_DepthFramebuffer->Bind( );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glm::mat4 lightMatrix = glm::ortho( -10.f, 10.f, -10.f, 10.f, -40.f, 40.f );
	lightMatrix *= glm::lookAt(
		scene.GetLight( ).m_Position,
		scene.GetLight( ).m_Position + scene.GetLight( ).m_Direction,
		glm::vec3( 0.f, 1.f, 0.f ) );

	PR_CRenderScene::SNode* node = NULL;

	// Render all nodes into depth buffer
	for (node = scene.GetRoot( ); node; node = node->m_Next) {
		switch (node->m_Type) {
		case PR_CRenderScene::NODE_MESH:
			{
				PR_CRenderScene::SMeshNode* meshNode = (PR_CRenderScene::SMeshNode*)node;

				// Setup shader
				m_DepthShader->Set( "u_Light", lightMatrix );
				m_DepthShader->Set( "u_World", meshNode->m_Transform );

				// Render
				meshNode->m_Mesh->Render( );
			} break;
		}
	}

	m_ShadowFramebuffer->Bind( );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Render all nodes into shadow buffer
	for (node = scene.GetRoot( ); node; node = node->m_Next) {
		switch (node->m_Type) {
		case PR_CRenderScene::NODE_MESH:
			{
				PR_CRenderScene::SMeshNode* meshNode = (PR_CRenderScene::SMeshNode*)node;

				// Setup shader
				m_ShadowShader->Set( "u_Camera", scene.GetCameraMatrix( ) );
				m_ShadowShader->Set( "u_Light", lightMatrix );
				m_ShadowShader->Set( "u_World", meshNode->m_Transform );

				m_ShadowShader->Set( "u_Bias", 0.0008f );
				m_ShadowShader->Set( "u_AngleBias", 0.0016f );
				m_ShadowShader->Set( "u_LightDirection", scene.GetLight( ).m_Direction );

				m_DepthTexture->Bind( 0 );

				// Render
				meshNode->m_Mesh->Render( );
			} break;
		}
	}

	PR_CFramebufferResource::Release( );
}

/**	Load Resources
*******************************************************************************/
void PR_CShadowMapRenderer::LoadResources( ) {
	if (m_DepthFramebuffer != NULL || m_ShadowFramebuffer != NULL || m_DepthShader != NULL || m_ShadowShader != NULL)
		return;

	//--------------------------------------------------- Depth buffer
	m_DepthFramebuffer	= PR_CResource::Create<PR_CFramebufferResource>( );
	m_DepthTexture		= PR_CResource::Create<PR_CTextureResource>( );

	m_DepthFramebuffer->SetResolution( 2048, 2048 );
	m_DepthFramebuffer->BindTextureDepth( m_DepthTexture );

	//--------------------------------------------------- Depth shader
	m_DepthShader = PR_CResource::Create<PR_CShaderResource>( );
	m_DepthShader->Compile( SRC_DEPTH_VERT, SRC_DEPTH_FRAG );

	//--------------------------------------------------- Shadow buffer
	m_ShadowFramebuffer	= PR_CResource::Create<PR_CFramebufferResource>( );
	m_ShadowTexture		= PR_CResource::Create<PR_CTextureResource>( );
	m_ShadowDepthTexture= PR_CResource::Create<PR_CTextureResource>( );

	m_ShadowFramebuffer->SetResolution( PR_CContext::Instance( )->GetWindowWidth( ), PR_CContext::Instance( )->GetWindowHeight( ) );
	m_ShadowFramebuffer->BindTextureColor( m_ShadowTexture, 0, GL_RED, GL_UNSIGNED_BYTE );
	m_ShadowFramebuffer->BindTextureDepth( m_ShadowDepthTexture );

	//--------------------------------------------------- Shadow shader
	m_ShadowShader = PR_CResource::Create<PR_CShaderResource>( );
	m_ShadowShader->Compile( SRC_SHADOW_VERT, SRC_SHADOW_FRAG );

	PR_ASSERT_MSG( m_DepthFramebuffer->IsComplete( ), "Depth buffer isn't complete" );
	PR_ASSERT_MSG( m_ShadowFramebuffer->IsComplete( ), "Shadow buffer isn't complete" );
}