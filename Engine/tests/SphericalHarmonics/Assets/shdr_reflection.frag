#version 330 core

#define PI 3.14159265359
#define TAU 6.28318530718

in vec3 f_World;
in vec3 f_Normal;

uniform vec3 u_Eye;
uniform sampler2D u_Sampler;

out vec4 o_Color;

vec4 sphereSkybox(vec3 direction, sampler2D sampler) {
	float phi = (atan(direction.z, direction.x) + PI) / TAU;
	float theta = direction.y * 0.5 + 0.5;
	return texture(sampler, vec2(phi, theta));
}

void main() {
	vec3 eyeDir = normalize(f_World - u_Eye);
	vec3 reflectedNormal = reflect(eyeDir, f_Normal);
	o_Color = sphereSkybox(reflectedNormal, u_Sampler);
}