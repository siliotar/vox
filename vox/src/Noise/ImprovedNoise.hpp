#pragma once

class Xoroshiro;

class ImprovedNoise
{
private:
	int* _p;
	double _xo;
	double _yo;
	double _zo;

	float _sampleAndLerp(double a, double b, double c, double d, double e, double f);
	int _P(int i);

public:
	ImprovedNoise(Xoroshiro& random);
	~ImprovedNoise();

	float sample(double x, double y, double z);
};
