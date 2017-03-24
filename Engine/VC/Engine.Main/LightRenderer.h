#pragma once
#include <Prism/Render/DeferredRenderer.h>
#include <Prism/Resource/FramebufferResource.h>
#include <Prism/Resource/ShaderResource.h>

class CLightRenderer {
public:
	CLightRenderer( );
	PR_CTextureResource* ApplyTo( PR_CRenderScene& scene, PR_SGBuffer& gBuffer, PR_CTextureResource* shadowBuffer );

private:
	void LoadResources( );

	PR_CShaderResource*			m_Shader;
	PR_CFramebufferResource*	m_Framebuffer;
	PR_CTextureResource*		m_Output;
};