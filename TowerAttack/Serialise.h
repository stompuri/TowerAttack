#pragma once

#include <SFML/System.hpp> // We are using stuff from there

namespace TA
{
	//void SerialiseString(std::ostream & os, std::string _string); // Serialise string into bytes
	std::string ReadString(std::istream & is); // Read string (1st the length 2nd the characters) and return it
}