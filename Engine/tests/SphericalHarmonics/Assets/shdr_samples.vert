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
//layout( location = 1 ) in vec3 a_Color;
layout( location = 2 ) in vec3 a_Coeff0;
layout( location = 3 ) in vec3 a_Coeff1;
layout( location = 4 ) in vec3 a_Coeff2;

uniform mat4 u_Camera;
uniform int u_CoeffIndex;

out vec3 f_Color;

void main( ) {
	vec2 sphere = a_Position;
	float coeff = 0.0;

	if (u_CoeffIndex < 3)
		coeff = a_Coeff0[u_CoeffIndex];
	else if (u_CoeffIndex < 6)
		coeff = a_Coeff1[u_CoeffIndex - 3];
	else
		coeff = a_Coeff2[u_CoeffIndex - 6];

	vec3 world = vec3(
		cos(sphere.y) * sin(sphere.x),
		cos(sphere.x),
		sin(sphere.y) * sin(sphere.x)
	) * abs(coeff);

	vec3 clr = vec3(1.0, 0.0, 0.0);

	if (coeff > 0.0)
		clr = vec3(0.0, 1.0, 0.0);

	gl_Position = u_Camera * vec4(world, 1.0);
	f_Color = clr;
	//f_Color = a_Color;
	//gl_Position = BIAS_MAT * vec4(sphere, 0.0, 1.0);
	//gl_Position = vec4(a_Position, 0.0, 1.0);
}