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

out vec4 o_Color;

void main() {
	o_Color = vec4(fs_in.world, 1.0);
}