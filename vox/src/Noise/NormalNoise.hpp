#pragma once

#include "PerlinNoise.hpp"

class NormalNoise
{
private:
	float _valueFactor;
	PerlinNoise _first;
	PerlinNoise _second;

public:
	NormalNoise(Xoroshiro& random, int firstOctave, const std::vector<float>& amplitudes);
	~NormalNoise();

	float sample(float x, float y, float z);
};
