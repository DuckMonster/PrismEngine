#include "stdafx.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Prism/Context.h>
#include <Prism/Utils/DirLight.h>
#include <Prism/Utils/DefaultShader.h>
#include <Prism/Utils/RenderUtils.h>

/**	Constructor
*******************************************************************************/
CProgram::CProgram( ) {
}

/**	Init
*******************************************************************************/
void CProgram::Init( ) {
	PR_CLogger::sm_LogLevel = PR_LOG_LEVEL_TRIVIAL;

	PR_CResource::SetResourceDirectory( "./Assets/" );

	m_Shader.CompileFiles( "Shader/Deferred/shdr_deferred_base" );
	m_mesh.LoadFromFile( "UnitCube.fbx" );
	m_plane.LoadFromFile( "UnitPlane.fbx" );
	m_Texture.LoadFromFile( "sample.png" );

	m_Material.SetShader( &m_Shader );
	m_Material.SetTexture( &m_Texture );
	m_Material.SetColor( glm::vec4( 1.f, 0.f, 0.f, 1.f ) );

	m_GroundMaterial = m_Material;
	m_GroundMaterial.SetColor( glm::vec4( 0.f, 1.f, 0.f, 1.f ) );

	m_TextMesh.LoadFont( "font.png", 7, 7 );
	m_TextMesh.SetString( "Hello world!" );
}

/**	Update
*******************************************************************************/
void CProgram::Update( double delta ) {
	static float t = 0.f;
	t += delta;

	/*if (t >= 0.5f)
		PR_CLogger( PR_LOG_LEVEL_LOW ) << delta;*/
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
		* glm::lookAt( glm::vec3( glm::sin( t * 0.2f ), 1.f, glm::cos( t * 0.2f ) ) * 5.f, glm::vec3( 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );

	PR_SDirLight light;
	light.m_Position = glm::vec3( 5.f, 5.f, -5.f );
	light.m_Direction = -light.m_Position;

	scene.SetCameraMatrix( camera );
	scene.SetLight( light );

	glm::mat4 world( 1.f );

	world = glm::translate( glm::mat4( 1.f ), glm::vec3( 1.5f, 0.f, 0.f ) );
	//world = glm::scale( world, glm::vec3( 0.5f ) );
	//world = glm::rotate( world, t, glm::vec3( 0.f, 0.f, 1.f ) );
	scene.AddMesh( &m_mesh, &m_Material, world );

	world = glm::translate( glm::mat4( 1.f ), glm::vec3( -1.5f, 0.5f, 0.f ) );
	world = glm::rotate( world, t, glm::vec3( 0.f, 1.f, 0.f ) );
	scene.AddMesh( &m_mesh, &m_Material, world );

	world = glm::translate( glm::mat4( 1.f ), glm::vec3( 0.f, -0.5f, 0.f )  );
	world = glm::scale( world, glm::vec3( 100.f ) );
	scene.AddMesh( &m_plane, &m_GroundMaterial, world );

	//m_DeferredRenderer.Render( scene );
	m_Renderer.Render( scene );

	std::string fpsString = "FPS: " + std::to_string( (int)(1.0 / delta) );

	m_TextMesh.SetString( fpsString );
	m_TextMesh.Render( 2, 10, 1 );
	//m_ShadowRenderer.Render( scene );

	//PR_CFramebufferResource::Release( );
	//glDisable( GL_DEPTH_TEST );

	//float left = -1.f,
	//	right = 0.5f,
	//	top = 1.f,
	//	bottom = -0.5f,
	//	width = right - left,
	//	height = top - bottom;

	//glm::mat4 quadMatrix(
	//	(right - left) / 2.f, 0.f, 0.f, 0.f,
	//	0.f, (top - bottom) / 2.f, 0.f, 0.f,
	//	0.f, 0.f, 1.f, 0.f,
	//	(right + left) / 2.f, (bottom + top) / 2.f, 0.f, 1.f
	//);

	//PR_SGBuffer* gBuffer = m_DeferredRenderer.GetGBuffer( );
	//PR_CTextureResource* lightResult = m_LightRenderer.ApplyTo( scene, gBuffer, &m_ShadowRenderer.GetShadowTexture( ) );
	//PR_CTextureResource* postResult = lightResult;// m_TestPosteffect.ApplyTo( lightResult );

	//if (!sf::Keyboard::isKeyPressed( sf::Keyboard::Space ))
	//	postResult = m_FXAA.ApplyTo( lightResult );

	//PR_RenderTexture( *postResult );
}