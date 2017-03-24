#version 330 core

const vec2 quadVertices[4] = 
vec2[]( vec2( -1.0, -1.0 ), vec2( 1.0, -1.0 ), vec2( 1.0, 1.0 ), vec2( -1.0, 1.0 ) );

const vec2 quadUvs[4] = 
vec2[]( vec2( 0.0, 0.0 ), vec2( 1.0, 0.0 ), vec2( 1.0, 1.0 ), vec2( 0.0, 1.0 ) );

out vec2 f_UV;

void main( ) {
	gl_Position = vec4(quadVertices[gl_VertexID], 0.0, 1.0);
	f_UV = quadUvs[gl_VertexID];
}