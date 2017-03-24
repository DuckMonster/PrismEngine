#include "SH.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/integer.hpp>

using namespace glm;

namespace {
	/**	Fast Factorial
	*******************************************************************************/
	double FastFactorial( int n ) {
		static const double FACTORIALS[] ={
			1.0, 2.0, 6.0, 24.0, 120.0, 720.0, 5040.0, 40320.0, 362880.0, 3628800.0,
			479001600.0, 6227020800.0, 87178291200.0, 1307674368000.0, 20922789888000.0,
			355687428096000.0, 6402373705728000.0, 121645100408832000.0, 2432902008176640000.0,
			51090942171709440000.0, 1124000727777607680000.0, 25852016738884976640000.0,
			620448401733239439360000.0, 15511210043330985984000000.0, 403291461126605635584000000.0,
			10888869450418352160768000000.0, 304888344611713860501504000000.0, 8841761993739701954543616000000.0,
			265252859812191058636308480000000.0, 8222838654177922817725562880000000.0, 263130836933693530167218012160000000.0,
			8683317618811886495518194401280000000.0
		};

		if (n <= 33)
			return FACTORIALS[n];
		else
			return (double)n * FastFactorial( n - 1 );
	}

	// Associated Legendre Polynomial P(l, m, x) at x
	double P( int l, int m, double x ) {
		double pmm = 1.0;
		if (m > 0) {
			double somx2 = sqrt( (1.0 - x) * (1.0 + x) );

			double fact = 1.0;
			for (int i = 0; i <= m; i++) {
				pmm *= (-fact) * somx2;
				fact += 2.0;
			}
		}

		if (l == m) return pmm;

		double pmmp1 = x * (2.0 * m + 1.0) * pmm;
		if (l == m + 1) return pmmp1;

		double pll = 0.0;
		for (int ll=m + 2; ll <= l; ++ll) {
			pll = ((2.0 * ll - 1.0)*x*pmmp1 - (ll + m - 1.0)*pmm) / (ll - m);
			pmm = pmmp1;
			pmmp1 = pll;
		}

		return pll;
	}

	double K( int l, int m ) {
		//double temp = ((2.0*l + 1.0)*FastFactorial( l - m )) / (4.0 * pi<double>( ) * FastFactorial( l + m ));
		double temp = ((2.0*l + 1.0)*factorial( l - m )) / (4.0 * pi<double>( ) * factorial( l + m ));
		return sqrt( temp );
	}

	double SH( int l, int m, double theta, double phi ) {
		const double sqrt2 = sqrt( 2.0 );
		if (m == 0) return K( l, 0 ) * P( l, m, cos( theta ) );
		else if (m > 0) return sqrt2 * K( l, m ) * cos( m*phi ) * P( l, m, cos( theta ) );
		else return sqrt2 * K( l, -m ) * sin( -m * phi ) * P( l, -m, cos( theta ) );
	}

	/**	Random with range [0.0..1.0)
	*******************************************************************************/
	float frand( ) { return (float)rand( ) / RAND_MAX; }
}

/**	Spherical Harmonics - Generate Samples
*******************************************************************************/
void SH_GenSamples( SH_Sample samples[], const size_t sampleSize ) {
	int sample_sqrt = sqrt( sampleSize );
	float oneOverN = 1.f / sample_sqrt;

	int i = 0;

	for (int a = 0; a < sample_sqrt; ++a)
		for (int b = 0; b < sample_sqrt; ++b) {
			vec2 rnd = vec2( a + frand( ), b + frand( ) ) * oneOverN;
			vec2 sph;
			vec3 vec;

			sph.x = 2.f * acos( sqrt( 1.f - rnd.y ) );
			sph.y = 2.f * pi<float>( ) * rnd.x;

			vec.x = cos( sph.x ) * sin( sph.y );
			vec.y = sin( sph.y );
			vec.z = sin( sph.x ) * sin( sph.y );

			samples[i].sph = sph;
			samples[i].vec = vec;

			int c = 0;
			for (int l = 0; l < 3; ++l)
				for (int m =-l; m <= l; ++m) {
					samples[i].coeff[c] = SH( l, m, sph.x, sph.y );
					++c;
				}

			++i;
		}
}

/**	Spherical Harmonics - Project
*******************************************************************************/
void SH_Project( SH_Func fn, const SH_Sample samples[], const size_t sampleSize, vec3 result[] ) {
	const double weight = 4.0 * glm::pi<double>( );

	// for each sample
	for (int i=0; i < sampleSize; ++i) {
		double theta = samples[i].sph.x;
		double phi = samples[i].sph.y;

		for (int n = 0; n < 9; ++n) {
			result[n] += fn( theta, phi ) * (float)samples[i].coeff[n];
		}
	}

	// divide the result by weight and number of samples
	double factor = weight / sampleSize;
	for (int i=0; i < 9; ++i) {
		result[i] = result[i] * (float)factor;
	}
}