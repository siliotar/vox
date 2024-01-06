#pragma once

#include "Xoroshiro.hpp"
#include "ImprovedNoise.hpp"
#include <vector>

class PerlinNoise
{
private:
	std::vector<ImprovedNoise*> _noiseLevels;
	std::vector<float> _amplitudes;
	float _lowestFreqInputFactor;
	float _lowestFreqValueFactor;

	float _edgeValues(float x);
public:
	float maxValue;

	PerlinNoise(Xoroshiro& random, int firstOctave, const std::vector<float> amplitudes);
	~PerlinNoise();

	float sample(float x, float y, float z);
};
