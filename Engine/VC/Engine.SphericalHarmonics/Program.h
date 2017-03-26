#pragma once
#include <Prism/IProgram.h>
#include <Prism/Resource/MeshResource.h>
#include <Prism/Resource/ShaderResource.h>
#include <Prism/Resource/TextureResource.h>
#include <Prism/Resource/FramebufferResource.h>
#include <Prism/Render/Renderer.h>
#include <Prism/Render/DeferredRenderer.h>
#include <Prism/Render/ShadowMapRenderer.h>
#include <Prism/Render/PostEffect/FXAA.h>

/**	CProgram
*******************************************************************************/
class CProgram : public PR_IProgram {
public:
	CProgram( );

	void Init( ) override;
	void Update( double delta ) override;
	void Render( double delta ) override;

private:
	void RenderSamples( );

	PR_CShaderResource* m_SkyboxShader;
	PR_CShaderResource* m_ReflectionShader;
	PR_CShaderResource* m_SampleShader;

	PR_CMeshResource*	m_Sphere;
	PR_CMeshResource*	m_Head;

	PR_CTextureResource*m_SkyboxTexture;

	GLuint m_SampleVAO;
	GLuint m_SampleVBO;
	GLuint m_ColorVBO;
	GLuint m_CoeffVBO;
};