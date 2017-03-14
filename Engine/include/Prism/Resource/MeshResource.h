#pragma once
#include "Prism/Resource/Resource.h"
#include <GL/glew.h>

class PR_CMeshResource : public PR_CResource {
public:
	PR_CMeshResource( );
	~PR_CMeshResource( );

	void Render( );

private:
	bool Load( const std::string& path ) override;
	bool Create( ) override;
	void Delete( ) override;

	bool LoadAssimp( const char* fileName );

	GLuint			m_objectHandle;

	GLuint			m_positionHandle;
	GLuint			m_normalHandle;
	GLuint			m_uvHandle;

	GLuint			m_elementsHandle;
	unsigned int	m_vertexCount;
};
