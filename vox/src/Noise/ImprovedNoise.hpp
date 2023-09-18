#pragma once

class Xoroshiro;

class ImprovedNoise
{
private:
	int* _p;
	float _xo;
	float _yo;
	float _zo;

	float _sampleAndLerp(float a, float b, float c, float d, float e, float f);
	int _P(int i);

public:
	ImprovedNoise(Xoroshiro& random);
	~ImprovedNoise();

	float sample(float x, float y, float z);
};
