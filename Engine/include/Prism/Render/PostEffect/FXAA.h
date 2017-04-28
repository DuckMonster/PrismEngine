#pragma once
#include <Prism/Render/PostEffect/PostEffect.h>
#include <Prism/Resource/FramebufferResource.h>
#include <Prism/Resource/ShaderResource.h>

class PR_CFXAA : PR_IPostEffect {
public:
	PR_CFXAA( );
	virtual PR_CTextureResource* ApplyTo( PR_CTextureResource* texture ) override;

private:
	void LoadResources( );

	PR_CShaderResource			m_Shader;
	PR_CFramebufferResource		m_Framebuffer;
	PR_CTextureResource			m_Output;
};