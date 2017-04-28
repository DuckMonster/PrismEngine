#include <Prism\Utils\DefaultShader.h>

namespace {
	const char* QUAD_SHADER_VERT = ""
		"#version 330 core\n"
		"const vec2 quadVertices[4] = "
		"vec2[]( vec2( -1.0, -1.0 ), vec2( 1.0, -1.0 ), vec2( 1.0, 1.0 ), vec2( -1.0, 1.0 ) );"

		"const vec2 quadUvs[4] = "
		"vec2[]( vec2( 0.0, 0.0 ), vec2( 1.0, 0.0 ), vec2( 1.0, 1.0 ), vec2( 0.0, 1.0 ) );"

		"uniform mat4 u_QuadMatrix;"

		"out vec2 f_UV;"

		"void main( ) {"
		"	gl_Position = u_QuadMatrix * vec4( quadVertices[gl_VertexID], 0.0, 1.0 );"
		"	f_UV = quadUvs[gl_VertexID];"
		"}";

	const char* QUAD_SHADER_FRAG = ""
		"#version 330 core\n"
		"uniform sampler2D u_Sampler;"

		"in vec2 f_UV;"
		"out vec4 o_Color;"
		"void main() {"
		"	o_Color = texture(u_Sampler, f_UV);"
		"}";
}

PR_CShaderResource& PR_QuadShader( ) {
	static PR_CShaderResource SHADER;

	// Compile
	if (!SHADER.IsValid( )) {
		SHADER.CompileSource( QUAD_SHADER_VERT, QUAD_SHADER_FRAG );
	}

	return SHADER;
}