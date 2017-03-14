#pragma once
#include <Prism/Asset/ShaderAsset.h>

class PR_CMaterial {
public:
	PR_CMaterial( );
	~PR_CMaterial( );

	void SetShader( PR_CShaderAsset* shader ) { m_Shader = shader; }
	PR_CShaderAsset* GetShader( ) { return m_Shader; }

	void SetColor( const glm::vec4& color ) { m_Color = color; }
	const glm::vec4& GetColor( ) { return m_Color; }

private:
	PR_CShaderAsset*	m_Shader;
	glm::vec4			m_Color;
};