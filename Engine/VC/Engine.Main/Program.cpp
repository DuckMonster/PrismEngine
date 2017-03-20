#include "stdafx.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Prism/Context.h>
#include <Prism/Utils/DirLight.h>
#include <Prism/Utils/DefaultShader.h>

/**	Constructor
*******************************************************************************/
CProgram::CProgram( ) {
}

/**	Init
*******************************************************************************/
void CProgram::Init( ) {
	m_Shader	= PR_CResource::Load<PR_CShaderResource>( "Deferred/shdr_deferred_base" );
	m_mesh		= PR_CResource::Load<PR_CMeshResource>( "UnitCube.fbx" );
	m_plane		= PR_CResource::Load<PR_CMeshResource>( "UnitPlane.fbx" );
	m_Texture	= PR_CResource::Load<PR_CTextureResource>( "sample.png" );

	m_LightShader = PR_CResource::Load<PR_CShaderResource>( "Deferred/shdr_deferred_light" );
	m_LightShader->Set( "u_Deferred.position", 0 );
	m_LightShader->Set( "u_Deferred.normal", 1 );
	m_LightShader->Set( "u_Deferred.depth", 2 );

	m_Framebuffer	= PR_CResource::Create<PR_CFramebufferResource>( );
	m_FBColor		= PR_CResource::Create<PR_CTextureResource>( );
	m_FBNormal		= PR_CResource::Create<PR_CTextureResource>( );
	m_FBDepth		= PR_CResource::Create<PR_CTextureResource>( );

	m_Framebuffer->SetResolution( PR_CContext::Instance( )->GetWindowWidth( ), PR_CContext::Instance( )->GetWindowHeight( ) );
	m_Framebuffer->BindTextureColor( m_FBColor, 0, GL_RGB, GL_UNSIGNED_BYTE );
	m_Framebuffer->BindTextureColor( m_FBNormal, 1, GL_RGB16F, GL_FLOAT );
	m_Framebuffer->BindTextureDepth( m_FBDepth );

	PR_ASSERT( m_Framebuffer->IsComplete( ) );
	if (!m_Framebuffer->IsComplete( ))
		std::cout << "Hello!\n";

	m_FBColor->SetFilter( GL_NEAREST );
	m_FBDepth->SetFilter( GL_NEAREST );

	m_Material.SetShader( m_Shader );
	m_Material.SetColor( glm::vec4( 1.f, 0.f, 0.f, 1.f ) );

	m_GroundMaterial = m_Material;
	m_GroundMaterial.SetColor( glm::vec4( 0.f, 1.f, 0.f, 1.f ) );
}

/**	Update
*******************************************************************************/
void CProgram::Update( double delta ) {
}

/**	Render
*******************************************************************************/
void CProgram::Render( double delta ) {
	static float t = 0.f;
	t += delta;

	glEnable( GL_DEPTH_TEST );

	glm::ivec2 contextSize = PR_CContext::Instance( )->GetWindowSize( );
	float contextRatio = (float)contextSize.x / contextSize.y;

	PR_CRenderScene scene;

	glm::mat4 camera = glm::ortho( -5.f * contextRatio, 5.f * contextRatio, -5.f, 5.f, 0.f, 100.f )
		* glm::lookAt( glm::vec3( 5.f, 5.f, 5.f ), glm::vec3( 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );

	PR_SDirLight light;
	light.m_Position = glm::vec3( 5.f, 5.f, -5.f );
	light.m_Direction = -light.m_Position;

	scene.SetCameraMatrix( camera );
	scene.SetLight( light );

	glm::mat4 world( 1.f );

	world = glm::translate( glm::mat4( 1.f ), glm::vec3( 1.5f, 0.f, 0.f ) );
	//world = glm::scale( world, glm::vec3( 0.5f ) );
	//world = glm::rotate( world, t, glm::vec3( 0.f, 0.f, 1.f ) );
	scene.AddMesh( m_mesh, &m_Material, world );

	world = glm::translate( glm::mat4( 1.f ), glm::vec3( -1.5, 0.f, 0.f ) );
	world = glm::rotate( world, t, glm::vec3( 0.f, 1.f, 0.f ) );
	scene.AddMesh( m_mesh, &m_Material, world );

	world = glm::translate( glm::mat4( 1.f ), glm::vec3( 0.f, -0.5f, 0.f ) );
	world = glm::scale( world, glm::vec3( 100.f ) );
	scene.AddMesh( m_plane, &m_GroundMaterial, world );

	m_Texture->Bind( 0 );
	m_Framebuffer->Bind( );
	m_renderer.Render( scene );

	PR_CFramebufferResource::Release( );

	glDisable( GL_DEPTH_TEST );

	float left = -1.f,
		right = 0.5f,
		top = 1.f,
		bottom = -0.5f,
		width = right - left,
		height = top - bottom;

	glm::mat4 quadMatrix(
		(right - left) / 2.f, 0.f, 0.f, 0.f,
		0.f, (top - bottom) / 2.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		(right + left) / 2.f, (bottom + top) / 2.f, 0.f, 1.f
	);

	/*m_LightShader->Use( );
	m_FBColor->Bind( 0 );
	m_FBDepth->Bind( 1 );
	glDrawArrays( GL_QUADS, 0, 4 );*/

	GetQuadShader( )->Use( );
	GetQuadShader( )->Set( "u_QuadMatrix", glm::mat4( 1.f ) ); // quadMatrix )
	m_FBColor->Bind( 0 );
	glDrawArrays( GL_QUADS, 0, 4 );
}