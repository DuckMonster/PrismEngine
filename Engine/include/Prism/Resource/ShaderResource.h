#pragma once
#include <vector>
#include <Prism/Resource/Resource.h>
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

class PR_CShaderResource : public PR_CResource {
public:
	PR_CShaderResource( );
	~PR_CShaderResource( );

	const bool IsValid( ) { return m_shaderHandle != -1; }

	void Use( );
	void CompileSource( const char* vertSrc, const char* fragSrc );
	void CompileFiles( const std::string& fileName );
	void CompileFiles( const std::string& vertFile, const std::string& fragFile );

	GLuint GetHandle( ) { return m_shaderHandle; }

	template<typename TValueType>
	bool Set( const std::string& uniform, const TValueType& value );
	template<typename TValueType>
	bool Set( const std::string& uniform, const TValueType* ptr, size_t count );

	void Delete( );

protected:
	GLuint CreateShaderFromSource( GLuint shaderType, const char* src );

	void SetUniform( GLuint uniform, const glm::mat4* value, size_t count ) { glUniformMatrix4fv( uniform, count, false, glm::value_ptr( *value ) ); }
	void SetUniform( GLuint uniform, const int* value, size_t count ) { glUniform1iv( uniform, count, value ); }
	void SetUniform( GLuint uniform, const float* value, size_t count ) { glUniform1fv( uniform, count, value ); }
	void SetUniform( GLuint uniform, const glm::vec2* value, size_t count ) { glUniform2fv( uniform, count, glm::value_ptr( *value ) ); }
	void SetUniform( GLuint uniform, const glm::vec3* value, size_t count ) { glUniform3fv( uniform, count, glm::value_ptr( *value ) ); }
	void SetUniform( GLuint uniform, const glm::vec4* value, size_t count ) { glUniform4fv( uniform, count, glm::value_ptr( *value ) ); }

private:
	GLuint						m_shaderHandle;
	std::vector<std::string>	m_UniformErrors;
};

template<typename TValueType>
inline bool PR_CShaderResource::Set( const std::string& uniform, const TValueType& value ) {
	Use( );
	GLint uniformLocation = glGetUniformLocation( m_shaderHandle, uniform.c_str( ) );

	if (uniformLocation == -1) {
		// If this uniform has not been used before, add it to the list and display an error
		if (std::find( m_UniformErrors.begin( ), m_UniformErrors.end( ), uniform ) == m_UniformErrors.end( )) {
			PR_CLogger( PR_LOG_LEVEL_TRIVIAL ) << "Uniform \"" << uniform << "\" doesn't exist in program " << m_shaderHandle;
			m_UniformErrors.push_back( uniform );
		}

		return false;
	}

	SetUniform( uniformLocation, &value, 1 );
	return true;
}

template<typename TValueType>
inline bool PR_CShaderResource::Set( const std::string & uniform, const TValueType* ptr, size_t count ) {
	Use( );
	GLint uniformLocation = glGetUniformLocation( m_shaderHandle, uniform.c_str( ) );

	if (uniformLocation == -1) {
		// If this uniform has not been used before, add it to the list and display an error
		if (std::find( m_UniformErrors.begin( ), m_UniformErrors.end( ), uniform ) == m_UniformErrors.end( )) {
			PR_CLogger( PR_LOG_LEVEL_TRIVIAL ) << "Uniform \"" << uniform << "\" doesn't exist in program " << m_shaderHandle;
			m_UniformErrors.push_back( uniform );
		}

		return false;
	}

	SetUniform( uniformLocation, ptr, count );
	return true;
}
