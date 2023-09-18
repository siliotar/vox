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

float ImprovedNoise::_sampleAndLerp(float a, float b, float c, float d, float e, float f)
{
	int h = _P(a);
	int i = _P(a + 1);
	int j = _P(h + b);
	int k = _P(h + b + 1);
	int l = _P(i + b);
	int m = _P(i + b + 1);

	float n = SimplexNoise::gradDot(_P(j + c), d, e, f);
	float o = SimplexNoise::gradDot(_P(l + c), d - 1, e, f);
	float p = SimplexNoise::gradDot(_P(k + c), d, e - 1, f);
	float q = SimplexNoise::gradDot(_P(m + c), d - 1, e - 1, f);
	float r = SimplexNoise::gradDot(_P(j + c + 1), d, e, f - 1);
	float s = SimplexNoise::gradDot(_P(l + c + 1), d - 1, e, f - 1);
	float t = SimplexNoise::gradDot(_P(k + c + 1), d, e - 1, f - 1);
	float u = SimplexNoise::gradDot(_P(m + c + 1), d - 1, e - 1, f - 1);

	float v = smoothStep(d);
	float w = smoothStep(e);
	float x = smoothStep(f);

	return lerp3(v, w, x, n, o, p, q, r, s, t, u);
}

float ImprovedNoise::sample(float x, float y, float z)
{
	float x2 = x + _xo;
	float y2 = y + _yo;
	float z2 = z + _zo;
	float x3 = floorf(x2);
	float y3 = floorf(y2);
	float z3 = floorf(z2);
	float x4 = x2 - x3;
	float y4 = y2 - y3;
	float z4 = z2 - z3;

	return _sampleAndLerp(x3, y3, z3, x4, y4, z4);
}
