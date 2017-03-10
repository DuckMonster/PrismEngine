#include "Prism/Asset/ShaderAsset.h"
#include "Prism/Utils/Log.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace prism;
using namespace std;

/**	Constructor
*******************************************************************************/
CShaderAsset::CShaderAsset( ) :
	CAsset( ),
	m_shaderHandle( -1 ) {
}

/**	Destructor
*******************************************************************************/
CShaderAsset::~CShaderAsset( ) {
}

/**	Load
*******************************************************************************/
//bool CShaderAsset::Load( const char* assetName ) {
//	string vert( assetName ), frag( assetName );
//	vert += ".vert";
//	frag += ".frag";
//
//	return Load( vert.c_str( ), frag.c_str( ) );
//}
//
/**	Load
*******************************************************************************/
//bool CShaderAsset::Load( const char* vertName, const char* fragName ) {
//	string vert( GetAssetDirectory( ) ), frag( GetAssetDirectory( ) );
//	vert += vertName;
//	frag += fragName;
//
//	if (CreateShader( vert, frag )) {
//		std::cout << "Successfully loaded shader { " << vertName << ", " << fragName << " }\n";
//		return true;
//	}
//	else {
//		std::cout << "Failed to load shader { " << vertName << ", " << fragName << " }\n";
//		return false;
//	}
//}

/**	Use
*******************************************************************************/
void prism::CShaderAsset::Use( ) {
	PRISM_ASSERT_MSG( m_shaderHandle != -1, "Trying to use an uninitialized shader" );
	glUseProgram( m_shaderHandle );
}

/**	Release
*******************************************************************************/
void prism::CShaderAsset::Release( ) {
	// Already released
	if (m_shaderHandle == -1)
		return;

	glDeleteProgram( m_shaderHandle );
	m_shaderHandle = -1;
}

/**	Set uniform
*******************************************************************************/
//bool prism::CShaderAsset::Set( const std::string& uniform, const glm::mat4 & value ) {
//	Use( );
//	GLint uniformLocation = glGetUniformLocation( m_shaderHandle, uniform.c_str( ) );
//
//	if (uniformLocation == -1)
//		return false;
//
//	glUniformMatrix4fv(  )
//}

/**	Load
*******************************************************************************/
bool prism::CShaderAsset::Load( const std::string& path ) {
	string vertFile( path ), fragFile( path );
	vertFile += ".vert";
	fragFile += ".frag";

	return CreateShader( vertFile, fragFile );
}

/**	Create shader
*******************************************************************************/
bool CShaderAsset::CreateShader( const string& vertFile, const string& fragFile ) {
	static char s_fileBuffer[1 << 16];
	static char* s_filePtr = &s_fileBuffer[0];

	GLuint vertShader, fragShader;

	m_shaderHandle = glCreateProgram( );
	vertShader = glCreateShader( GL_VERTEX_SHADER );
	fragShader = glCreateShader( GL_FRAGMENT_SHADER );

	// Vertex shader
	{
		ifstream fileStream( vertFile.c_str( ), ifstream::binary );
		if (!fileStream.is_open( ))
			return false;

		fileStream.seekg( 0, ios::end );
		size_t fileLength = fileStream.tellg( );
		fileStream.seekg( 0, ios::beg );

		// Read all and add terminator
		fileStream.read( s_fileBuffer, fileLength );
		s_fileBuffer[fileLength] = '\0';

		glShaderSource( vertShader, 1, &s_filePtr, NULL );
		glCompileShader( vertShader );
	}

	// Fragment shader
	{
		ifstream fileStream( fragFile.c_str( ), ifstream::binary );
		if (!fileStream.is_open( ))
			return false;

		fileStream.seekg( 0, ios::end );
		size_t fileLength = fileStream.tellg( );
		fileStream.seekg( 0, ios::beg );

		// Read all and add terminator
		fileStream.read( s_fileBuffer, fileLength );
		s_fileBuffer[fileLength] = '\0';

		glShaderSource( fragShader, 1, &s_filePtr, NULL );
		glCompileShader( fragShader );
	}

	// Link program
	glAttachShader( m_shaderHandle, vertShader );
	glAttachShader( m_shaderHandle, fragShader );
	glLinkProgram( m_shaderHandle );

	// Reset file buffer and get error log
	s_fileBuffer[0] = '\0';

	glGetProgramInfoLog( m_shaderHandle, 1 << 16, NULL, s_fileBuffer );
	if (strlen( s_fileBuffer ) > 0) {
		cout << "SHADER ERROR:\n"
			<< s_fileBuffer << "\n---------\n\n";
	}

	// Clean up shaders
	glDetachShader( m_shaderHandle, vertShader );
	glDetachShader( m_shaderHandle, fragShader );
	glDeleteShader( vertShader );
	glDeleteShader( fragShader );

	return true;
}
