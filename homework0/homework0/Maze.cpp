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
	rows = csv.size() - 1;
	columns = csv[0].size() - 1;

	map = new Node**[rows];
	printf("Allocating %d rows..\n", rows);
	printf("Allocating %d columns per row..\n", columns);

	for (size_t i = 1, k = 0; i < csv.size(); ++i, ++k)
	{
		map[k] = new Node*[columns];
		for (size_t j = 1, n = 0; j < csv[i].size(); ++j, ++n)
		{
			map[k][n] = new Node();

			if (csv[i][j] == WATER)
				map[k][n]->cost = 2;
			else
				map[k][n]->cost = 1;

			map[k][n]->location = Point(k, n);
			map[k][n]->isPassable = csv[i][j] != WALL;
			printf("[%c]", csv[i][j]);
		}
		printf("\n");
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

size_t Maze::calculateHeuristics(Node* current, const Point& target)
{
	size_t targetCost = 0;
	int x = current->location.x;
	int y = current->location.y;

	int xDifference = target.x - x;
	int yDifference = target.y - y;

	while (yDifference && xDifference)
	{
		targetCost += map[x][y]->cost == 1 ? 15 : 20;
		x = xDifference > 0 ? x + 1 : x - 1;
		y = yDifference > 0 ? y + 1 : y - 1;


		xDifference = target.x - x;
		yDifference = target.y - y;
	}

	while (xDifference)
	{
		targetCost += map[x][y]->cost * 10;
		x = xDifference > 0 ? x + 1 : x - 1;
		xDifference = target.x - x;
	}

	while (yDifference)
	{
		targetCost += map[x][y]->cost * 10;
		y = yDifference > 0 ? y + 1 : y - 1;
		yDifference = target.y - y;
	}

	return targetCost;
}

void Maze::shortestPath()
{
	Node* current = map[startPosition.x][startPosition.y];
	open.push(current);
	current->g = calculateHeuristics(current, startPosition);
	current->h = calculateHeuristics(current, endPosition);

	while (true)
	{
		current = open.top();
		printf("Choosing (%i, %i), H: %i, G: %i\n", current->location.x, current->location.y, current->h, current->g);
		open.pop();
		if (open.size() && open.top()->f() == current->f() && open.top()->h < current->h)
		{
			current = open.top();
			open.pop();
		}
		closed.push_back(current->location);


		if (current->h == 0)
		{
			char matrix[7][13];
			while (current->parent)
			{
				matrix[current->location.x][current->location.y] = '*';
				printf("(%i, %i), ", current->location.x, current->location.y);
				current = current->parent;
			}
			printf("\n");
			for (int i = 0; i < rows; ++i)
			{
				printf("%i", i);
				for (int j = 0; j < columns; ++j)
					printf("[%c]", !map[i][j]->isPassable ? 'N' : (map[i][j]->cost == 2 ? WATER : (matrix[i][j] == '*' ? matrix[i][j] : ' ')));

				printf("\n");
			}


			return;
		}

		std::vector<Node*> neighbours = getNeighbours(current);
		printf("Got %i neighbours..\n", neighbours.size());
		for (Node* neighbour : neighbours)
		{
			if (!neighbour->isPassable || std::find(closed.begin(), closed.end(), neighbour->location) != closed.end())
				continue;

			if (std::find(visited.begin(), visited.end(), neighbour->location) != visited.end())
				continue;
			neighbour->g = calculateHeuristics(neighbour, startPosition);
			neighbour->h = calculateHeuristics(neighbour, endPosition);
			printf("neighbour pos (%i, %i), H: %i, G: %i\n", neighbour->location.x, neighbour->location.y, neighbour->h, neighbour->g);
			//system("pause");
			neighbour->parent = current;
			if (std::find(visited.begin(), visited.end(), neighbour->location) == visited.end())
			{
				open.push(neighbour);
				visited.push_back(neighbour->location);
			}
		}
	}
}
