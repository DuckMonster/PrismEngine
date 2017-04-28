#pragma once
#include <Prism/Render/Renderer.h>
#include <Prism/Resource/FramebufferResource.h>

class PR_CShadowMapRenderer : public PR_CRenderer {
public:
	PR_CShadowMapRenderer( );
	virtual void Render( PR_CRenderScene& scene );

	PR_CTextureResource& GetDepthTexture( ) { return m_DepthTexture; }
	PR_CTextureResource& GetShadowTexture( ) { return m_ShadowTexture; }

private:
	void LoadResources( );

	PR_CShaderResource			m_DepthShader;
	PR_CShaderResource			m_ShadowShader;

	PR_CFramebufferResource		m_DepthFramebuffer;
	PR_CTextureResource			m_DepthTexture;

	PR_CFramebufferResource		m_ShadowFramebuffer;
	PR_CTextureResource			m_ShadowTexture;
	PR_CTextureResource			m_ShadowDepthTexture;
};