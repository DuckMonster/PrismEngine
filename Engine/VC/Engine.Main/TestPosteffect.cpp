#include "stdafx.h"
#include "TestPosteffect.h"
#include <Prism/Context.h>

/**	Constructor
*******************************************************************************/
CTestPosteffect::CTestPosteffect( ) {
}

/**	Apply To
*******************************************************************************/
PR_CTextureResource* CTestPosteffect::ApplyTo( PR_CTextureResource* texture ) {
	LoadResources( );

	m_Shader.Use( );
	m_Framebuffer.Bind( );
	texture->Bind( 0 );

	glDrawArrays( GL_QUADS, 0, 4 );

	PR_CFramebufferResource::Release( );
	return &m_Output;
}

/**	Load Resources
*******************************************************************************/
void CTestPosteffect::LoadResources( ) {
	if (m_Framebuffer.IsValid( ))
		return;

	m_Framebuffer.Create( );
	m_Output.Create( );

	m_Framebuffer.BindTextureColor( m_Output, 0, GL_RGBA, GL_UNSIGNED_BYTE );

	m_Shader.CompileFiles( "Shader/Post/shdr_post_test" );
}
