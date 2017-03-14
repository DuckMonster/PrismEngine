#version 330 core

in VS_OUT {
	vec3 normal;
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
	vec3 light_dir = normalize(u_Light.direction);
	vec3 norm = normalize(fs_in.normal);

	float diffuse;

	// Diffuse
	{
		float value = max(dot(-light_dir, norm), 0.0);
		diffuse = value;
	}

	o_Color = vec4(u_Material.color.xyz * diffuse, u_Material.color.w);
}