#pragma once

class SplineValue;
struct SplineElement;
class Spline;

enum class NoiseName;
struct NoiseMapCell;

#include <vector>
#include <variant>
#include <string>

class SplineValue
{
private:
	float _applyWeirness(const NoiseMapCell& noises) const;
public:
	NoiseName coordinate;
	std::vector<SplineElement> points;
	float getValue(const NoiseMapCell& noises) const;
};

struct SplineElement
{
	float location;
	std::variant<float, SplineValue> value;
	float derivative;
};

class Spline
{
private:
	SplineValue _value;

public:
	Spline(const std::string& jsonFileName);
	~Spline();

	void print() const;
	float getValue(const NoiseMapCell& noises) const;
};
