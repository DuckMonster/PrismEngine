#pragma once
#include <Prism/Render/Renderer.h>
#include <Prism/Resource/FramebufferResource.h>

struct PR_SGBuffer {
	PR_CTextureResource* Position;
	PR_CTextureResource* Normal;
	PR_CTextureResource* Diffuse;
	PR_CTextureResource* Depth;
};

class PR_CDeferredRenderer : public PR_CRenderer {
public:
	PR_CDeferredRenderer( );
	virtual void Render( PR_CRenderScene& scene );

	PR_SGBuffer& GetGBuffer( ) { return m_GBuffer; }

private:
	void LoadResources( );

	PR_CShaderResource*			m_Shader;
	PR_CFramebufferResource*	m_Framebuffer;
	PR_SGBuffer					m_GBuffer;
};