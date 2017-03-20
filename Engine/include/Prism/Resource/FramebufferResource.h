#pragma once
#include <Prism/Resource/Resource.h>
#include <Prism/Resource/TextureResource.h>
#include <GL/glew.h>

class PR_CFramebufferResource : public PR_CResource {
public:
	static void Release( );

public:
	PR_CFramebufferResource( );

	GLuint GetHandle( ) { return m_Handle; }
	bool IsComplete( );

	void Bind( );
	void SetResolution( size_t width, size_t height );

	void BindTextureColor( PR_CTextureResource* texture, size_t index, GLuint format, GLuint type );
	void BindTextureDepth( PR_CTextureResource* texture );

protected:
	bool Load( const std::string& filePath ) override { return false; }
	bool Create( ) override;
	void Delete( ) override;

private:
	GLuint		m_Handle;
	size_t		m_Width;
	size_t		m_Height;
};