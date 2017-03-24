#version 330 core

in VS_OUT {
	vec3 world;
	vec3 normal;
	vec2 uv;
} fs_in;

struct Material {
	vec4 color;
};

struct Light {
	vec3 position;
	vec3 direction;
};

uniform Material u_Material;
uniform Light u_Light;

layout(location = 0) out vec3 o_Color;
layout(location = 1) out vec3 o_Normal;

void main() {
	o_Color = fs_in.world;
	o_Normal = fs_in.normal;
}