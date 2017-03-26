#pragma once
#include <Prism/Resource/Resource.h>
#include <FreeImage/FreeImage.h>
#include <GL/glew.h>

class PR_CImageResource : public PR_CResource {
private:
	static void InitFreeImage( );
	static void ReleaseFreeImage( );
	static bool sm_FreeImageInitialized;

public:
	PR_CImageResource( );

	const int GetWidth( ) { return m_Width; }
	const int GetHeight( ) { return m_Height; }
	const unsigned char* GetBits( ) { return m_Bits; }

	const unsigned char* GetPixel( float u, float v );
	const unsigned char* GetPixel( int x, int y );

	const GLuint GetGLDataFormat( );
	const GLuint GetGLDataType( );

	void Delete( ) override;

private:
	bool Load( const std::string& path ) override;
	bool Create( ) override;

	FREE_IMAGE_FORMAT m_Format;
	FREE_IMAGE_COLOR_TYPE m_ColorFormat;
	FREE_IMAGE_TYPE m_DataType;
	FIBITMAP*		m_Bitmap;
	int				m_Width;
	int				m_Height;
	size_t			m_Components;
	int				m_BPP;
	unsigned char*	m_Bits;
};