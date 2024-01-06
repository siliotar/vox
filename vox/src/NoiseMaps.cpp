#include "NoiseMaps.hpp"
#include "Settings.hpp"
#include <cstring>
#include "Noise/PerlinNoise.hpp"
#include <cmath>
#include "Camera.hpp"

NoiseMap* NoiseMap::_noiseMap = nullptr;

static const std::vector<float> erosionAmplitudes = { 1.0f, 1.0f, 0.0f, 1.0f, 1.0f };
static const std::vector<float> continentalnessAmplitudes = { 1.0f, 1.0f, 2.0f, 2.0f, 2.0f, 1.0f, 1.0f, 1.0f, 1.0f };
static const std::vector<float> pvAmplitudes = { 1.0f, 2.0f, 1.0f, 0.0f, 0.0f, 0.0f };
static const std::vector<float> shiftAmplitudes = { 1.0f, 1.0f, 1.0f, 0.0f};
static const std::vector<float> temperatureAmplitudes = { 1.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f };
static const std::vector<float> humidityAmplitudes = { 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f };

static float _getNoiseAt(int x, int y, size_t octaves, float frequency, float amplitude, float scale)
{
	float value = 0.0f;
	float freq = scale;
	float amp = 1.0f;
	float accumulatedAmps = 0.0f;

	for (int i = 0; i < octaves; ++i)
	{
		float tx = x * freq;
		float ty = y * freq;

		//float noise = perlinNoise(SEED, tx, ty);
		float noise = 1;
		value += noise * amp;
		accumulatedAmps += amp;

		freq *= frequency;
		amp *= amplitude;
	}
	return value / accumulatedAmps;
}

static float _gNoiseAt(int x, int y, int firstOctave, const std::vector<float>& amplitudes)
{
	float value = 0;
	float inputF = powf(2.0f, firstOctave);
	float valueF = 1.0f;

	for (size_t i = 0; i < amplitudes.size(); ++i)
	{
		if (amplitudes[i] != 0.0f)
			//value += amplitudes[i] * valueF * perlinNoise(SEED, x * inputF, y * inputF);
			value += amplitudes[i] * valueF * 1;
		inputF *= 2.0f;
		valueF /= 2.0f;
	}

	return value;
}

static float _getPVAt(int x, int y)
{
	float value = _gNoiseAt(x, y, -7, pvAmplitudes);
	//value = abs(value) * 2.0f - 1.0f;
	return value;
}

static float _getCAt(int x, int y)
{
	float value = _gNoiseAt(x, y, -9, continentalnessAmplitudes);
	return value;
}

static float _getEAt(int x, int y)
{
	float value = _gNoiseAt(x, y, -9, erosionAmplitudes);
	return value;
}

static NoiseMapCell _getNoisesAt(int x, int y)
{
	NoiseMapCell result;

	result.PV = _getPVAt(x, y);
	result.C = _getCAt(x, y);
	result.E = _getEAt(x, y);

	return result;
}

void NoiseMap::init()
{
	if (_noiseMap == nullptr)
		_noiseMap = new NoiseMap;
}

void NoiseMap::terminate()
{
	delete _noiseMap;
}

