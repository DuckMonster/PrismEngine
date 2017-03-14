#include <Prism/Resource/FramebufferResource.h>

/**	Constructor
*******************************************************************************/
PR_CFramebufferResource::PR_CFramebufferResource( ) :
	m_Handle( -1 ), m_Width( 0 ), m_Height( 0 ) {
}

/**	Use
*******************************************************************************/
void PR_CFramebufferResource::Use( ) {
	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
}

/**	Set Resolution
*******************************************************************************/
void PR_CFramebufferResource::SetResolution( size_t width, size_t height ) {
	m_Width		= width;
	m_Height	= height;
}

/**	Bind Texture
*******************************************************************************/
void PR_CFramebufferResource::BindTexture( PR_CTextureResource* textureResource ) {
	PR_ASSERT_MSG( m_Width != 0 && m_Height != 0, "Framebuffer size not set" );

	GLuint texHandle = textureResource->GetHandle( );

	// Setup texture
	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
	glBindTexture( GL_TEXTURE_2D, texHandle );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texHandle, 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

/**	Create
*******************************************************************************/
bool PR_CFramebufferResource::Create( ) {
	glCreateFramebuffers( 1, &m_Handle );
	return true;
}

/**	Release
*******************************************************************************/
void PR_CFramebufferResource::Delete( ) {
	glDeleteFramebuffers( 1, &m_Handle );
}
