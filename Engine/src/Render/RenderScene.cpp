#include <Prism\Render\RenderScene.h>

/**	Constructor
*******************************************************************************/
PR_CRenderScene::PR_CRenderScene( ) :
	m_CameraMatrix( 1.f ),
	m_RootNode( NULL ), m_LastNode( NULL ) {
}

/**	Destructor
*******************************************************************************/
PR_CRenderScene::~PR_CRenderScene( ) {
	Clear( );
}

/**	Clear
*******************************************************************************/
void PR_CRenderScene::Clear( ) {
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
void PR_CRenderScene::AddMesh( PR_CMeshResource* mesh, PR_CMaterial* material, glm::mat4& transform ) {
	SMeshNode* meshNode = new SMeshNode;

	meshNode->m_Mesh		= mesh;
	meshNode->m_Material	= material;
	meshNode->m_Transform	= transform;

	AddNode( meshNode );
}

/**	Add Node
*******************************************************************************/
void PR_CRenderScene::AddNode( SNode * node ) {
	// This is the first node
	if (m_RootNode == NULL) {
		m_RootNode = node;
	}

	// Add it to the list
	else {
		m_LastNode->m_Next = node;
	}

	m_LastNode = node;
}