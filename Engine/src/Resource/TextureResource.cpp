#include <Prism\Resource\TextureResource.h>
#include <SOIL\SOIL.h>

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
	glBindTexture( GL_TEXTURE_2D, m_Handle );
}

/**	Load
*******************************************************************************/
bool PR_CTextureResource::Load( const std::string& resourcePath ) {
	Create( );

	//--------------------------------------------------- Load image with SOIL
	unsigned char* img;
	img = SOIL_load_image( resourcePath.c_str( ), &m_Width, &m_Height, 0, SOIL_LOAD_RGBA );

	// Failed
	if (img == NULL)
		return false;

	//--------------------------------------------------- Upload pixel data
	Bind( 0 );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img );

	//--------------------------------------------------- Free from SOIL
	SOIL_free_image_data( img );

	return true;
}

/**	Create
*******************************************************************************/
bool PR_CTextureResource::Create( ) {
	glGenTextures( 1, &m_Handle );
	Bind( 0 );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	return true;
}

/**	Release
*******************************************************************************/
void PR_CTextureResource::Delete( ) {
	if (m_Handle == -1)
		return;

	glDeleteTextures( 1, &m_Handle );
}
