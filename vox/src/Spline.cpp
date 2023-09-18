#include "Spline.hpp"
#include <fstream>
#include <iostream>
#include "json/json.hpp"
using json = nlohmann::json;
#include "NoiseMaps.hpp"
#include "glm/glm.hpp"
#include "Utils.hpp"

static float map(float oldMin, float oldMax, float newMin, float newMax, float value)
{
	float oldRange = oldMax - oldMin;
	float newRange = newMax - newMin;

	return (value - oldMin) / oldRange * newRange + newMin;
}

float interpolate_zero_grad(float alpha, float a, float b)
{
	return lerp(alpha, a, b) + alpha * (1.0f - alpha) * lerp(alpha, a - b, b - a);
}

static float getFloatFromVariant(const std::variant<float, SplineValue>& variant, const NoiseMapCell& noises)
{
	if (std::holds_alternative<float>(variant))
		return std::get<float>(variant);
	else
		return std::get<SplineValue>(variant).getValue(noises);
}

float getDerivativeLeft(float location, const NoiseMapCell& noises)
{
	float a = location / 3.0f - 0.3333334f;

	if (location == 1)
		return 3;
	else if (location == -1)
		return -3;
	else
		return 3;
}

float SplineValue::_applyWeirness(const NoiseMapCell& noises) const
{
	int wi = -1;
	size_t size = points.size();

	for (size_t i = 0; i < size; ++i)
		if (points[i].location < noises.PV)
			wi = i;

	if (wi < 0)
		return getFloatFromVariant(points[0].value, noises) + points[0].derivative * (noises.PV - points[0].location);

	if (wi == size - 1)
		return getFloatFromVariant(points[wi].value, noises) - points[wi].derivative * (noises.PV - points[wi].location);

	float loc0 = points[wi].location;
	float loc1 = points[wi + 1].location;
	float der0 = points[wi].derivative;
	float der1 = points[wi].derivative;
	float f = (noises.PV - loc0) / (loc1 - loc0);

	float val0 = getFloatFromVariant(points[wi].value, noises);
	float val1 = getFloatFromVariant(points[wi + 1].value, noises);

	float f8 = der0 * (loc1 - loc0) - (val1 - val0);
	float f9 = -der1 * (loc1 - loc0) - (val1 - val0);
	float f10 = lerp(f, val0, val1) + f * (1.0f - f) * lerp(f, f8, f9);
	return f10;
}

float SplineValue::getValue(const NoiseMapCell& noises) const
{
	if (coordinate == NoiseName::ridges)
		return _applyWeirness(noises);
	float noise = noises.getValueByName(coordinate);
	for (size_t i = 0; i < points.size(); ++i)
	{
		if (noise <= points[i].location)
		{
			float rightValue = getFloatFromVariant(points[i].value, noises);
			float leftValue;
			if (i == 0)
				return rightValue;
			else
				leftValue = getFloatFromVariant(points[i - 1].value, noises);
			return interpolate_zero_grad((noise - points[i - 1].location) / (points[i].location - points[i - 1].location), leftValue, rightValue);
		}
	}
	return getFloatFromVariant(points[points.size() - 1].value, noises);
}

static void _parsePoints(const json& data, std::vector<SplineElement>& points)
{
	for (auto element : data)
	{
		points.emplace_back();
		SplineElement& el = points[points.size() - 1];
		el.location = element["location"].get<float>();
		el.derivative = element["derivative"].get<float>();
		if (element["value"].is_object())
		{
			SplineValue value;
			value.coordinate = element["value"]["coordinate"].get<std::string>() == "erosion" ? NoiseName::erosion : NoiseName::ridges;
			_parsePoints(element["value"]["points"], value.points);
			el.value = value;
		}
		else
			el.value = element["value"].get<float>();
	}
}

std::string nameToStr(NoiseName name)
{
	if (name == NoiseName::continentalness)
		return "continentalness";
	else if (name == NoiseName::erosion)
		return "erosion";
	else
		return "ridge";
}

void printValue(const SplineValue& value, size_t depth)
{
	for (size_t i = 0; i < depth; ++i)
		std::cout << "\t";
	std::cout << nameToStr(value.coordinate) << std::endl;
	for (auto el : value.points)
	{
		for (size_t i = 0; i < depth; ++i)
			std::cout << "\t";
		std::cout << "location: " << el.location << std::endl;
		if (std::holds_alternative<float>(el.value))
		{
			for (size_t i = 0; i < depth; ++i)
				std::cout << "\t";
			std::cout << "value: " << std::get<float>(el.value) << std::endl;
		}
		else
		{
			for (size_t i = 0; i < depth; ++i)
				std::cout << "\t";
			std::cout << "value: " << "{" << std::endl;
			printValue(std::get<SplineValue>(el.value), depth + 1);
			for (size_t i = 0; i < depth; ++i)
				std::cout << "\t";
			std::cout << "}" << std::endl;
		}
	}
}

void Spline::print() const
{
	printValue(_value, 0);
}

Spline::Spline(const std::string& jsonFileName)
{
	_value.coordinate = NoiseName::continentalness;

	std::fstream fileStream(jsonFileName);
	json data = json::parse(fileStream);

	_parsePoints(data, _value.points);
}

Spline::~Spline()
{}

float Spline::getValue(const NoiseMapCell& noises) const
{
	return _value.getValue(noises);
}
