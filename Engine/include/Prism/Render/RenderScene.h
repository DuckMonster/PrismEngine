#pragma once
#include <Prism/Asset/MeshAsset.h>
#include <Prism/Asset/ShaderAsset.h>
#include <glm/matrix.hpp>

namespace prism {
	class CRenderScene {
	public:
		enum NodeType {
			NODE_BASE,
			NODE_MESH
		};

		struct SNode {
			SNode( const NodeType type ) : m_Type( type ), m_Next( NULL ) {}

			NodeType		m_Type;
			SNode*			m_Next;
		};

		struct SMeshNode : public SNode {
			SMeshNode( ) : SNode( NODE_MESH ), m_Mesh( NULL ), m_Shader( NULL ) {}

			CMeshAsset*		m_Mesh;
			CShaderAsset*	m_Shader;
			glm::mat4		m_Transform;
		};

	public:
		CRenderScene( );
		~CRenderScene( );

		void Clear( );

		void AddMesh( CMeshAsset& mesh, CShaderAsset& shader, glm::mat4& transform );
		SNode* GetRoot( ) { return m_RootNode; }

		void SetCameraMatrix( const glm::mat4& cameraMatrix ) { m_CameraMatrix = cameraMatrix; }
		const glm::mat4& GetCameraMatrix( ) { return m_CameraMatrix; }

	private:
		void AddNode( SNode* node );

		glm::mat4	m_CameraMatrix;
		SNode*		m_RootNode;
		SNode*		m_LastNode;
	};
}