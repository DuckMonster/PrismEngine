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

#define c1 0.429043
#define c2 0.511664
#define c3 0.743125
#define c4 0.886227
#define c5 0.247708

float SH_0_0( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return  c1; }
float SH_1_0( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return -c2*n.y; }
float SH_1_1( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return  c2*n.z; }
float SH_1_2( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return -c2*n.x; }
float SH_2_0( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return  c3*n.x*n.y; }
float SH_2_1( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return -c3*n.y*n.z; }
float SH_2_2( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return  c4*(3.0*n.z*n.z-1.0); }
float SH_2_3( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return -c3*n.x*n.z; }
float SH_2_4( in vec3 s ) { vec3 n = s.xzy; n.x = -n.x; return  c5*(n.x*n.x-n.y*n.y); }

vec3 calcIrradiance( vec3 inNormal ){
	// convert y-axis to z-axis and rotation! (rotating normal instead of SH coefficients)
	vec3 squaredNormal = inNormal * inNormal;
                
	return
		c1 * u_SHCoeff[8] * (squaredNormal.x - squaredNormal.z)
		+ c3 * u_SHCoeff[6] * squaredNormal.y
		+ c4 * u_SHCoeff[0]
		- c5 * u_SHCoeff[6]
		+ 2.0*c1 * (u_SHCoeff[4]*-inNormal.x*-inNormal.z + u_SHCoeff[7]*-inNormal.x*inNormal.y + u_SHCoeff[5]*-inNormal.z*inNormal.y )
		+ 2.0*c2 * (u_SHCoeff[3]*-inNormal.x + u_SHCoeff[1]*-inNormal.z + u_SHCoeff[2]*inNormal.y);
}

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
	vec3 eyeDir = normalize(f_Normal);
	//vec3 reflectedNormal = normalize(reflect(eyeDir, f_Normal));

	vec3 clr = diffuseIrradience( f_Normal );
	clr = 0.7 * calcIrradiance(f_Normal);

	o_Color = vec4( clr, 1.0 );
}