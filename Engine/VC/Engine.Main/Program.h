#pragma once
#include <Prism/IProgram.h>
#include <Prism/Resource/MeshResource.h>
#include <Prism/Resource/ShaderResource.h>
#include <Prism/Resource/TextureResource.h>
#include <Prism/Resource/FramebufferResource.h>
#include <Prism/Render/Renderer.h>

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
	PR_CTextureResource*		m_Texture;
	PR_CRenderer				m_renderer;

	PR_CFramebufferResource*	m_Framebuffer;
	PR_CTextureResource*		m_FBTexture;
};