#pragma once
#include <Prism/Resource/Resource.h>
#include <GL/glew.h>

class PR_CTextureResource : public PR_CResource {
public:
	PR_CTextureResource( );

	void Bind( unsigned int id );
	const GLuint& GetHandle( ) { return m_Handle; }

protected:
	bool Load( const std::string& resourcePath ) override;
	bool Create( ) override;
	void Delete( ) override;

private:
	int			m_Width;
	int			m_Height;
	GLuint		m_Handle;
};