#include "BigInt.hpp"

float lerp(float a, float b, float c)
{
	return b + a * (c - b);
}

float lerp2(float a, float b, float c, float d, float e, float f)
{
	return lerp(b, lerp(a, c, d), lerp(a, e, f));
}

float lerp3(float a, float b, float c, float d, float e, float f, float g, float h, float i, float j, float k)
{
	return lerp(c, lerp2(a, b, d, e, f, g), lerp2(a, b, h, i, j, k));
}

float smoothStep(float x)
{
	return x * x * x * (x * (x * 6 - 15) + 10);
}

BigInt longFromBytes(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned char e, unsigned char f, unsigned char g, unsigned char h)
{
	return BigInt(a) << 56 | BigInt(b) << 48 | BigInt(c) << 40 | BigInt(d) << 32 | BigInt(e) << 24 | BigInt(f) << 16 | BigInt(g) << 8 | BigInt(h);
}
