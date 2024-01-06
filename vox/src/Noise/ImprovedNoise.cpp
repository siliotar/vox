#include "ImprovedNoise.hpp"
#include "Xoroshiro.hpp"
#include "SimplexNoise.hpp"
#include "Utils.hpp"

ImprovedNoise::ImprovedNoise(Xoroshiro& random)
	: _p(new int[256]), _xo(random.nextDouble() * 256),
	_yo(random.nextDouble() * 256), _zo(random.nextDouble() * 256)
{
	for (size_t i = 0; i < 256; ++i)
		_p[i] = i > 127 ? i - 256 : i;
	for (size_t i = 0; i < 256; ++i)
	{
		int j = random.nextInt(256 - i);
		int t = _p[i];
		_p[i] = _p[i + j];
		_p[i + j] = t;
	}
}

ImprovedNoise::~ImprovedNoise()
{
	delete[] _p;
}

int ImprovedNoise::_P(int i)
{
	return _p[i & 255] & 255;
}

float ImprovedNoise::_sampleAndLerp(double a, double b, double c, double d, double e, double f)
{
	int h = _P(a);
	int i = _P(a + 1);
	int j = _P(h + b);
	int k = _P(h + b + 1);
	int l = _P(i + b);
	int m = _P(i + b + 1);

	double n = SimplexNoise::gradDot(_P(j + c), d, e, f);
	double o = SimplexNoise::gradDot(_P(l + c), d - 1, e, f);
	double p = SimplexNoise::gradDot(_P(k + c), d, e - 1, f);
	double q = SimplexNoise::gradDot(_P(m + c), d - 1, e - 1, f);
	double r = SimplexNoise::gradDot(_P(j + c + 1), d, e, f - 1);
	double s = SimplexNoise::gradDot(_P(l + c + 1), d - 1, e, f - 1);
	double t = SimplexNoise::gradDot(_P(k + c + 1), d, e - 1, f - 1);
	double u = SimplexNoise::gradDot(_P(m + c + 1), d - 1, e - 1, f - 1);

	double v = smoothStep(d);
	double w = smoothStep(e);
	double x = smoothStep(f);

	return lerp3(v, w, x, n, o, p, q, r, s, t, u);
}

float ImprovedNoise::sample(double x, double y, double z)
{
	double x2 = x + _xo;
	double y2 = y + _yo;
	double z2 = z + _zo;
	double x3 = floor(x2);
	double y3 = floor(y2);
	double z3 = floor(z2);
	double x4 = x2 - x3;
	double y4 = y2 - y3;
	double z4 = z2 - z3;

	return _sampleAndLerp(x3, y3, z3, x4, y4, z4);
}
