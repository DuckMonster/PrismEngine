#pragma once
#include <Prism/Asset/MeshAsset.h>
#include <Prism/Render/Material.h>
#include <Prism/Utils/DirLight.h>
#include <glm/matrix.hpp>

/**	CRenderScene
*******************************************************************************/
class PR_CRenderScene {
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
		SMeshNode( ) : SNode( NODE_MESH ), m_Mesh( NULL ), m_Material( NULL ) {}

		PR_CMeshAsset*	m_Mesh;
		PR_CMaterial*	m_Material;
		glm::mat4		m_Transform;
	};

public:
	PR_CRenderScene( );
	~PR_CRenderScene( );

	void Clear( );

	void AddMesh( PR_CMeshAsset* mesh, PR_CMaterial* material, glm::mat4& transform );
	SNode* GetRoot( ) { return m_RootNode; }

	void SetCameraMatrix( const glm::mat4& cameraMatrix ) { m_CameraMatrix = cameraMatrix; }
	const glm::mat4& GetCameraMatrix( ) { return m_CameraMatrix; }

	void SetLight( const PR_SDirLight& light ) { m_Light = light; }
	const PR_SDirLight& GetLight( ) { return m_Light; }

private:
	void AddNode( SNode* node );

	glm::mat4		m_CameraMatrix;
	PR_SDirLight	m_Light;
	SNode*			m_RootNode;
	SNode*			m_LastNode;
};