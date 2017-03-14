#pragma once
#include <Prism/IProgram.h>
#include <Prism/Asset/MeshAsset.h>
#include <Prism/Asset/ShaderAsset.h>
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
	PR_CShaderAsset*	m_shader;

	PR_CMaterial		m_Material;
	PR_CMaterial		m_GroundMaterial;

	PR_CMeshAsset*		m_mesh;
	PR_CMeshAsset*		m_plane;
	PR_CRenderer		m_renderer;
};