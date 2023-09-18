#pragma once

#include "BigInt.hpp"

class XoroshiroPositional;

class Xoroshiro
{
private:
	BigInt _seedLo;
	BigInt _seedHi;

	BigInt _next();
	BigInt _nextBits(size_t bits);
	BigInt _rotateLeft(const BigInt& value, size_t shift);
	void _rotateLeft(const BigInt& value, size_t shift, BigInt& result);

public:
	Xoroshiro(const BigInt& seed);
	Xoroshiro(const BigInt& seedLo, const BigInt& seedHi);
	Xoroshiro(const Xoroshiro& other);

	int nextInt();
	int nextInt(int max);
	double nextDouble();

	Xoroshiro fork();
	Xoroshiro forkWithHashOf(const std::string& name);
};
