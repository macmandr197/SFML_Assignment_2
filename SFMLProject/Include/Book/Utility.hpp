#include <sstream>
#include "String"
#include <SFML/System/String.hpp>


class Utility
{
public:
	float Clamp(float, float, float);
	sf::String toString(int);
	float RandomFloatInRange(float, float);
	int RandomIntInRange(int,int);

};