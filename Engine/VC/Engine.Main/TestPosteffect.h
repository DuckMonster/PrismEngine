#pragma once
#include <Prism/Render/PostEffect/PostEffect.h>
#include <Prism/Resource/FramebufferResource.h>
#include <Prism/Resource/ShaderResource.h>

class CTestPosteffect : PR_IPostEffect {
public:
	CTestPosteffect( );
	virtual PR_CTextureResource* ApplyTo( PR_CTextureResource* texture ) override;

private:
	void LoadResources( );

	PR_CShaderResource			m_Shader;
	PR_CFramebufferResource		m_Framebuffer;
	PR_CTextureResource			m_Output;
};