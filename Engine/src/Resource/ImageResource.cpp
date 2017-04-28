#include <Prism\Resource\ImageResource.h>
#include <FreeImage\FreeImage.h>

//--------------------------------------------------- FreeImage library
/**	Initalize Free Image
*******************************************************************************/
void PR_CImageResource::InitFreeImage( ) {
	FreeImage_Initialise( true );
}

/**	Release Free Image
*******************************************************************************/
void PR_CImageResource::ReleaseFreeImage( ) {
	FreeImage_DeInitialise( );
}

bool PR_CImageResource::sm_FreeImageInitialized = false;
//---------------------------------------------------

/**	Constructor
*******************************************************************************/
PR_CImageResource::PR_CImageResource( ) : PR_CResource( ),
m_Format( FIF_UNKNOWN ),
m_Bitmap( NULL ),
m_Width( -1 ),
m_Height( -1 ),
m_Bits( NULL ) {
}

/**	Destructor
*******************************************************************************/
PR_CImageResource::~PR_CImageResource( ) {
	Delete( );
}

/**	Get Pixel (from uv coordinates)
*******************************************************************************/
const unsigned char* PR_CImageResource::GetPixel( float u, float v ) {
	int x = (int)(u * m_Width);
	int y = (int)(v * m_Height);

	return GetPixel( x, y );
}

/**	Get Pixel (pixel coordinates)
*******************************************************************************/
const unsigned char* PR_CImageResource::GetPixel( int x, int y ) {
	int index = x + y * m_Width;
	return m_Bits + (index * (m_BPP / 8));
}

/**	Get OpenGL Color Format
*******************************************************************************/
const GLuint PR_CImageResource::GetGLDataFormat( ) {
	// Note: FreeImage loads files in BGR format unless they're floating point images

	if (GetGLDataType( ) == GL_FLOAT) {
		switch (m_ColorFormat) {
		case FIC_RGB: return GL_RGB;
		case FIC_RGBALPHA: return GL_RGBA;
		default: return GL_RED;
		};
	}
	else {
		switch (m_ColorFormat) {
		case FIC_RGB: return GL_BGR;
		case FIC_RGBALPHA: return GL_BGRA;
		default: return GL_RED;
		};
	}
}

/**	Get OpenGL Image Format
*******************************************************************************/
const GLuint PR_CImageResource::GetGLDataType( ) {
	switch (m_DataType) {
	case FIT_BITMAP: return GL_UNSIGNED_BYTE;
	case FIT_UINT16: return GL_UNSIGNED_SHORT;
	case FIT_INT16: return GL_SHORT;
	case FIT_UINT32: return GL_UNSIGNED_INT;
	case FIT_INT32: return GL_INT;

	case FIT_RGBF:
	case FIT_FLOAT: return GL_FLOAT;

	default: return GL_UNSIGNED_BYTE;
	};
}

/**	Load
*******************************************************************************/
bool PR_CImageResource::LoadFromFile( const std::string& path ) {
	if (sm_FreeImageInitialized)
		InitFreeImage( );

	if (IsLoaded( ))
		Delete( );

	m_Format = FreeImage_GetFileType( path.c_str( ) );

	// Try to parse file format from the filename
	if (m_Format == FIF_UNKNOWN)
		m_Format = FreeImage_GetFIFFromFilename( path.c_str( ) );
	// Nothing found :(
	if (m_Format == FIF_UNKNOWN) {
		PR_CLogger( PR_LOG_LEVEL_HIGH ) << "Couldn't find file format of " << path;
		return false;
	}

	if (!FreeImage_FIFSupportsReading( m_Format )) {
		PR_CLogger( PR_LOG_LEVEL_HIGH ) << "Can't load image file of type " << m_Format;
		return false;
	}

	m_Bitmap		= FreeImage_Load( m_Format, path.c_str( ) );

	m_ColorFormat	= FreeImage_GetColorType( m_Bitmap );
	m_DataType		= FreeImage_GetImageType( m_Bitmap );
	m_Width			= FreeImage_GetWidth( m_Bitmap );
	m_Height		= FreeImage_GetHeight( m_Bitmap );
	m_Components	= FreeImage_GetColorsUsed( m_Bitmap );
	m_BPP			= FreeImage_GetBPP( m_Bitmap );
	m_Bits			= FreeImage_GetBits( m_Bitmap );
}

/**	Delete
*******************************************************************************/
void PR_CImageResource::Delete( ) {
	if (!IsLoaded( ))
		return;

	FreeImage_Unload( m_Bitmap );
	m_Format = FIF_UNKNOWN;
	m_Bitmap = NULL;
	m_Width = -1;
	m_Height = -1;
	m_Bitmap = NULL;
}