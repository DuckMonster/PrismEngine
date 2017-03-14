#include <Prism/Resource/ShaderResource.h>
#include <Prism/Utils/Log.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

namespace {
	/**	Read File
	*******************************************************************************/
	std::string ReadFile( const std::string& fileName ) {
		static char s_fileBuffer[1 << 16];

		// Open file
		ifstream fileStream( fileName.c_str( ), ifstream::binary );
		if (!fileStream.is_open( ))
			return false;

		fileStream.seekg( 0, ios::end );
		size_t fileLength = fileStream.tellg( );
		fileStream.seekg( 0, ios::beg );

		// Read all and add terminator
		fileStream.read( s_fileBuffer, fileLength );
		s_fileBuffer[fileLength] = '\0';

		return std::string( s_fileBuffer );
	}
}

/**	Constructor
*******************************************************************************/
PR_CShaderResource::PR_CShaderResource( ) :
	PR_CResource( ),
	m_shaderHandle( -1 ) {
}

/**	Destructor
*******************************************************************************/
PR_CShaderResource::~PR_CShaderResource( ) {
}

/**	Use
*******************************************************************************/
void PR_CShaderResource::Use( ) {
	PR_ASSERT_MSG( m_shaderHandle != -1, "Trying to use an uninitialized shader" );
	glUseProgram( m_shaderHandle );
}

/**	Compile
*******************************************************************************/
void PR_CShaderResource::Compile( const char* vertSrc, const char* fragSrc ) {
	static char s_logBuffer[256];

	PR_ASSERT_MSG( m_shaderHandle != -1, "Shader isn't created" );

	// Create pipeline
	GLuint vertShader, fragShader;
	vertShader = CreateShaderFromSource( GL_VERTEX_SHADER, vertSrc );
	fragShader = CreateShaderFromSource( GL_FRAGMENT_SHADER, fragSrc );

	// Link program
	glAttachShader( m_shaderHandle, vertShader );
	glAttachShader( m_shaderHandle, fragShader );
	glLinkProgram( m_shaderHandle );

	// Get errors
	glGetProgramInfoLog( m_shaderHandle, 1 << 16, NULL, s_logBuffer );
	if (strlen( s_logBuffer ) > 0) {
		cout << "SHADER ERROR:\n"
			<< s_logBuffer << "\n---------\n\n";
	}

	// Clean up shaders
	glDetachShader( m_shaderHandle, vertShader );
	glDetachShader( m_shaderHandle, fragShader );
	glDeleteShader( vertShader );
	glDeleteShader( fragShader );
}

/**	Compile file
*******************************************************************************/
void PR_CShaderResource::CompileFile( const std::string& vertPath, const std::string& fragPath ) {
	std::string vertSrc = ReadFile( vertPath ),
		fragSrc = ReadFile( fragPath );

	Compile( vertSrc.c_str( ), fragSrc.c_str( ) );
}

/**	Load
*******************************************************************************/
bool PR_CShaderResource::Load( const std::string& path ) {
	string vertFile( path ), fragFile( path );
	vertFile += ".vert";
	fragFile += ".frag";

	Create( );
	CompileFile( vertFile, fragFile );

	return true;
}

/**	Create
*******************************************************************************/
bool PR_CShaderResource::Create( ) {
	m_shaderHandle = glCreateProgram( );
	return true;
}

/**	Release
*******************************************************************************/
void PR_CShaderResource::Delete( ) {
	// Already released
	if (m_shaderHandle == -1)
		return;

	glDeleteProgram( m_shaderHandle );
	m_shaderHandle = -1;
}

/**	Create shader from source
*******************************************************************************/
GLuint PR_CShaderResource::CreateShaderFromSource( GLuint shaderType, const char* src ) {
	GLuint shader = glCreateShader( shaderType );

	glShaderSource( shader, 1, &src, NULL );
	glCompileShader( shader );

	return shader;
}