#include "Maze.h"
#include "Node.h"
#include "CsvReader.h"
#include "Definitions.h"

#include <string>

Maze::Maze(const Point& start, const Point& end)
	: startPosition(start), endPosition(end), map(NULL), rows(0), columns(0)
{  }

Maze::~Maze()
{
	for (int i = 0; i < rows; ++i)
	{
		for (int j = 0; j < columns; ++j)
			delete map[i][j];

		delete[] map[i];
	}

	delete[] map;
	map = NULL;
}

void Maze::loadCsv(const std::string& filePath)
{
	CsvReader csvReader;
	std::vector<std::string> csv = csvReader.readCsv(filePath);
	parseCsv(csv);
}

void Maze::parseCsv(const std::vector<std::string>& csv)
{
	rows = csv.size();
	columns = csv[0].size();

	map = new Node**[rows];

	for (size_t i = 0; i < rows; ++i)
	{
		map[i] = new Node*[columns];
		for (size_t j = 0; j < columns; ++j)
		{
			map[i][j] = new Node();

			if (csv[i][j] == WATER)
				map[i][j]->cost = WATER_TYPE;
			else
				map[i][j]->cost = NORMAL_TYPE;

			map[i][j]->location = Point(i, j);
			map[i][j]->isPassable = csv[i][j] != WALL;
		}
	}
}

std::vector<Node*> Maze::getNeighbours(const Node* current)
{
	int x = current->location.x;
	int y = current->location.y;

	std::vector<Node*> neighbours;

	if (x - 1 >= 0)
	{
		neighbours.push_back(map[x - 1][y]);
		if (y + 1 < columns)
			neighbours.push_back(map[x - 1][y + 1]);
		if (y - 1 >= 0)
			neighbours.push_back(map[x - 1][y - 1]);
	}
	if (x + 1 < rows)
	{
		neighbours.push_back(map[x + 1][y]);
		if (y + 1 < columns)
			neighbours.push_back(map[x + 1][y + 1]);
		if (y - 1 >= 0)
			neighbours.push_back(map[x + 1][y - 1]);
	}
	if (y - 1 >= 0)
		neighbours.push_back(map[x][y - 1]);
	if (y + 1 < columns)
		neighbours.push_back(map[x][y + 1]);

	return neighbours;
}

size_t Maze::calculateHeuristics(Node* current) const
{
	size_t targetCost = 0;
	int x = current->location.x;
	int y = current->location.y;

	int xDifference = endPosition.x - x;
	int yDifference = endPosition.y - y;

	while (yDifference && xDifference)
	{
		targetCost += map[x][y]->cost == NORMAL_TYPE ? DIAGONAL_COST : WATER_COST;
		x = xDifference > 0 ? x + 1 : x - 1;
		y = yDifference > 0 ? y + 1 : y - 1;


		xDifference = endPosition.x - x;
		yDifference = endPosition.y - y;
	}

	while (xDifference)
	{
		targetCost += map[x][y]->cost * NORMAL_COST;
		x = xDifference > 0 ? x + 1 : x - 1;
		xDifference = endPosition.x - x;
	}

	while (yDifference)
	{
		targetCost += map[x][y]->cost * NORMAL_COST;
		y = yDifference > 0 ? y + 1 : y - 1;
		yDifference = endPosition.y - y;
	}

	return targetCost;
}

size_t Maze::getCostFromStart(const Point& parentLocation, const Point& currentLocation) const
{
	int xDifference = currentLocation.x - parentLocation.x;
	int yDifference = currentLocation.y - parentLocation.y;

	if (xDifference && yDifference)
		return DIAGONAL_COST;

	return NORMAL_COST;
}

void Maze::printPath(const Node* target) const
{
	const Node* current = target;
	char** matrix = new char*[rows];
	for (int i = 0; i < rows; ++i)
		matrix[i] = new char[columns] { ' ' };


	while (current->parent)
	{
		matrix[current->location.x][current->location.y] = PATH;
		current = current->parent;
	}

	printf("\n");

	matrix[startPosition.x][startPosition.y] = PLAYER;
	matrix[endPosition.x][endPosition.y] = TARGET;
	
	char printer = 0;

	for (int k = 0; k < columns; ++k)
		printf("  %c", 'A' + k);

	for (int i = 0; i < rows; ++i)
	{
		printf("\n");
		printf("%i", i);
		for (int j = 0; j < columns; ++j)
		{
			if (!map[i][j]->isPassable)
				printer = WALL;
			else if (map[i][j]->cost == WATER_TYPE)
				printer = WATER;
			else
				printer = matrix[i][j];

			printf("[%c]", printer);
		}
	}

	for (int i = 0; i < rows; ++i)
		delete[] matrix[i];

	delete[] matrix;
}

std::vector<std::string> Maze::shortestPath()
{
	Node* current = map[startPosition.x][startPosition.y];
	current->h = calculateHeuristics(current);
	open.push(current);

	while (true)
	{
		current = open.pop();
		closed.push_back(current->location);

		if (current->h == 0)
		{
			std::vector<std::string> result;
			std::string coordinates;

			printPath(current);
			while (current->parent)
			{
				coordinates = "(" + std::string(1, 'A' + current->location.y) + ", " +
									std::to_string(current->location.x) + ")";

				result.push_back(coordinates);
				current = current->parent;
			}

			std::reverse(result.begin(), result.end());
			return result;
		}

		std::vector<Node*> neighbours = getNeighbours(current);
		for (Node* neighbour : neighbours)
		{
			if (!neighbour->isPassable || std::find(closed.begin(), closed.end(), neighbour->location) != closed.end())
				continue;

			size_t newG = (current->cost == WATER_TYPE) 
						  ? WATER_COST 
						  : getCostFromStart(current->location, neighbour->location);

			newG += current->g;
			if (neighbour->g == 0 || newG <= neighbour->g)
			{
				neighbour->parent = current;
				neighbour->g = newG;
				neighbour->h = calculateHeuristics(neighbour);

				if (!open.exists(neighbour))
					open.push(neighbour);
			}
		}
	}
}
