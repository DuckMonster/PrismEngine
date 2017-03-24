#include <Prism\Render\DeferredRenderer.h>
#include <Prism\Context.h>

namespace {
	const char* SRC_DEFERRED_VERT = "\
#version 330 core\n\
\
layout( location = 0 ) in vec3 a_Position;\
layout( location = 1 ) in vec3 a_Normal;\
layout( location = 2 ) in vec2 a_UV;\
\
uniform mat4 u_Camera;\
uniform mat4 u_World;\
\
out VS_OUT{\
	vec3 world;\
	vec3 normal;\
	vec2 uv;\
} vs_out;\
\
void main( ) {\
	gl_Position = u_Camera * u_World * vec4( a_Position, 1.0 );\
\
	vs_out.world = (u_World * vec4( a_Position, 1.0 )).xyz;\
	vs_out.normal = (u_World * vec4( a_Normal, 0.0 )).xyz;\
	vs_out.uv = a_UV;\
}";

	const char* SRC_DEFERRED_FRAG = "\
#version 330 core\n\
\
in VS_OUT{\
	vec3 world;\
	vec3 normal;\
	vec2 uv;\
} fs_in;\
\
struct Material {\
	vec4 color;\
	sampler2D sampler;\
	float textureWeight;\
};\
\
uniform Material u_Material;\
\
layout( location = 0 ) out vec3 o_Color;\
layout( location = 1 ) out vec3 o_Normal;\
layout( location = 2 ) out vec3 o_Diffuse;\
\
void main( ) {\
	o_Color = fs_in.world;\
	o_Normal = fs_in.normal;\
	o_Diffuse = texture(u_Material.sampler, fs_in.uv).xyz;\
	o_Diffuse = mix(vec3(1.0), o_Diffuse, u_Material.textureWeight);\
	o_Diffuse *= u_Material.color.xyz;\
}";
}

/**	Constructor
*******************************************************************************/
PR_CDeferredRenderer::PR_CDeferredRenderer( ) :
	m_Framebuffer( NULL ), m_Shader( NULL ) {
}

/**	Render
*******************************************************************************/
void PR_CDeferredRenderer::Render( PR_CRenderScene & scene ) {
	LoadResources( );

	m_Framebuffer->Bind( );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	PR_CRenderScene::SNode* node = NULL;

	// Render all nodes
	for (node = scene.GetRoot( ); node; node = node->m_Next) {
		switch (node->m_Type) {
		case PR_CRenderScene::NODE_MESH:
			{
				PR_CRenderScene::SMeshNode* meshNode = (PR_CRenderScene::SMeshNode*)node;
				PR_CMaterial* material = meshNode->m_Material;

				// Setup shader
				m_Shader->Set( "u_Camera", scene.GetCameraMatrix( ) );
				m_Shader->Set( "u_World", meshNode->m_Transform );

				m_Shader->Set( "u_Material.color", material->GetColor( ) );
				if (material->GetTexture( ) != NULL) {
					material->GetTexture( )->Bind( 0 );
					m_Shader->Set( "u_Material.textureWeight", 1.f );
				}
				else {
					m_Shader->Set( "u_Material.textureWeight", 0.f );
				}

				// Render
				meshNode->m_Mesh->Render( );
			} break;
		}
	}

	PR_CFramebufferResource::Release( );
}

/**	Load Resources
*******************************************************************************/
void PR_CDeferredRenderer::LoadResources( ) {
	if (m_Framebuffer != NULL || m_Shader != NULL)
		return;

	// Framebuffer
	m_Framebuffer		= PR_CResource::Create<PR_CFramebufferResource>( );
	m_GBuffer.Position	= PR_CResource::Create<PR_CTextureResource>( );
	m_GBuffer.Normal	= PR_CResource::Create<PR_CTextureResource>( );
	m_GBuffer.Diffuse	= PR_CResource::Create<PR_CTextureResource>( );
	m_GBuffer.Depth		= PR_CResource::Create<PR_CTextureResource>( );

	//m_GBuffer.Position->SetFilter( GL_NEAREST );
	//m_GBuffer.Normal->SetFilter( GL_NEAREST );
	//m_GBuffer.Diffuse->SetFilter( GL_NEAREST );

	m_Framebuffer->SetResolution( PR_CContext::Instance( )->GetWindowWidth( ), PR_CContext::Instance( )->GetWindowHeight( ) );
	m_Framebuffer->BindTextureColor( m_GBuffer.Position, 0, GL_RGB16F, GL_FLOAT );
	m_Framebuffer->BindTextureColor( m_GBuffer.Normal, 1, GL_RGB16F, GL_FLOAT );
	m_Framebuffer->BindTextureColor( m_GBuffer.Diffuse, 2, GL_RGB, GL_UNSIGNED_BYTE );
	m_Framebuffer->BindTextureDepth( m_GBuffer.Depth );

	// Shader
	m_Shader			= PR_CResource::Create<PR_CShaderResource>( );
	m_Shader->Compile( SRC_DEFERRED_VERT, SRC_DEFERRED_FRAG );
}