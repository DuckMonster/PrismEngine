#include <Prism\Utils\RenderUtils.h>
#include <Prism\Resource\ShaderResource.h>
#include <Prism\Utils\DefaultShader.h>

/**	Render Texture
*******************************************************************************/
void PR_RenderTexture( PR_CTextureResource* texture ) {
	PR_QuadShader( )->Use( );
	PR_QuadShader( )->Set( "u_QuadMatrix", glm::mat4( 1.f ) );
	texture->Bind( 0 );

	glDrawArrays( GL_QUADS, 0, 4 );
}
