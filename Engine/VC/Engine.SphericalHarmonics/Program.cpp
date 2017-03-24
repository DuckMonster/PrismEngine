#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Prism/Context.h>
#include <Prism/Utils/DirLight.h>
#include <Prism/Utils/DefaultShader.h>
#include <Prism/Utils/RenderUtils.h>
#include <FreeImage/FreeImage.h>

#include "SH.h"

#define SAMPLE_N 1000
#define SAMPLE_N_SQRT 100

namespace {
	float frand( ) { return (float)rand( ) / RAND_MAX; }
}

/**	Constructor
*******************************************************************************/
CProgram::CProgram( ) {
}

/**	Init
*******************************************************************************/
void CProgram::Init( ) {
	PR_CLogger::sm_LogLevel = PR_LOG_LEVEL_TRIVIAL;

	FreeImage_Initialise( );

	FREE_IMAGE_FORMAT format;
	FIBITMAP* bitmap( NULL );

	format = FreeImage_GetFileType( "./Assets/Skybox.hdr" );

	if (format == FIF_UNKNOWN)
		format = FreeImage_GetFIFFromFilename( "./Assets/Skybox.hdr" );

	if (format == FIF_UNKNOWN)
		std::cout << "File format is unknown.\n";

	if (FreeImage_FIFSupportsReading( format ))
		bitmap = FreeImage_Load( format, "./Assets/Skybox.hdr" );

	BYTE* bits( FreeImage_GetBits( bitmap ) );
	int width( FreeImage_GetWidth( bitmap ) );
	int height( FreeImage_GetHeight( bitmap ) );

	// Generate SH coefficients
	SH_Sample* samples = new SH_Sample[SAMPLE_N];
	SH_GenSamples( samples, SAMPLE_N );

	auto imgFunc = []( double theta, double phi ) {
		float v = glm::max<float>( 0.f, 5.f * glm::cos( theta ) - 4 ) +
			glm::max<float>( 0.f, -4.f * glm::sin( theta - glm::pi<float>( )) * cos( phi - 2.5f ) - 3 );

		return glm::vec3( v ); 
	};

	glm::vec3 projection[9];
	SH_Project( imgFunc, samples, SAMPLE_N, projection );

	glm::vec2 sample_sph[SAMPLE_N];

	for (size_t i = 0; i < SAMPLE_N; i++) {
		sample_sph[i] = samples[i].sph;
	}

	delete[] samples;
	//

	glGenTextures( 1, &m_Texture );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, bits );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	FreeImage_Unload( bitmap );
	FreeImage_DeInitialise( );

	m_SkyboxShader = PR_CResource::Load<PR_CShaderResource>( "shdr_skybox" );
	m_ReflectionShader = PR_CResource::Load<PR_CShaderResource>( "shdr_reflection" );
	m_SampleShader = PR_CResource::Load<PR_CShaderResource>( "shdr_samples" );
	m_Sphere = PR_CResource::Load<PR_CMeshResource>( "UnitSphere.fbx" );
	m_Head = PR_CResource::Load<PR_CMeshResource>( "Head/head.fbx" );

	//--------------------------------------------------- Set up samples
	glGenVertexArrays( 1, &m_SampleVAO );
	glGenBuffers( 1, &m_SampleVBO );
	glGenBuffers( 1, &m_ColorVBO );

	glBindBuffer( GL_ARRAY_BUFFER, m_SampleVBO );
	glBufferData( GL_ARRAY_BUFFER, SAMPLE_N * sizeof( glm::vec2 ), sample_sph, GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, m_ColorVBO );
	glBufferData( GL_ARRAY_BUFFER, SAMPLE_N * sizeof( glm::vec3 ), sample_sph, GL_STATIC_DRAW );

	glBindVertexArray( m_SampleVAO );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, false, 0, 0 );

	glBindVertexArray( 0 );
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

	glm::vec3 eye = glm::vec3( glm::sin( t * 0.2f ), glm::cos( t * 0.15f ) * 0.5f, glm::cos( t * 0.2f ) ) * 5.f;

	glm::mat4 proj = glm::perspective( glm::radians( 90.f ), contextRatio, 0.01f, 2.f );
	glm::mat4 view = glm::lookAt(
		eye,
		glm::vec3( 0.f ),
		glm::vec3( 0.f, 1.f, 0.f ) );

	m_SkyboxShader->Use( );
	m_SkyboxShader->Set( "u_Projection", proj );
	m_SkyboxShader->Set( "u_View", view );

	glDepthMask( GL_FALSE );
	//m_Sphere->Render( );
	glDepthMask( GL_TRUE );

	proj = glm::perspective( glm::radians( 90.f ), contextRatio, 0.1f, 10.f );

	m_ReflectionShader->Use( );
	m_ReflectionShader->Set( "u_Projection", proj );
	m_ReflectionShader->Set( "u_View", view );
	//m_ReflectionShader->Set( "u_Model", glm::mat4(1.f));
	m_ReflectionShader->Set( "u_Model", glm::scale( glm::mat4( 1.f ), glm::vec3( 4.f ) ) );
	m_ReflectionShader->Set( "u_Eye", eye );

	glDisable( GL_DEPTH_TEST );
	m_SampleShader->Use( );
	m_SampleShader->Set( "u_Camera", proj * view );
	glBindVertexArray( m_SampleVAO );
	glPointSize( 1.f );
	glDrawArrays( GL_POINTS, 0, SAMPLE_N );

	//m_Sphere->Render( );
	//m_Head->Render( );
	//glDrawArrays( GL_QUADS, 0, 4 );

	//RenderSamples( );
}

/**	Render Samples
*******************************************************************************/
void CProgram::RenderSamples( ) {
	PR_QuadShader( )->Use( );
	PR_QuadShader( )->Set( "u_QuadMatrix", glm::mat4( 1.f ) );
	glBindTexture( GL_TEXTURE_2D, m_Texture );
	glDrawArrays( GL_QUADS, 0, 4 );
}