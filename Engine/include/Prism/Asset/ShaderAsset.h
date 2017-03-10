#pragma once
#include "Prism/Asset/Asset.h"
#include <GL/glew.h>
#include <glm/matrix.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace prism {
	class CShaderAsset : public CAsset {
	public:
		CShaderAsset( );
		~CShaderAsset( );

		void Use( );
		void Release( ) override;

		GLuint GetHandle( ) { return m_shaderHandle; }
		template<typename TValueType>
		bool Set( const std::string& uniform, const TValueType& value );

	protected:
		bool Load( const std::string& path ) override;
		void SetUniform( GLuint uniform, const glm::mat4& value ) { glUniformMatrix4fv( uniform, 1, false, glm::value_ptr( value ) ); }
		bool CreateShader( const std::string& vertFile, const std::string& fragFile );

	private:
		GLuint		m_shaderHandle;
	};

	template<typename TValueType>
	inline bool CShaderAsset::Set( const std::string & uniform, const TValueType& value ) {
		Use( );
		GLint uniformLocation = glGetUniformLocation( m_shaderHandle, uniform.c_str( ) );

		if (uniformLocation == -1)
			return false;

		SetUniform( uniformLocation, value );
	}
}