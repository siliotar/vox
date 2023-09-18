#include "SimplexNoise.hpp"

static int GRADIENT[16][3] = {
	{ 1,  1,  0},
	{-1,  1,  0},
	{ 1, -1,  0},
	{-1, -1,  0},
	{ 1,  0,  1},
	{-1,  0,  1},
	{ 1,  0, -1},
	{-1,  0, -1},
	{ 0,  1,  1},
	{ 0, -1,  1},
	{ 0,  1, -1},
	{ 0, -1, -1},
	{ 1,  1,  0},
	{ 0, -1,  1},
	{-1,  1,  0},
	{ 0, -1, -1}
};

float SimplexNoise::gradDot(int a, float b, float c, float d)
{
	const int* grad = GRADIENT[a & 15];
	return grad[0] * b + grad[1] * c + grad[2] * d;
}
