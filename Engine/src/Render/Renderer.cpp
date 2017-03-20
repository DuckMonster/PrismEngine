#include <Prism\Render\Renderer.h>
#include <glm/gtc/type_ptr.hpp>

/**	Constructor
*******************************************************************************/
PR_CRenderer::PR_CRenderer( ) {
}

/**	Render
*******************************************************************************/
void PR_CRenderer::Render( PR_CRenderScene& scene ) {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	PR_SDirLight lightSource = scene.GetLight( );

	PR_CRenderScene::SNode* node = NULL;

	// Render all nodes
	for (node = scene.GetRoot( ); node; node = node->m_Next) {
		switch (node->m_Type) {
		case PR_CRenderScene::NODE_MESH:
			{
				PR_CRenderScene::SMeshNode* meshNode = (PR_CRenderScene::SMeshNode*)node;

				// Setup shader
				PR_CShaderResource* shader = meshNode->m_Material->GetShader( );

				shader->Use( );
				shader->Set( "u_Camera", scene.GetCameraMatrix( ) );
				shader->Set( "u_World", meshNode->m_Transform );
				shader->Set( "u_Material.color", meshNode->m_Material->GetColor( ) );

				// Upload light
				shader->Set( "u_Light.position", lightSource.m_Position );
				shader->Set( "u_Light.direction", lightSource.m_Direction );

				// Render
				meshNode->m_Mesh->Render( );
			} break;
		}
	}
}