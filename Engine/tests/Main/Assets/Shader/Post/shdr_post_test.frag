#version 330 core

in vec2 f_UV;
uniform sampler2D u_Sampler;

out vec4 o_Color;

void main() {
	vec4 fragColor = texture(u_Sampler, f_UV);
	float luma = fragColor.x * 0.2126 + fragColor.y * 0.7152 + fragColor.b * 0.0722;
	o_Color = vec4(vec3(luma), 1.0);
}