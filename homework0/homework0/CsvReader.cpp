#include "CsvReader.h"

#include <fstream>
#include <string>

std::vector<std::string> CsvReader::readCsv(const std::string& filePath) const
{
	std::string csvString = read(filePath);
	return split(csvString);
}

std::string CsvReader::read(const std::string& filePath) const
{
	std::ifstream mapHandle(filePath, std::ios::in);
	if (!mapHandle)
		throw std::exception("Couldn't open map file.");

	std::string current;
	std::string message;

	int i = 0;
	while (!mapHandle.eof())
	{
		std::getline(mapHandle, current, ',');
		message += current;
	}

	mapHandle.close();
	return message;
}

std::vector<std::string> CsvReader::split(const std::string& string, char delim) const
{
	std::vector<std::string> splitted;
	std::string line;

	for (char ch : string)
	{
		if (ch == '\n')
		{
			splitted.push_back(line);
			line.clear();
			continue;
		}
		line.push_back(ch);
	}

	splitted.push_back(line);
	return splitted;
}