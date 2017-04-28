#include "stdafx.h"
#include "LightRenderer.h"
#include <Prism/Context.h>

/**	Constructor
*******************************************************************************/
CLightRenderer::CLightRenderer( ) {
}

/**	Apply to
*******************************************************************************/
PR_CTextureResource* CLightRenderer::ApplyTo( PR_CRenderScene& scene, PR_SGBuffer* gBuffer, PR_CTextureResource* shadowBuffer ) {
	LoadResources( );

	m_Shader.Set( "u_Light.direction", scene.GetLight( ).m_Direction );
	gBuffer->Position.Bind( 0 );
	gBuffer->Normal.Bind( 1 );
	gBuffer->Diffuse.Bind( 2 );
	gBuffer->Depth.Bind( 3 );
	shadowBuffer->Bind( 4 );

	m_Framebuffer.Bind( );
	glDrawArrays( GL_QUADS, 0, 4 );
	PR_CFramebufferResource::Release( );

	return &m_Output;
}

/**	Load Resources
*******************************************************************************/
void CLightRenderer::LoadResources( ) {
	if (m_Framebuffer.IsValid( ) || m_Shader.IsValid( ))
		return;

	m_Framebuffer.Create( );
	m_Output.Create( );

	m_Framebuffer.BindTextureColor( m_Output, 0, GL_RGBA, GL_UNSIGNED_BYTE );

	m_Shader.CompileFiles( "Shader/Deferred/shdr_deferred_light" );
	m_Shader.Set( "u_Deferred.position", 0 );
	m_Shader.Set( "u_Deferred.normal", 1 );
	m_Shader.Set( "u_Deferred.diffuse", 2 );
	m_Shader.Set( "u_Deferred.depth", 3 );
	m_Shader.Set( "u_Deferred.shadow", 4 );
}