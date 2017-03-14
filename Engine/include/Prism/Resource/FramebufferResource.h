#pragma once
#include <Prism/Resource/Resource.h>
#include <Prism/Resource/TextureResource.h>
#include <GL/glew.h>

class PR_CFramebufferResource : public PR_CResource {
public:
	static void Release( ) { glBindFramebuffer( GL_FRAMEBUFFER, 0 ); }

public:
	PR_CFramebufferResource( );

	void Use( );
	void SetResolution( size_t width, size_t height );

	void BindTexture( PR_CTextureResource* textureResource );

protected:
	bool Load( const std::string& filePath ) override { return false; }
	bool Create( ) override;
	void Delete( ) override;

private:
	GLuint		m_Handle;
	size_t		m_Width;
	size_t		m_Height;
};