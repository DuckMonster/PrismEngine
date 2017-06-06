#include <Prism\Utils\RenderUtils.h>
#include <Prism\Resource\ShaderResource.h>
#include <Prism\Utils\DefaultShader.h>

/**	Render Texture
*******************************************************************************/
void PR_RenderTexture( const PR_CTextureResource& texture ) {
	GLboolean isDepthTesting;
	glGetBooleanv( GL_DEPTH_TEST, &isDepthTesting );

	glDisable( GL_DEPTH_TEST );

	PR_QuadShader( ).Use( );
	PR_QuadShader( ).Set( "u_QuadMatrix", glm::mat4( 1.f ) );
	texture.Bind( 0 );

	glDrawArrays( GL_QUADS, 0, 4 );

	// Restore depth testing
	if (isDepthTesting)
		glEnable( GL_DEPTH_TEST );
}
