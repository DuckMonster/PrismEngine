#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

uniform mat4 u_Camera;
uniform mat4 u_World;

out VS_OUT {
	vec3 world;
	vec3 normal;
	vec2 uv;
} vs_out;

void main() {
	gl_Position = u_Camera * u_World * vec4(a_Position, 1.0);

	vs_out.world = (u_World * vec4(a_Position, 1.0)).xyz;
	vs_out.normal = (u_World * vec4(a_Normal, 0.0)).xyz;
	vs_out.uv = a_UV;
}