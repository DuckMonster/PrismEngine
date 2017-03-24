#version 330 core

in vec2 f_UV;

struct Light {
	vec3 position;
	vec3 direction;
};

struct Deferred {
	sampler2D position;
	sampler2D normal;
	sampler2D diffuse;
	sampler2D depth;

	sampler2D shadow;
};

uniform Light u_Light;
uniform Deferred u_Deferred;

out vec4 o_Color;

void main() {
	vec3 d_world = texture(u_Deferred.position, f_UV).xyz;
	vec3 d_normal = normalize(texture(u_Deferred.normal, f_UV).xyz);
	vec3 d_diffuse = texture(u_Deferred.diffuse, f_UV).xyz;
	float d_depth = texture(u_Deferred.depth, f_UV).x;
	float d_shadow = texture(u_Deferred.shadow, f_UV).x;

	vec3 lightDir = normalize(u_Light.direction);

	// Diffuse
	float diffuse = 0.0;
	{
		diffuse = dot(d_normal, -lightDir);
	}

	diffuse *= 1.0 - d_shadow;
	
	o_Color = vec4(d_diffuse * (diffuse), 1.0);
	//o_Color = vec4(diffuse);
}