#include "stdafx.h"
#include "TestPosteffect.h"
#include <Prism/Context.h>

/**	Constructor
*******************************************************************************/
CTestPosteffect::CTestPosteffect( ) :
	m_Framebuffer( NULL ), m_Output( NULL ), m_Shader( NULL ) {
}

/**	Apply To
*******************************************************************************/
PR_CTextureResource * CTestPosteffect::ApplyTo( PR_CTextureResource* texture ) {
	LoadResources( );

	m_Shader->Use( );
	m_Framebuffer->Bind( );
	texture->Bind( 0 );

	glDrawArrays( GL_QUADS, 0, 4 );

	PR_CFramebufferResource::Release( );
	return m_Output;
}

/**	Load Resources
*******************************************************************************/
void CTestPosteffect::LoadResources( ) {
	if (m_Framebuffer != NULL)
		return;

	m_Framebuffer = PR_CResource::Create<PR_CFramebufferResource>( );
	m_Output = PR_CResource::Create<PR_CTextureResource>( );

	m_Framebuffer->SetResolution( PR_CContext::Instance( )->GetWindowWidth( ), PR_CContext::Instance( )->GetWindowHeight( ) );
	m_Framebuffer->BindTextureColor( m_Output, 0, GL_RGBA, GL_UNSIGNED_BYTE );

	m_Shader = PR_CResource::Load<PR_CShaderResource>( "Shader/Post/shdr_post_test" );
}
