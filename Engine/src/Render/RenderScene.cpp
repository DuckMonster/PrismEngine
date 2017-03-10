#include <Prism\Render\RenderScene.h>
using namespace prism;

/**	Constructor
*******************************************************************************/
CRenderScene::CRenderScene( ) :
	m_CameraMatrix( 1.f ),
	m_RootNode( NULL ), m_LastNode( NULL ) {
}

/**	Destructor
*******************************************************************************/
CRenderScene::~CRenderScene( ) {
	Clear( );
}

/**	Clear
*******************************************************************************/
void CRenderScene::Clear( ) {
	SNode* ptr = m_RootNode;
	while (ptr) {
		SNode* next = ptr->m_Next;
		delete ptr;

		ptr = next;
	}

	m_RootNode = NULL;
	m_LastNode = NULL;
}

/**	Add Mesh
*******************************************************************************/
void CRenderScene::AddMesh( CMeshAsset& mesh, CShaderAsset& shader, glm::mat4& transform ) {
	SMeshNode* meshNode = new SMeshNode;

	meshNode->m_Mesh		= &mesh;
	meshNode->m_Shader		= &shader;
	meshNode->m_Transform	= transform;

	AddNode( meshNode );
}

/**	Add Node
*******************************************************************************/
void CRenderScene::AddNode( SNode * node ) {
	// This is the first node
	if (m_RootNode==NULL) {
		m_RootNode = node;
	}

	// Add it to the list
	else {
		m_LastNode->m_Next = node;
	}

	m_LastNode = node;
}