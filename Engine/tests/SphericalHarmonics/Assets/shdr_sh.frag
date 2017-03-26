#version 330 core

#define PI 3.14159265359
#define TAU 6.28318530718

in vec3 f_World;
in vec3 f_Normal;

uniform vec3 u_Eye;
uniform vec3 u_SHCoeff[9];

out vec4 o_Color;

#define k01 0.2820947918 // sqrt(  1/PI)/2
#define k02 0.4886025119 // sqrt(  3/PI)/2
#define k03 1.0925484306 // sqrt( 15/PI)/2
#define k04 0.3153915652 // sqrt(  5/PI)/4
#define k05 0.5462742153 // sqrt( 15/PI)/4

float SH_0_0( in vec3 s ) { vec3 n = s.zxy; return  k01; }
float SH_1_0( in vec3 s ) { vec3 n = s.zxy; return -k02*n.y; }
float SH_1_1( in vec3 s ) { vec3 n = s.zxy; return  k02*n.z; }
float SH_1_2( in vec3 s ) { vec3 n = s.zxy; return -k02*n.x; }
float SH_2_0( in vec3 s ) { vec3 n = s.zxy; return  k03*n.x*n.y; }
float SH_2_1( in vec3 s ) { vec3 n = s.zxy; return -k03*n.y*n.z; }
float SH_2_2( in vec3 s ) { vec3 n = s.zxy; return  k04*(3.0*n.z*n.z-1.0); }
float SH_2_3( in vec3 s ) { vec3 n = s.zxy; return -k03*n.x*n.z; }
float SH_2_4( in vec3 s ) { vec3 n = s.zxy; return  k05*(n.x*n.x-n.y*n.y); }

vec3 diffuseIrradience( in vec3 dir ) {
	return 
	u_SHCoeff[0] * SH_0_0( dir ) + 
	u_SHCoeff[1] * SH_1_0( dir ) +
	u_SHCoeff[2] * SH_1_1( dir ) +
	u_SHCoeff[3] * SH_1_2( dir ) +
	u_SHCoeff[4] * SH_2_0( dir ) +
	u_SHCoeff[5] * SH_2_1( dir ) +
	u_SHCoeff[6] * SH_2_2( dir ) +
	u_SHCoeff[7] * SH_2_3( dir ) +
	u_SHCoeff[8] * SH_2_4( dir );
}

void main() {
	vec3 eyeDir = normalize(f_World - u_Eye);
	vec3 reflectedNormal = normalize(reflect(eyeDir, f_Normal));

	vec3 clr = diffuseIrradience( reflectedNormal );

	o_Color = vec4( clr, 1.0 );
}