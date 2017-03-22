#pragma once
#include <Prism/Resource/ShaderResource.h>
#include <Prism/Resource/TextureResource.h>

class PR_CMaterial {
public:
	PR_CMaterial( );
	~PR_CMaterial( );

	void SetShader( PR_CShaderResource* shader ) { m_Shader = shader; }
	PR_CShaderResource* GetShader( ) { return m_Shader; }

	void SetTexture( PR_CTextureResource* texture ) { m_Texture = texture; }
	PR_CTextureResource* GetTexture( ) { return m_Texture; }

	void SetColor( const glm::vec4& color ) { m_Color = color; }
	const glm::vec4& GetColor( ) { return m_Color; }

private:
	PR_CShaderResource*		m_Shader;
	PR_CTextureResource*	m_Texture;
	glm::vec4				m_Color;
};