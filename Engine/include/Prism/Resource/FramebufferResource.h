#pragma once
#include <vector>
#include <Prism/Resource/Resource.h>
#include <Prism/Resource/TextureResource.h>
#include <GL/glew.h>

class PR_CFramebufferResource : public PR_CResource {
public:
	static void Release( );

public:
	PR_CFramebufferResource( );
	~PR_CFramebufferResource( );

	bool Create( );
	bool Create( size_t width, size_t height );
	void Delete( );

	GLuint GetHandle( ) { return m_Handle; }
	bool IsValid( ) { return m_Handle != -1; }
	bool IsComplete( );

	void Bind( );
	void SetResolution( size_t width, size_t height );

	void BindTextureColor( PR_CTextureResource& texture, size_t index, GLuint format, GLuint type );
	void BindTextureDepth( PR_CTextureResource& texture );

protected:

private:
	std::vector<GLenum> m_ColorBuffers;

	GLuint		m_Handle;
	size_t		m_Width;
	size_t		m_Height;
};