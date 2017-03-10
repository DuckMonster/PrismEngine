#include <Prism\Render\Renderer.h>
#include <glm/gtc/type_ptr.hpp>
using namespace prism;

/**	Constructor
*******************************************************************************/
CRenderer::CRenderer( ) {
}

/**	Render
*******************************************************************************/
void CRenderer::Render( CRenderScene& scene ) {
	CRenderScene::SNode* node = NULL;

	// Render all nodes
	for (node = scene.GetRoot( ); node; node = node->m_Next) {
		switch (node->m_Type) {
		case CRenderScene::NODE_MESH:
			{
				CRenderScene::SMeshNode* meshNode = (CRenderScene::SMeshNode*)node;

				// Setup shader
				meshNode->m_Shader->Use( );
				meshNode->m_Shader->Set( "u_Camera", scene.GetCameraMatrix( ) );
				meshNode->m_Shader->Set( "u_World", meshNode->m_Transform );

				// Render
				meshNode->m_Mesh->Render( );
			} break;
		}
	}
}