#include "Xoroshiro.hpp"

#include <iostream>
#include <chrono>
#include "md5.hpp"
#include "Utils.hpp"

static const BigInt SILVER_RATIO_64 = "7640891576956012809";
static const BigInt GOLDEN_RATIO_64 = "-7046029254386353131";

static const BigInt STAFFORD_1 = "-4658895280553007687";
static const BigInt STAFFORD_2 = "-7723592293110705685";

static const BigInt POW2_60 = "18446744073709551616";
static const BigInt MAX_ULONG = "18446744073709551615";
static const BigInt MAX_UINT = "4294967295";

static const float FLOAT_MULTIPLIER = 1 / pow(2, 24);
static const double DOUBLE_MULTIPLIER = 1.1102230246251565E-16;

static void mixStafford13(BigInt& value)
{
	value = ((value ^ value >> 30) * STAFFORD_1) & MAX_ULONG;
	value = ((value ^ value >> 27) * STAFFORD_2) & MAX_ULONG;
	value = (value ^ value >> 31) & MAX_ULONG;
}

Xoroshiro::Xoroshiro(const BigInt& seed)
{
	if (seed < 0)
		_seedLo = (seed + POW2_60) ^ SILVER_RATIO_64;
	else
		_seedLo = seed ^ SILVER_RATIO_64;
	_seedHi = (_seedLo + GOLDEN_RATIO_64) & MAX_ULONG;
	mixStafford13(_seedLo);
	mixStafford13(_seedHi);
}

Xoroshiro::Xoroshiro(const BigInt& seedLo, const BigInt& seedHi) : _seedLo(seedLo), _seedHi(seedHi)
{}

Xoroshiro::Xoroshiro(const Xoroshiro& other) : _seedLo(other._seedLo), _seedHi(other._seedHi)
{}

BigInt Xoroshiro::_rotateLeft(const BigInt& value, size_t shift)
{
	BigInt v = value;
	v <<= shift;
	v &= MAX_ULONG;
	v |= (value >> (64 - shift));
	return v;
}

void Xoroshiro::_rotateLeft(const BigInt& value, size_t shift, BigInt& result)
{
	result = value;
	result <<= shift;
	result &= MAX_ULONG;
	result |= (value >> (64 - shift));
}

BigInt Xoroshiro::_next()
{
	const BigInt l = _seedLo;
	BigInt h = _seedHi;

	const BigInt value = (_rotateLeft((l + h) & MAX_ULONG, 17) + l) & MAX_ULONG;

	h ^= l;

	_rotateLeft(l, 49, _seedLo);
	_seedLo ^= h;
	_seedLo ^= ((h << 21) & MAX_ULONG);
	_rotateLeft(h, 28, _seedHi);

	return value;
}

BigInt Xoroshiro::_nextBits(size_t bits)
{
	return _next() >> (64 - bits);
}

int Xoroshiro::nextInt()
{
	BigInt bi = _next();
	bi &= MAX_UINT;
	long long value = bi.getLL();
	if (value >= 0x80000000)
		return value - 0x100000000;
	return value;
}

int Xoroshiro::nextInt(int max)
{
	BigInt bi = _next();
	bi &= MAX_UINT;
	BigInt maxBigInt(max);
	BigInt product = bi * maxBigInt;
	BigInt productLo = product;
	productLo &= MAX_UINT;
	if (productLo < maxBigInt)
	{
		const BigInt newMax = ((~maxBigInt & MAX_UINT) + 1) % maxBigInt;
		while (productLo < newMax)
		{
			bi = _next() & MAX_UINT;
			product = bi * maxBigInt;
			productLo = product & MAX_UINT;
		}
	}
	const BigInt productHi = product >> 32;
	return productHi.getLL();
}

double Xoroshiro::nextDouble()
{
	return _nextBits(53).getLL() * DOUBLE_MULTIPLIER;
}

Xoroshiro Xoroshiro::fork()
{
	return { _next(), _next() };
}

Xoroshiro Xoroshiro::forkWithHashOf(const std::string& name) const
{
	std::vector<unsigned char> hash = md5AsBytes(name);

	BigInt lo = longFromBytes(hash[0], hash[1], hash[2], hash[3], hash[4], hash[5], hash[6], hash[7]);
	BigInt hi = longFromBytes(hash[8], hash[9], hash[10], hash[11], hash[12], hash[13], hash[14], hash[15]);

	return { lo ^ _seedLo, hi ^ _seedHi };
}
