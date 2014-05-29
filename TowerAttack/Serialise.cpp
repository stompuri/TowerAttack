#include "Serialise.h"
#include "stdafx.h"

namespace TA
{
	/*void SerialiseString(std::ostream & os, std::string _string)
	{
		std::cout << "DEBUG <SerialiseString>: Saving string: '" + _string + "'\n";
		// Save the length of the file name string
		int fSize = _string.length();
		os.write((char*)&fSize, sizeof(int));
		// Go through the file name string char by char saving each char
		const char* strdata = _string.c_str();

		std::cout << "DEBUG <SerialiseString>: Saving... fSize = " + std::to_string(fSize) + " strData[1] = " + strdata[1] + "\n";
		for (unsigned int i = 0; i < _string.length(); ++i)
		{
			os.write((char*)&strdata[i], sizeof(char)); // save filename char by char
			std::cout << "[" << strdata[i] << "]";
		}
		std::cout << "\nDEBUG <SerialiseString>: Filename saved\n";
	}*/

	std::string ReadString(std::istream & is)
	{
		std::cout << "DEBUG <ReadString>: Reading file name\n";
		// Read the length of the file name
		int fSize = 0;
		is.read((char*)&fSize, sizeof(int));
		std::cout << "DEBUG <ReadString>: Size = " + std::to_string(fSize) + "\n";
		// Make a char pointer and initialize space for the characters in file name string
		char *cString = new char[fSize];

		// Read the file name char by char
		for (int i = 0; i < fSize; ++i)
		{
			is.read((char*)&cString[i], sizeof(char)); // Read filename char by char
			std::cout << "[" << cString[i] << "]";
		}
		cString[fSize] = '\0';
		std::string _string = cString;
		std::cout << "\nDEBUG <ReadString>: String read:\n" + _string + "\n";

		return _string;
	}
}