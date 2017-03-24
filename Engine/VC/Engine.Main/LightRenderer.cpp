#include "stdafx.h"
#include "LightRenderer.h"
#include <Prism/Context.h>

/**	Constructor
*******************************************************************************/
CLightRenderer::CLightRenderer( ) :
	m_Framebuffer( NULL ), m_Output( NULL ), m_Shader( NULL ) {
}

/**	Apply to
*******************************************************************************/
PR_CTextureResource* CLightRenderer::ApplyTo( PR_CRenderScene& scene, PR_SGBuffer& gBuffer, PR_CTextureResource* shadowBuffer ) {
	LoadResources( );

	m_Shader->Set( "u_Light.direction", scene.GetLight( ).m_Direction );
	gBuffer.Position->Bind( 0 );
	gBuffer.Normal->Bind( 1 );
	gBuffer.Diffuse->Bind( 2 );
	gBuffer.Depth->Bind( 3 );
	shadowBuffer->Bind( 4 );

	m_Framebuffer->Bind( );
	glDrawArrays( GL_QUADS, 0, 4 );
	PR_CFramebufferResource::Release( );

	return m_Output;
}

/**	Load Resources
*******************************************************************************/
void CLightRenderer::LoadResources( ) {
	if (m_Framebuffer != NULL)
		return;

	m_Framebuffer = PR_CResource::Create<PR_CFramebufferResource>( );
	m_Output = PR_CResource::Create<PR_CTextureResource>( );

	m_Framebuffer->SetResolution( PR_CContext::Instance( )->GetWindowWidth( ), PR_CContext::Instance( )->GetWindowHeight( ) );
	m_Framebuffer->BindTextureColor( m_Output, 0, GL_RGBA, GL_UNSIGNED_BYTE );

	m_Shader = PR_CResource::Load<PR_CShaderResource>( "Shader/Deferred/shdr_deferred_light" );
	m_Shader->Set( "u_Deferred.position", 0 );
	m_Shader->Set( "u_Deferred.normal", 1 );
	m_Shader->Set( "u_Deferred.diffuse", 2 );
	m_Shader->Set( "u_Deferred.depth", 3 );
	m_Shader->Set( "u_Deferred.shadow", 4 );
}