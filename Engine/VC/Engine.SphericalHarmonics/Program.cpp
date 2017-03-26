#include "Program.h"
#include <glm/gtc/matrix_transform.hpp>
#include <Prism/Context.h>
#include <Prism/Utils/DirLight.h>
#include <Prism/Utils/DefaultShader.h>
#include <Prism/Utils/RenderUtils.h>
#include <Prism/Resource/ImageResource.h>

#include "SH.h"

#define SAMPLE_N 100
#define SAMPLE_N_SQRT 100

namespace {
	float frand( ) { return (float)rand( ) / RAND_MAX; }
	glm::vec3 GetFloatColor( glm::vec2 uv, FIBITMAP* bmap ) {
		int width = FreeImage_GetWidth( bmap );
		int height = FreeImage_GetHeight( bmap );
		BYTE* bits = FreeImage_GetBits( bmap );

		int x = (int)(uv.x * width);
		int y = (int)(uv.y * height);

		int index = (x + y * width);

		glm::vec3 clr;
		clr.x = *((float*)(bits + index * 12));
		clr.y = *((float*)(bits + index * 12 + 4));
		clr.z = *((float*)(bits + index * 12 + 8));

		return clr;
	}
}

/**	Constructor
*******************************************************************************/
CProgram::CProgram( ) {
}

/**	Init
*******************************************************************************/
void CProgram::Init( ) {
	PR_CLogger::sm_LogLevel = PR_LOG_LEVEL_TRIVIAL;

	m_SkyboxShader = PR_CResource::Load<PR_CShaderResource>( "shdr_skybox" );
	m_ReflectionShader = PR_CResource::Load<PR_CShaderResource>( "shdr_sh" );
	m_SampleShader = PR_CResource::Load<PR_CShaderResource>( "shdr_samples" );
	m_Sphere = PR_CResource::Load<PR_CMeshResource>( "UnitSphere.fbx" );
	m_Head = PR_CResource::Load<PR_CMeshResource>( "Head/head.fbx" );

	//PR_CImageResource* image = PR_CResource::Load<PR_CImageResource>( "sample.bmp" );
	PR_CImageResource* image = PR_CResource::Load<PR_CImageResource>( "Skybox.hdr" );
	m_SkyboxTexture = PR_CResource::Create<PR_CTextureResource>( );
	m_SkyboxTexture->LoadImage( image );

	float* pix = (float*)image->GetPixel( 200, 100 );
	glm::vec3 clr( pix[0], pix[1], pix[2] );

	//--------------------------------------------------- Set up samples
	// Generate SH coefficients
	SH_Sample* samples = new SH_Sample[SAMPLE_N];
	SH_GenSamples( samples, SAMPLE_N );

	auto imgFunc = []( double theta, double phi ) {
		float v = glm::max<float>( 0.f, 5.f * glm::cos( theta ) - 4 ) +
			glm::max<float>( 0.f, -4.f * glm::sin( theta - glm::pi<float>( ) ) * cos( phi - 2.5f ) - 3 );

		return glm::vec3( v );
	};

	glm::vec3 projection[9];
	SH_Project( image, samples, SAMPLE_N, projection );

	projection[0] = glm::vec3( 0.39925f );
	projection[1] = glm::vec3( -0.21075f );
	projection[2] = glm::vec3( 0.28687f );
	projection[3] = glm::vec3( 0.28277f );
	projection[4] = glm::vec3( -0.31530f );
	projection[5] = glm::vec3( -0.00040f );
	projection[6] = glm::vec3( 0.13159f );
	projection[7] = glm::vec3( 0.00098f );
	projection[8] = glm::vec3( -0.09359f );

	m_ReflectionShader->Set( "u_SHCoeff", projection, 9 );

	glm::vec2 sample_sph[SAMPLE_N];
	float sample_coeff[SAMPLE_N * 9];

	for (size_t i = 0; i < SAMPLE_N; i++) {
		sample_sph[i] = samples[i].sph;
		for (size_t j = 0; j < 9; j++) {
			sample_coeff[i * 9 + j] = samples[i].coeff[j];
		}
	}

	delete[] samples;

	glGenVertexArrays( 1, &m_SampleVAO );
	glGenBuffers( 1, &m_SampleVBO );
	glGenBuffers( 1, &m_ColorVBO );
	glGenBuffers( 1, &m_CoeffVBO );

	glBindBuffer( GL_ARRAY_BUFFER, m_SampleVBO );
	glBufferData( GL_ARRAY_BUFFER, SAMPLE_N * sizeof( glm::vec2 ), sample_sph, GL_STATIC_DRAW );
	/*glBindBuffer( GL_ARRAY_BUFFER, m_ColorVBO );
	glBufferData( GL_ARRAY_BUFFER, SAMPLE_N * sizeof( glm::vec3 ), sample_sph, GL_STATIC_DRAW );*/
	glBindBuffer( GL_ARRAY_BUFFER, m_CoeffVBO );
	glBufferData( GL_ARRAY_BUFFER, SAMPLE_N * 9 * sizeof( float ), sample_coeff, GL_STATIC_DRAW );

	glBindVertexArray( m_SampleVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_SampleVBO );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, false, 0, 0 );
	glBindBuffer( GL_ARRAY_BUFFER, m_CoeffVBO );
	glEnableVertexAttribArray( 2 );
	glEnableVertexAttribArray( 3 );
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 2, 3, GL_FLOAT, false, sizeof( float ) * 9, 0 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, false, sizeof( float ) * 9, (void*)(sizeof( float ) * 3) );
	glVertexAttribPointer( 4, 3, GL_FLOAT, false, sizeof( float ) * 9, (void*)(sizeof( float ) * 6) );

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

	glm::mat4 proj = glm::perspective( glm::radians( 90.f ), contextRatio, 0.01f, 10.f );
	glm::mat4 view = glm::lookAt(
		eye,
		glm::vec3( 0.f ),
		glm::vec3( 0.f, 1.f, 0.f ) );

	m_SkyboxShader->Use( );
	m_SkyboxShader->Set( "u_Projection", proj );
	m_SkyboxShader->Set( "u_View", view );

	glDepthMask( GL_FALSE );
	m_Sphere->Render( );
	glDepthMask( GL_TRUE );

	//proj = glm::ortho( -contextRatio * 2.f, contextRatio * 2.f, -2.f, 2.f, 0.f, 10.f );
	//view = glm::lookAt(
	//	glm::vec3( 2.f, 2.f, 2.f ),
	//	glm::vec3( 0.f ),
	//	glm::vec3( 0.f, 1.f, 0.f ) );

	m_ReflectionShader->Use( );
	m_ReflectionShader->Set( "u_Projection", proj );
	m_ReflectionShader->Set( "u_View", view );
	m_ReflectionShader->Set( "u_Model", glm::scale( glm::mat4( 1.f ), glm::vec3( 1.f ) ) );
	m_ReflectionShader->Set( "u_Eye", eye );

	m_Sphere->Render( );

	int c_index = 0;

	for (int i=1; i < 9; i++)
		if (sf::Keyboard::isKeyPressed( (sf::Keyboard::Key)(sf::Keyboard::Num0 + i) ))
			c_index = i;

	m_SampleShader->Use( );
	m_SampleShader->Set( "u_Camera", proj * view );
	m_SampleShader->Set( "u_CoeffIndex", c_index );
	glBindVertexArray( m_SampleVAO );
	glPointSize( 2.f );
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
	glDrawArrays( GL_QUADS, 0, 4 );
}