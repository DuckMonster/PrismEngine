#version 330 core

#define PI 3.14159265359
#define TAU 6.28318530718

in vec3 f_EyeDirection;
uniform sampler2D u_Sampler;

out vec4 o_Color;

vec4 sphereSkybox(vec3 direction, sampler2D sampler) {
	float phi = (atan(direction.z, direction.x)) / TAU;
	float theta = direction.y * 0.5 + 0.5;
	return texture(sampler, vec2(phi, theta));
}

void main() {
	o_Color = sphereSkybox(f_EyeDirection, u_Sampler);
}