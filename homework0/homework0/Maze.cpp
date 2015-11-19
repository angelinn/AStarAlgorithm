#include "Maze.h"
#include "Node.h""

#include <fstream>
#include <string>

Maze::Maze(const std::string& filePath, const Point& start, const Point& end) 
	: startPosition(start), endPosition(end), map(NULL)
{
	std::string csv = readCsv(filePath);
	auto splitted = split(csv);
	parseCsv(splitted);
}

Maze::~Maze()
{
	delete[] map;
	map = NULL;

	size_t openSize = open.size();
	for (size_t i = 0; i < openSize; ++i)
		delete open[i];

	size_t closedSize = closed.size();
	for (size_t j = 0; j < closedSize; ++j)
		delete closed[j];

	open.clear();
	closed.clear();
}

std::string Maze::readCsv(const std::string& filePath) const
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

std::vector<std::string> Maze::split(const std::string& string, char delim) const
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

	return splitted;
}

void Maze::parseCsv(const std::vector<std::string>& csv)
{
	map = new Node*[csv.size()];
	printf("Allocating %d rows..\n", csv.size() - 1);

	for (size_t i = 1; i < csv.size(); ++i)
	{
		map[i] = new Node[csv[i].size()];
		printf("Allocation %d columns..\n", csv[i].size());
		for (size_t j = 1; j < csv[i].size(); ++j)
		{
			if (csv[i][j] == WATER)
				map[i][j].cost = 2;
			else
				map[i][j].cost = 1;

			map[i][j].isPassable = (!csv[i][j] == WALL);
			printf("%c(%d)", csv[i][j], map[i][j].cost);
		}
		printf("\n");
	}
}