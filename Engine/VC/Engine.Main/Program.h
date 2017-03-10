#pragma once
#include <Prism/IProgram.h>
#include <Prism/Asset/MeshAsset.h>
#include <Prism/Asset/ShaderAsset.h>
#include <Prism/Render/Renderer.h>

class CProgram : public prism::IProgram {
public:
	CProgram( );

	void Init( ) override;
	void Update( double delta ) override;
	void Render( double delta ) override;

private:
	prism::CShaderAsset*	m_shader;
	prism::CMeshAsset*		m_mesh;
	prism::CRenderer		m_renderer;
};