#pragma once
#include "Prism/Asset/Asset.h"
#include <GL/glew.h>

class PR_CMeshAsset : public PR_CAsset {
public:
	PR_CMeshAsset( );
	~PR_CMeshAsset( );

	void Release( ) override;
	void Render( );

private:
	bool Load( const std::string& path ) override;
	bool LoadAssimp( const char* fileName );

	GLuint			m_objectHandle;

	GLuint			m_positionHandle;
	GLuint			m_normalHandle;
	GLuint			m_uvHandle;

	GLuint			m_elementsHandle;
	unsigned int	m_vertexCount;
};
