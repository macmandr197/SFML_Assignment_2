#include <SFML/System/String.hpp>
#include "Book/Utility.hpp" 
#include <random>

float Utility::Clamp(float value, float lower, float upper)
{
	return value <= lower ? lower : value >= upper ? upper : value;
	return value;
}

sf::String Utility::toString(int integer)
{
	std::ostringstream os;
	os << integer;
	return os.str();
}

float Utility::RandomFloatInRange(float a, float b)
{
	float random = float(rand()) / float(RAND_MAX);
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

int Utility::RandomIntInRange(int a, int b)
{
	std::random_device rseed;
	std::mt19937 rng(rseed());
	std::uniform_int<int> dist(a, b);
	return dist(rng);
}