NoiseMap::NoiseMap() :
	_map(nullptr), _size(0), 
	offset("res/offset.json"), factor("res/factor.json"), jaggedness("res/jaggedness.json")
{
	_size = RenderDistance * 2 + 1;
	_dataSize = _size * CHUNK_X * _size * CHUNK_Z;
	_map = new NoiseMapCell[_dataSize];
	_tempBuffer = new NoiseMapCell[_dataSize];

	int maxX = _size * CHUNK_X;
	int maxZ = _size * CHUNK_Z;

	const glm::vec3& playerPos = Camera::getPlayerPosition();
	int startX = (static_cast<int>(playerPos.x) / CHUNK_X - RenderDistance) * CHUNK_X;
	int startZ = (static_cast<int>(playerPos.z) / CHUNK_Z - RenderDistance) * CHUNK_Z;

	const Xoroshiro random = Xoroshiro(SEED).fork();
	_temperature = new NormalNoise(random.forkWithHashOf("minecraft:temperature"), -10, temperatureAmplitudes);
	_humidity = new NormalNoise(random.forkWithHashOf("minecraft:vegetation"), -8, humidityAmplitudes);
	_continentalness = new NormalNoise(random.forkWithHashOf("minecraft:continentalness"), -9, continentalnessAmplitudes);
	_erosion = new NormalNoise(random.forkWithHashOf("minecraft:erosion"), -9, erosionAmplitudes);
	_weirdness = new NormalNoise(random.forkWithHashOf("minecraft:ridge"), -7, pvAmplitudes);
	_shift = new NormalNoise(random.forkWithHashOf("minecraft:offset"), -3, shiftAmplitudes);

	for (int z = 0; z < maxZ; ++z)
	{
		float tz = startZ + z;
		tz /= 4.0f;
		for (int x = 0; x < maxX; ++x)
		{
			float tx = startX + x;
			tx /= 4.0f;
			float xx = tx + _shift->sample(tx, 0, tz) * 4;
			float zz = tz + _shift->sample(tz, tx, 0) * 4;

			NoiseMapCell cell;
			cell.C = _continentalness->sample(xx, 0, zz);
			cell.E = _erosion->sample(xx, 0, z);
			cell.PV = _weirdness->sample(xx, 0, zz);
			_map[x + z * maxX] = cell;
		}
	}
}

NoiseMap::~NoiseMap()
{
	delete[] _map;
	delete[] _tempBuffer;
}

void NoiseMap::shift(int xOffset, int zOffset)
{
	int maxX = _noiseMap->_size * CHUNK_X;
	int maxZ = _noiseMap->_size * CHUNK_Z;

	xOffset *= CHUNK_X;
	zOffset *= CHUNK_Z;

	const glm::vec3& playerPos = Camera::getPlayerPosition();
	int startX = static_cast<int>(playerPos.x) / CHUNK_X * CHUNK_X - RenderDistance * CHUNK_X;
	int startZ = static_cast<int>(playerPos.z) / CHUNK_X * CHUNK_X - RenderDistance * CHUNK_Z;

	for (int z = 0; z < maxZ; ++z)
	{
		int oldZ = z + zOffset;
		for (int x = 0; x < maxX; ++x)
		{
			int oldX = x + xOffset;
			float zz = startZ + z;
			zz /= 4.0f;
			if (oldX >= maxX || oldX < 0 || oldZ >= maxZ || oldZ < 0)
			{
				NoiseMapCell cell;
				float xx = startX + x;
				xx /= 4.0f;
				xx = xx + _noiseMap->_shift->sample(xx, 0, zz) * 4;
				zz = zz + _noiseMap->_shift->sample(zz, xx, 0) * 4;
				cell.C = _noiseMap->_continentalness->sample(xx, 0, zz);
				cell.E = _noiseMap->_erosion->sample(xx, 0, zz);
				cell.PV = _noiseMap->_weirdness->sample(xx, 0, zz);
				_noiseMap->_tempBuffer[x + z * maxX] = cell;
			}
			else
				_noiseMap->_tempBuffer[x + z * maxX] = _noiseMap->_map[oldX + oldZ * maxX];
		}
	}

	NoiseMapCell* temp = _noiseMap->_map;
	_noiseMap->_map = _noiseMap->_tempBuffer;
	_noiseMap->_tempBuffer = temp;
}

const NoiseMapCell& NoiseMap::getNoisesAt(int x, int z)
{
	const glm::vec3& playerPos = Camera::getPlayerPosition();
	x -= (static_cast<int>(playerPos.x) / CHUNK_X - RenderDistance) * CHUNK_X;
	z -= (static_cast<int>(playerPos.z) / CHUNK_Z - RenderDistance) * CHUNK_Z;

	const NoiseMapCell& cell = _noiseMap->_map[x + z * _noiseMap->_size * CHUNK_X];
	return cell;
}

float NoiseMap::getHeightOffset(int x, int y)
{
	const NoiseMapCell& cell = getNoisesAt(x, y);
	return _noiseMap->offset.getValue(cell);
}

float NoiseMap::getSquashingFactor(int x, int y)
{
	const NoiseMapCell& cell = getNoisesAt(x, y);
	return _noiseMap->factor.getValue(cell);
}
