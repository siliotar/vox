#include "NormalNoise.hpp"

static const double INPUT_FACTOR = 1.0181268882175227;

NormalNoise::NormalNoise(Xoroshiro& random, int firstOctave, const std::vector<float>& amplitudes)
	: _first(random, firstOctave, amplitudes), _second(random, firstOctave, amplitudes)
{
	float min = +INFINITY;
	float max = -INFINITY;

	size_t size = amplitudes.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (amplitudes[i])
		{
			min = min < i ? min : i;
			max = max > i ? max : i;
		}
	}

	float expectedDeviation = 0.1f * (1 + 1 / (max - min + 1));
	_valueFactor = (1.0f / 6) / expectedDeviation;
}

NormalNoise::~NormalNoise()
{}

float NormalNoise::sample(float x, float y, float z)
{
	float x2 = x * INPUT_FACTOR;
	float y2 = y * INPUT_FACTOR;
	float z2 = z * INPUT_FACTOR;
	float p1 = _first.sample(x, y, z);
	float p2 = _second.sample(x2, y2, z2);
	return (p1 + p2) * _valueFactor;
}
