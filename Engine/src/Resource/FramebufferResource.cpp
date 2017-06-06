#include <Prism/Resource/FramebufferResource.h>
#include <Prism/Context.h>

/**	Release
*******************************************************************************/
void PR_CFramebufferResource::Release( ) {
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

	glm::ivec2 windowSize = PR_CContext::Instance( )->GetWindowSize( );
	glViewport( 0, 0, windowSize.x, windowSize.y );
}

/**	Constructor
*******************************************************************************/
PR_CFramebufferResource::PR_CFramebufferResource( ) :
	m_Handle( 0 ), m_Width( 0 ), m_Height( 0 ) {
}

/**	Destructor
*******************************************************************************/
PR_CFramebufferResource::~PR_CFramebufferResource( ) {
	Delete( );
}

/**	Is Complete
*******************************************************************************/
bool PR_CFramebufferResource::IsComplete( ) const {
	if (!IsValid( ))
		return false;

	GLint prevFBO;
	glGetIntegerv( GL_FRAMEBUFFER_BINDING, &prevFBO );

	Bind( );
	bool isComplete = (glCheckFramebufferStatus( GL_FRAMEBUFFER ) == GL_FRAMEBUFFER_COMPLETE);
	glBindFramebuffer( GL_FRAMEBUFFER, prevFBO );

	return isComplete;
}

/**	Bind
*******************************************************************************/
void PR_CFramebufferResource::Bind( ) const {
	// Bind frame buffer and draw buffers
	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );

	if (!m_ColorBuffers.empty( ))
		glDrawBuffers( m_ColorBuffers.size( ), &m_ColorBuffers[0] );

	glViewport( 0, 0, m_Width, m_Height );
}

/**	Set Resolution
*******************************************************************************/
void PR_CFramebufferResource::SetResolution( size_t width, size_t height ) {
	m_Width		= width;
	m_Height	= height;
}

/**	Bind Texture Color
*******************************************************************************/
void PR_CFramebufferResource::BindTextureColor( PR_CTextureResource& texture, size_t index, GLuint format, GLuint type ) {
	PR_ASSERT_MSG( m_Width != 0 && m_Height != 0, "Framebuffer size not set" );

	// Get the correct data format (based on internal format)
	GLuint dataFormat = format;
	switch (format) {
	case GL_RGB16F: dataFormat = GL_RGB; break;
	case GL_RGBA16F: dataFormat = GL_RGBA; break;
	}

	GLuint texHandle = texture.GetHandle( );

	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
	glBindTexture( GL_TEXTURE_2D, texHandle );

	glTexImage2D( GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, dataFormat, type, nullptr );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, texHandle, 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );

	// Add to color buffer list
	m_ColorBuffers.push_back( GL_COLOR_ATTACHMENT0 + index );
}

/**	Bind Texture Depth
*******************************************************************************/
void PR_CFramebufferResource::BindTextureDepth( PR_CTextureResource& texture ) {
	PR_ASSERT_MSG( m_Width != 0 && m_Height != 0, "Framebuffer size not set" );

	GLuint texHandle = texture.GetHandle( );

	glBindFramebuffer( GL_FRAMEBUFFER, m_Handle );
	glBindTexture( GL_TEXTURE_2D, texHandle );

	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texHandle, 0 );

	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

/**	Create ( defaults to current context size )
*******************************************************************************/
bool PR_CFramebufferResource::Create( ) {
	return Create( PR_CContext::Instance( )->GetWindowWidth( ), PR_CContext::Instance( )->GetWindowHeight( ) );
}

/**	Create
*******************************************************************************/
bool PR_CFramebufferResource::Create( size_t width, size_t height ) {
	if (IsValid( ))
		return false;

	glCreateFramebuffers( 1, &m_Handle );
	SetResolution( width, height );

	return true;
}

/**	Release
*******************************************************************************/
void PR_CFramebufferResource::Delete( ) {
	if (!IsValid( ))
		return;

	glDeleteFramebuffers( 1, &m_Handle );
}
