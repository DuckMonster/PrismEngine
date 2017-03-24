#version 330 core

#define PI 3.14159265359
#define TAU 6.28318530718

const mat4 BIAS_MAT = mat4(
2.0, 0.0, 0.0, 0.0,
0.0, 2.0, 0.0, 0.0,
0.0, 0.0, 1.0, 0.0,
-1.0, -1.0, 0.0, 1.0
);

layout( location = 0 ) in vec2 a_Position;
layout( location = 1 ) in vec3 a_Color;

uniform mat4 u_Camera;

out f_Color;

void main( ) {
	vec2 sphere = a_Position;

	vec3 world = vec3(
		cos(sphere.y) * sin(sphere.x),
		cos(sphere.x),
		sin(sphere.y) * sin(sphere.x)
	);

	gl_Position = u_Camera * vec4(world, 1.0);
	f_Color = a_Color;
	//gl_Position = BIAS_MAT * vec4(sphere, 0.0, 1.0);
	//gl_Position = vec4(a_Position, 0.0, 1.0);
}