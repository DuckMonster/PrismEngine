#pragma once
#include <Prism/Resource/Resource.h>
#include <GL/glew.h>

class PR_CImageResource;

class PR_CTextureResource : public PR_CResource {
public:
	PR_CTextureResource( );

	bool LoadFromFile( const std::string& resourcePath );
	bool LoadFromImage( PR_CImageResource& image );

	void Create( );
	void Delete( );

	void Bind( unsigned int id );
	const GLuint& GetHandle( ) { return m_Handle; }

	void SetFilter( GLuint filter );
	void SetWrapMode( GLuint wrapMode );

protected:

private:
	int			m_Width;
	int			m_Height;
	GLuint		m_Handle;
};