#pragma once
#include "Prism/Asset/Asset.h"
#include <GL/glew.h>

namespace prism {
	class CMeshAsset : public CAsset {
	public:
		CMeshAsset( );
		~CMeshAsset( );

		void Release( ) override;
		void Render( );

	private:
		bool Load( const std::string& path ) override;
		bool LoadAssimp( const char* fileName );

		GLuint			m_objectHandle;
		GLuint			m_positionHandle;
		GLuint			m_elementsHandle;
		unsigned int	m_vertexCount;
	};
}