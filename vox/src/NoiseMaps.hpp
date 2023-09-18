#pragma once

enum class NoiseName;
struct NoiseMapCell;
class NoiseMap;

#include "Spline.hpp"
#include <vector>
#include "Noise/NormalNoise.hpp"

enum class NoiseName
{
	continentalness,
	erosion,
	ridges
};

struct NoiseMapCell
{
	float PV;
	float C;
	float E;

	float getValueByName(NoiseName name) const
	{
		if (name == NoiseName::continentalness)
			return C;
		else if (name == NoiseName::erosion)
			return E;
		else
			return PV;
	}
};

class NoiseMap
{
private:
	static NoiseMap* _noiseMap;

	NoiseMapCell* _map;
	NoiseMapCell* _tempBuffer;
	size_t _size;
	size_t _dataSize;
	Spline offset;
	Spline factor;
	Spline jaggedness;
	NormalNoise* _temperature;
	NormalNoise* _humidity;
	NormalNoise* _continentalness;
	NormalNoise* _erosion;
	NormalNoise* _weirdness;
	NormalNoise* _shift;

	NoiseMap();
	~NoiseMap();

public:

	static void init();
	static void terminate();

	static void shift(int xOffset, int zOffset);

	static const NoiseMapCell& getNoisesAt(int x, int z);

	static float getHeightOffset(int x, int y);
	static float getSquashingFactor(int x, int y);
};
