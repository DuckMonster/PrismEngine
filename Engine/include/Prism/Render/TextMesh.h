#pragma once
#include <GL/glew.h>
#include <Prism/Resource/TextureResource.h>
#include <Prism/Resource/ShaderResource.h>

class PR_CTextMesh {
public:
	PR_CTextMesh( );
	~PR_CTextMesh( );

	void LoadFont( const std::string& file, size_t glyphWidth, size_t glyphHeight );
	void SetString( const std::string& str );

	void Render( const int x, const int y, const int scale );

private:
	void Init( );

	GLuint		m_VAO;
	GLuint		m_VBO;

	size_t		m_ImgWidth, m_ImgHeight;
	size_t		m_GlyphWidth, m_GlyphHeight;
	size_t		m_GlyphsPerRow;

	PR_CTextureResource m_FontTexture;
	PR_CShaderResource	m_Shader;

	glm::mat4	m_UIMatrix;

	// Current info
	size_t		m_CurrentGlyphCount;
	size_t		m_MaxGlyphCount;
};