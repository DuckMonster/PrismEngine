#pragma once
#include <Prism/Resource/Resource.h>
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

class PR_CShaderResource : public PR_CResource {
public:
	PR_CShaderResource( );
	~PR_CShaderResource( );

	void Use( );
	void Compile( const char* vertSrc, const char* fragSrc );
	void CompileFile( const std::string& vertFile, const std::string& fragFile );

	GLuint GetHandle( ) { return m_shaderHandle; }

	template<typename TValueType>
	bool Set( const std::string& uniform, const TValueType& value );

protected:
	bool Load( const std::string& path ) override;
	bool Create( ) override;
	void Delete( ) override;

	GLuint CreateShaderFromSource( GLuint shaderType, const char* src );

	void SetUniform( GLuint uniform, const glm::mat4& value ) { glUniformMatrix4fv( uniform, 1, false, glm::value_ptr( value ) ); }
	void SetUniform( GLuint uniform, const float& value ) { glUniform1fv( uniform, 1, &value ); }
	void SetUniform( GLuint uniform, const glm::vec2& value ) { glUniform2fv( uniform, 1, glm::value_ptr( value ) ); }
	void SetUniform( GLuint uniform, const glm::vec3& value ) { glUniform3fv( uniform, 1, glm::value_ptr( value ) ); }
	void SetUniform( GLuint uniform, const glm::vec4& value ) { glUniform4fv( uniform, 1, glm::value_ptr( value ) ); }

private:
	GLuint		m_shaderHandle;
};

template<typename TValueType>
inline bool PR_CShaderResource::Set( const std::string & uniform, const TValueType& value ) {
	Use( );
	GLint uniformLocation = glGetUniformLocation( m_shaderHandle, uniform.c_str( ) );


	if (uniformLocation == -1) {
		PR_CLogger( PR_LOG_LEVEL_TRIVIAL ) << "Uniform \"" << uniform << "\" doesn't exist in program " << m_shaderHandle;
		return false;
	}

	SetUniform( uniformLocation, value );
	return true;
}