#include "stdafx.h"
#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Prism/Context.h>

/**	Constructor
*******************************************************************************/
CProgram::CProgram( ) {
}

/**	Init
*******************************************************************************/
void CProgram::Init( ) {
	m_shader	= prism::CAsset::Load<prism::CShaderAsset>( "test_shader" );
	m_mesh		= prism::CAsset::Load<prism::CMeshAsset>( "UnitCube.fbx" );

	m_mesh		= prism::CAsset::Load<prism::CMeshAsset>( "UnitCube.fbx" );
	m_mesh		= prism::CAsset::Load<prism::CMeshAsset>( "UnitCube.fbx" );
	m_mesh		= prism::CAsset::Load<prism::CMeshAsset>( "UnitCube.fbx" );
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

	glm::ivec2 contextSize = prism::CContext::Instance( )->GetWindowSize( );
	float contextRatio = (float)contextSize.x / contextSize.y;

	prism::CRenderScene scene;

	glm::mat4 camera = glm::ortho( -5.f * contextRatio, 5.f * contextRatio, -5.f, 5.f, 0.f, 10.f )
		* glm::lookAt( glm::vec3( 5.f, 5.f, 5.f ), glm::vec3( 0.f ), glm::vec3( 0.f, 1.f, 0.f ) );

	scene.SetCameraMatrix( camera );

	glm::mat4 world( 1.f );
	world = glm::translate( glm::mat4( 1.f ), glm::vec3( 0.5f, 0.2f, 0.f ) );
	world = glm::rotate( world, t, glm::vec3( 0.f, 1.f, 0.f ) );
	scene.AddMesh( *m_mesh, *m_shader, world );

	world = glm::translate( glm::mat4( 1.f ), glm::vec3( -0.5f, -0.2f, 0.f ) );
	world = glm::scale( world, glm::vec3( 0.5f ) );
	world = glm::rotate( world, t, glm::vec3( 0.f, 0.f, 1.f ) );

	scene.AddMesh( *m_mesh, *m_shader, world );

	m_renderer.Render( scene );
}