#pragma once
#include <Prism/Resource/ShaderResource.h>

class PR_CMaterial {
public:
	PR_CMaterial( );
	~PR_CMaterial( );

	void SetShader( PR_CShaderResource* shader ) { m_Shader = shader; }
	PR_CShaderResource* GetShader( ) { return m_Shader; }

	void SetColor( const glm::vec4& color ) { m_Color = color; }
	const glm::vec4& GetColor( ) { return m_Color; }

private:
	PR_CShaderResource*	m_Shader;
	glm::vec4			m_Color;
};