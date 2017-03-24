#pragma once
#include <glm/glm.hpp>
static const int SH_COEFF_N = 0;
static const int SH_SAMPLE_SIZE = 10000;

struct SH_Sample {
	glm::vec2 sph;
	glm::vec3 vec;
	double coeff[9];
};

typedef glm::vec3 (*SH_Func)(double theta, double phi);

void SH_GenSamples( SH_Sample samples[], const size_t sampleSize );
void SH_Project( SH_Func fn, const SH_Sample samples[], const size_t sampleSize, glm::vec3 result[] );