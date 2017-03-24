#version 330 core

layout( location = 0 ) in vec3 a_Position;

uniform mat4 u_View;
uniform mat4 u_Projection;

out vec3 f_EyeDirection;

void main( ) {
	mat4 view = u_View;
	view[3] = vec4(0.0, 0.0, 0.0, 1.0);

	gl_Position = u_Projection * view * vec4(a_Position, 1.0);
	f_EyeDirection = normalize(a_Position);
}