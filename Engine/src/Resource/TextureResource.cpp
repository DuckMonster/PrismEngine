#include <Prism\Resource\TextureResource.h>
#include <Prism\Resource\ImageResource.h>

/**	Constructor
*******************************************************************************/
PR_CTextureResource::PR_CTextureResource( ) :
	PR_CResource( ),
	m_Handle( -1 ), m_Width( 0 ), m_Height( 0 ) {
}

/**	Bind
*******************************************************************************/
void PR_CTextureResource::Bind( unsigned int id ) {
	PR_ASSERT_MSG( m_Handle != -1, "Texture isn't created" );
	glActiveTexture( GL_TEXTURE0 + id );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, m_Handle );
}

/**	Set Filter
*******************************************************************************/
void PR_CTextureResource::SetFilter( GLuint filter ) {
	Bind( 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
}

/**	Set Wrap Mode
*******************************************************************************/
void PR_CTextureResource::SetWrapMode( GLuint wrapMode ) {
	Bind( 0 );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode );
}

/**	Load Image
*******************************************************************************/
void PR_CTextureResource::LoadImage( PR_CImageResource* image ) {
	m_Width = image->GetWidth( );
	m_Height = image->GetHeight( );

	GLuint dataFormat = image->GetGLDataFormat( );
	GLuint dataType = image->GetGLDataType( );

	GLuint internalFormat;

	switch (dataFormat) {
	case GL_RGB:
	case GL_BGR:
		internalFormat = dataType == GL_FLOAT ? GL_RGB16F : GL_RGB; break;
	case GL_RGBA:
	case GL_BGRA:
		internalFormat = dataType == GL_FLOAT ? GL_RGBA16F : GL_RGBA; break;
	}

	Bind( 0 );
	glTexImage2D( GL_TEXTURE_2D, 0, internalFormat,
		m_Width, m_Height, 0,
		dataFormat, dataType, image->GetBits( ) );
}

/**	Load
*******************************************************************************/
bool PR_CTextureResource::Load( const std::string& resourcePath ) {
	Create( );

	// Load image
	PR_CImageResource* image = PR_CResource::Load<PR_CImageResource>( resourcePath.c_str( ) );
	if (image == NULL)
		return false;

	LoadImage( image );

	// Free image data
	image->Delete( );
	image = NULL;

	return true;
}

/**	Create
*******************************************************************************/
bool PR_CTextureResource::Create( ) {
	glGenTextures( 1, &m_Handle );
	Bind( 0 );

	SetWrapMode( GL_REPEAT );
	SetFilter( GL_LINEAR );
	//SetFilter( GL_NEAREST );

	return true;
}

/**	Release
*******************************************************************************/
void PR_CTextureResource::Delete( ) {
	if (m_Handle == -1)
		return;

	glDeleteTextures( 1, &m_Handle );
}
