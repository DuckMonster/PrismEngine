#include <Prism/Render/TextMesh.h>
#include <Prism/Resource/ImageResource.h>
#include <Prism/Context.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

namespace {
	const char* SRC_TEXT_VERT = "\
#version 330 core\n\
\
layout( location = 0 ) in vec2 a_Position;\
layout( location = 1 ) in vec2 a_UV;\
\
uniform mat4 u_Matrix;\
\
out vec2 f_UV;\
\
void main( ) {\
	gl_Position = u_Matrix * vec4(a_Position, 0.0, 1.0);\
	gl_Position.z = -1.0;\
\
	f_UV = a_UV;\
}";

	const char* SRC_TEXT_FRAG = "\
#version 330 core\n\
\
in vec2 f_UV;\
\
uniform sampler2D u_Sampler;\
\
out vec4 o_Color;\
\
void main( ) {\
	o_Color = texture(u_Sampler, f_UV);\
}";
}

PR_CTextMesh::PR_CTextMesh( ) :
	m_VAO( -1 ), m_VBO( -1 ),
	m_GlyphWidth( 0 ), m_GlyphHeight( 0 ),
	m_CurrentGlyphCount( 0 ), m_MaxGlyphCount( 0 ) {
}

PR_CTextMesh::~PR_CTextMesh( ) {
}

void PR_CTextMesh::LoadFont( const std::string & file, size_t glyphWidth, size_t glyphHeight ) {
	Init( );

	m_GlyphWidth = glyphWidth;
	m_GlyphHeight = glyphHeight;

	PR_CImageResource image;
	image.LoadFromFile( file );

	m_ImgWidth = image.GetWidth( );
	m_ImgHeight = image.GetHeight( );

	m_FontTexture.LoadFromImage( image );
	m_FontTexture.SetFilter( GL_NEAREST );
}

void PR_CTextMesh::SetString( const std::string & str ) {

	m_CurrentGlyphCount = str.size( );

	uint glyphsPerRow = glm::floor<uint>( m_ImgWidth / m_GlyphWidth );
	float glyphU = (float)m_GlyphWidth / (float)m_ImgWidth;
	float glyphV = (float)m_GlyphHeight / (float)m_ImgHeight;

	//glyphU *= 10.f;
	//glyphV *= 10.f;

	// Buffer
	float* data = new float[m_CurrentGlyphCount * 4 * 4];
	for (int i=0; i < m_CurrentGlyphCount; i++) {
		const char glyphIndex = str[i] - 32;

		int index = i * 4 * 4;
		float x = i * (m_GlyphWidth);
		float u = glyphIndex % glyphsPerRow; // Hard coded 18 glyphs per row
		float v = glm::floor( glyphIndex / glyphsPerRow );

		// Top-left
		data[index++] = x;
		data[index++] = -(int)m_GlyphHeight;

		data[index++] = u * glyphU;
		data[index++] = 1.f - v * glyphV;

		// Bottom-left
		data[index++] = x;
		data[index++] = 0.f;

		data[index++] = u * glyphU;
		data[index++] = 1.f - (v + 1) * glyphV;

		// Bottom-right
		data[index++] = x + m_GlyphWidth;
		data[index++] = 0.f;

		data[index++] = (u + 1) * glyphU;
		data[index++] = 1.f - (v + 1) * glyphV;

		// Top-right
		data[index++] = x + m_GlyphWidth;
		data[index++] = -(int)m_GlyphHeight;

		data[index++] = (u + 1) * glyphU;
		data[index++] = 1.f - v * glyphV;
	}

	glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

	if (m_CurrentGlyphCount > m_MaxGlyphCount) {
		glBufferData( GL_ARRAY_BUFFER, m_CurrentGlyphCount * 4 * 4 * sizeof( float ), data, GL_DYNAMIC_DRAW );
		m_MaxGlyphCount = m_CurrentGlyphCount;
	}
	else {
		glBufferSubData( GL_ARRAY_BUFFER, 0, m_CurrentGlyphCount * 4 * 4 * sizeof( float ), data );
	}
}

void PR_CTextMesh::Render( const int x, const int y, const int scale ) {
	Init( );

	int screenWidth = PR_CContext::Instance( )->GetWindowWidth( );
	int screenHeight = PR_CContext::Instance( )->GetWindowHeight( );

	m_UIMatrix = glm::ortho<float>( 0.f, screenWidth, screenHeight, 0.f );
	m_UIMatrix[3][0] += (float)x * m_UIMatrix[0][0];
	m_UIMatrix[3][1] += (float)y * m_UIMatrix[1][1];

	m_UIMatrix[0][0] *= scale;
	m_UIMatrix[1][1] *= scale;

	mat4 someMat = glm::translate( glm::mat4( ), glm::vec3( 5.f, 10.f, 0.f ) );

	m_Shader.Use( );
	m_Shader.Set( "u_Matrix", m_UIMatrix );
	m_FontTexture.Bind( 0 );

	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	glBindVertexArray( m_VAO );
	glDrawArrays( GL_QUADS, 0, m_CurrentGlyphCount * 4 );
	glBindVertexArray( 0 );
}

void PR_CTextMesh::Init( ) {
	// Already initialized
	if (m_VAO != -1)
		return;

	m_Shader.CompileSource( SRC_TEXT_VERT, SRC_TEXT_FRAG );

	glGenVertexArrays( 1, &m_VAO );
	glGenBuffers( 1, &m_VBO );

	glBindVertexArray( m_VAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_VBO );

	glEnableVertexAttribArray( 0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, false, 4 * sizeof( float ), 0 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, false, 4 * sizeof( float ), (void*)(2 * sizeof( float )) );

	glBindVertexArray( 0 );
}
