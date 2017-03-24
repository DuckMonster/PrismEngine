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
#include "LightRenderer.h"
#include "TestPosteffect.h"

/**	CProgram
*******************************************************************************/
class CProgram : public PR_IProgram {
public:
	CProgram( );

	void Init( ) override;
	void Update( double delta ) override;
	void Render( double delta ) override;

private:
	PR_CShaderResource*			m_Shader;

	PR_CMaterial				m_Material;
	PR_CMaterial				m_GroundMaterial;

	PR_CMeshResource*			m_mesh;
	PR_CMeshResource*			m_plane;
	PR_CMeshResource*			m_Car;
	PR_CTextureResource*		m_Texture;
	PR_CRenderer				m_Renderer;
	PR_CDeferredRenderer		m_DeferredRenderer;
	PR_CShadowMapRenderer		m_ShadowRenderer;

	CLightRenderer				m_LightRenderer;
	CTestPosteffect				m_TestPosteffect;
	PR_CFXAA					m_FXAA;
};