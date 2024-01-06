#include "PerlinNoise.hpp"

PerlinNoise::PerlinNoise(Xoroshiro& random, int firstOctave, const std::vector<float> amplitudes)
	: _amplitudes(amplitudes)
{
	const Xoroshiro forked = random.fork();

	size_t size = _amplitudes.size();
	_noiseLevels.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		if (_amplitudes[i] != 0.0f)
		{
			int octave = firstOctave + i;
			_noiseLevels[i] = new ImprovedNoise(forked.forkWithHashOf("octave_" + std::to_string(octave)));
		}
		else
			_noiseLevels[i] = 0;
	}

	_lowestFreqInputFactor = powf(2, firstOctave);
	_lowestFreqValueFactor = powf(2, size - 1) / (powf(2, size) - 1);
	maxValue = _edgeValues(2);
}

PerlinNoise::~PerlinNoise()
{
	size_t size = _noiseLevels.size();
	for (size_t i = 0; i < size; ++i)
		if (_noiseLevels[i])
			delete _noiseLevels[i];
}

float PerlinNoise::_edgeValues(float x)
{
	float value = 0;
	float valueF = _lowestFreqValueFactor;

	size_t size = _noiseLevels.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_noiseLevels[i])
			value += _amplitudes[i] * x * valueF;
		valueF /= 2;
	}
	return value;
}

static float wrap(float value)
{
	return value - floorf(value / 33554432.0f + 0.5) * 33554432.0f;
}

float PerlinNoise::sample(float x, float y, float z)
{
	float value = 0;
	float inputF = _lowestFreqInputFactor;
	float valueF = _lowestFreqValueFactor;

	size_t size = _noiseLevels.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (_noiseLevels[i])
			value += _amplitudes[i] * valueF * _noiseLevels[i]->sample(wrap(x * inputF), wrap(y * inputF), wrap(z * inputF));
		inputF *= 2;
		valueF /= 2;
	}
	return value;
}
