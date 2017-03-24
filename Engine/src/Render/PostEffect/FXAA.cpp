#include <Prism\Render\PostEffect\FXAA.h>
#include <Prism\Context.h>

namespace {
	const char* SRC_VERT = "\
#version 330 core\n\
\
const vec2 quadVertices[4] =\
	vec2[]( vec2( -1.0, -1.0 ), vec2( 1.0, -1.0 ), vec2( 1.0, 1.0 ), vec2( -1.0, 1.0 ) );\
\
const vec2 quadUvs[4] =\
	vec2[]( vec2( 0.0, 0.0 ), vec2( 1.0, 0.0 ), vec2( 1.0, 1.0 ), vec2( 0.0, 1.0 ) );\
\
out vec2 f_UV;\
\
void main( ) {\
	gl_Position = vec4( quadVertices[gl_VertexID], 0.0, 1.0 );\
	f_UV = quadUvs[gl_VertexID];\
}\
";

	const char* SRC_FRAG = "\
#version 330 core\n\
\
in vec2 f_UV;\
uniform sampler2D u_Sampler;\
\
out vec4 o_Color;\
\
/* Basic FXAA implementation based on the code on geeks3d.com with the\
modification that the texture2DLod stuff was removed since it's\
unsupported by WebGL. */\
\
\n#define FXAA_REDUCE_MIN   (1.0/ 128.0)\n\
\n#define FXAA_REDUCE_MUL   (1.0 / 8.0)\n\
\n#define FXAA_SPAN_MAX     8.0\n\
\
vec4 applyFXAA( vec2 fragCoord, sampler2D tex ) {\
	vec4 color;\
	vec2 inverseVP = textureSize( tex, 0 );\
	inverseVP = vec2( 1.0 / inverseVP.x, 1.0 / inverseVP.y );\
	vec3 rgbNW = texture2D( tex, fragCoord + (vec2( -1.0, -1.0 ) * inverseVP) ).xyz;\
	vec3 rgbNE = texture2D( tex, fragCoord + (vec2( 1.0, -1.0 ) * inverseVP) ).xyz;\
	vec3 rgbSW = texture2D( tex, fragCoord + (vec2( -1.0, 1.0 ) * inverseVP) ).xyz;\
	vec3 rgbSE = texture2D( tex, fragCoord + (vec2( 1.0, 1.0 ) * inverseVP) ).xyz;\
	vec3 rgbM  = texture2D( tex, fragCoord ).xyz;\
	vec3 luma = vec3( 0.299, 0.587, 0.114 );\
	float lumaNW = dot( rgbNW, luma );\
	float lumaNE = dot( rgbNE, luma );\
	float lumaSW = dot( rgbSW, luma );\
	float lumaSE = dot( rgbSE, luma );\
	float lumaM  = dot( rgbM, luma );\
	float lumaMin = min( lumaM, min( min( lumaNW, lumaNE ), min( lumaSW, lumaSE ) ) );\
	float lumaMax = max( lumaM, max( max( lumaNW, lumaNE ), max( lumaSW, lumaSE ) ) );\
\
	vec2 dir;\
	dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));\
	dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));\
\
	float dirReduce = max( (lumaNW + lumaNE + lumaSW + lumaSE) *\
		(0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN );\
\
	float rcpDirMin = 1.0 / (min( abs( dir.x ), abs( dir.y ) ) + dirReduce);\
	dir = min( vec2( FXAA_SPAN_MAX, FXAA_SPAN_MAX ),\
		max( vec2( -FXAA_SPAN_MAX, -FXAA_SPAN_MAX ),\
			dir * rcpDirMin ) ) * inverseVP;\
\
	vec3 rgbA = 0.5 * (\
		texture2D( tex, fragCoord + dir * (1.0 / 3.0 - 0.5) ).xyz +\
		texture2D( tex, fragCoord + dir * (2.0 / 3.0 - 0.5) ).xyz);\
	vec3 rgbB = rgbA * 0.5 + 0.25 * (\
		texture2D( tex, fragCoord + dir * -0.5 ).xyz +\
		texture2D( tex, fragCoord + dir * 0.5 ).xyz);\
\
	float lumaB = dot( rgbB, luma );\
	if ((lumaB < lumaMin) || (lumaB > lumaMax))\
		color = vec4( rgbA, 1.0 );\
	else\
		color = vec4( rgbB, 1.0 );\
	return color;\
}\
\
void main( ) {\
	o_Color = applyFXAA( f_UV, u_Sampler );\
}\
";
}


/**	Constructor
*******************************************************************************/
PR_CFXAA::PR_CFXAA( ) :
	m_Framebuffer( NULL ), m_Output( NULL ), m_Shader( NULL ) {
}

/**	Apply To
*******************************************************************************/
PR_CTextureResource * PR_CFXAA::ApplyTo( PR_CTextureResource * texture ) {
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
void PR_CFXAA::LoadResources( ) {
	if (m_Framebuffer != NULL)
		return;

	m_Framebuffer = PR_CResource::Create<PR_CFramebufferResource>( );
	m_Output = PR_CResource::Create<PR_CTextureResource>( );

	m_Framebuffer->SetResolution( PR_CContext::Instance( )->GetWindowWidth( ), PR_CContext::Instance( )->GetWindowHeight( ) );
	m_Framebuffer->BindTextureColor( m_Output, 0, GL_RGBA, GL_UNSIGNED_BYTE );

	m_Shader = PR_CResource::Create<PR_CShaderResource>( );
	m_Shader->Compile( SRC_VERT, SRC_FRAG );
}