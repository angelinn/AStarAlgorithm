#include "Maze.h"
#include "Node.h"
#include "CsvReader.h"

#include <fstream>
#include <string>

Maze::Maze(const Point& start, const Point& end)
	: startPosition(start), endPosition(end), map(NULL), rows(0), columns(0)
{  }

Maze::~Maze()
{
	delete[] map;
	map = NULL;

	////size_t openSize = open.size();
	////for (size_t i = 0; i < openSize; ++i)
	////	delete open[i];

	//size_t closedSize = closed.size();
	//for (size_t j = 0; j < closedSize; ++j)
	//	delete closed[j];

	////open.clear();
	//closed.clear();
}

void Maze::loadCsv(const std::string& filePath)
{
	CsvReader csvReader;
	std::vector<std::string> csv = csvReader.readCsv(filePath);
	parseCsv(csv);
}

void Maze::parseCsv(const std::vector<std::string>& csv)
{
	rows = csv.size() - 1;
	columns = csv[0].size() - 1;

	map = new Node**[rows];
	printf("Allocating %d rows..\n", rows);

	for (size_t i = 1, k = 0; i < csv.size(); ++i, ++k)
	{
		map[k] = new Node*[columns];
		printf("Allocating %d columns..\n", columns);
		for (size_t j = 1; j < csv[i].size(); ++j)
		{
			map[k][j] = new Node();

			if (csv[i][j] == WATER)
				map[k][j]->cost = 2;
			else
				map[k][j]->cost = 1;

			map[k][j]->location = Point(k, j);
			map[k][j]->isPassable = csv[i][j] != WALL;
			printf("[%c]", csv[i][j]);
		}
		printf("\n");
	}
}

std::vector<Node*> Maze::getNeighbours(const Node* current)
{
	int x = current->location.x;
	int y = current->location.y;

	return std::vector<Node*>();
}

void createEuristics()
{

}

void Maze::shortestPath()
{
	Node* current = map[startPosition.x][startPosition.y];
	open.push(current);

	while (true)
	{
		current = open.top();
		open.pop();
		closed.push_back(current);

		if (current->h == 0)
			return;



	}


}
