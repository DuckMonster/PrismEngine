#version 330 core

in vec2 f_UV;

struct Light {
	vec3 position;
	vec3 direction;
};

struct Deferred {
	sampler2D position;
	sampler2D normal;
	sampler2D depth;
};

uniform Deferred u_Deferred;
uniform Light u_Light;

out vec4 o_Color;

void main() {
	vec3 d_world = texture(u_Deferred.position, f_UV).xyz;
	vec3 d_normal = texture(u_Deferred.normal, f_UV).xyz;
	float d_depth = texture(u_Deferred.depth, f_UV).x;

	o_Color = vec4(d_world, 1.0);
}