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
		{
			delete map[i][j];
			map[i][j] = NULL;
		}

		delete[] map[i];
		map[i] = NULL;
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
	printf("Allocating %d rows..\n", rows);
	printf("Allocating %d columns per row..\n", columns);

	for (size_t i = 0; i < rows; ++i)
	{
		map[i] = new Node*[columns];
		for (size_t j = 0; j < columns; ++j)
		{
			map[i][j] = new Node();

			if (csv[i][j] == WATER)
				map[i][j]->cost = 2;
			else
				map[i][j]->cost = 1;

			map[i][j]->location = Point(i, j);
			map[i][j]->isPassable = csv[i][j] != WALL;
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

size_t Maze::calculateHeuristics(Node* current) const
{
	size_t targetCost = 0;
	int x = current->location.x;
	int y = current->location.y;

	int xDifference = endPosition.x - x;
	int yDifference = endPosition.y - y;

	while (yDifference && xDifference)
	{
		targetCost += map[x][y]->cost == 1 ? 15 : 20;
		x = xDifference > 0 ? x + 1 : x - 1;
		y = yDifference > 0 ? y + 1 : y - 1;


		xDifference = endPosition.x - x;
		yDifference = endPosition.y - y;
	}

	while (xDifference)
	{
		targetCost += map[x][y]->cost * 10;
		x = xDifference > 0 ? x + 1 : x - 1;
		xDifference = endPosition.x - x;
	}

	while (yDifference)
	{
		targetCost += map[x][y]->cost * 10;
		y = yDifference > 0 ? y + 1 : y - 1;
		yDifference = endPosition.y - y;
	}

	return targetCost;
}

size_t Maze::calculate(Node* current, Point point) const
{
	size_t& targetCost = point == startPosition ? current->g : current->h;
	int x = current->location.x;
	int y = current->location.y;

	int xDifference = point.x - x;
	int yDifference = point.y - y;

	while (yDifference && xDifference)
	{
		targetCost += map[x][y]->cost == 1 ? 15 : 20;
		x = xDifference > 0 ? x + 1 : x - 1;
		y = yDifference > 0 ? y + 1 : y - 1;


		xDifference = point.x - x;
		yDifference = point.y - y;
	}

	while (xDifference)
	{
		targetCost += map[x][y]->cost * 10;
		x = xDifference > 0 ? x + 1 : x - 1;
		xDifference = point.x - x;
	}

	while (yDifference)
	{
		targetCost += map[x][y]->cost * 10;
		y = yDifference > 0 ? y + 1 : y - 1;
		yDifference = point.y - y;
	}

	return targetCost;
}

size_t Maze::getCostFromStart(const Point& parentLocation, const Point& currentLocation) const
{
	int xDifference = currentLocation.x - parentLocation.x;
	int yDifference = currentLocation.y - parentLocation.y;

	if (xDifference && yDifference)
		return 15;

	return 10;
}

void Maze::shortestPath()
{
	Node* current = map[startPosition.x][startPosition.y];
	current->h = calculateHeuristics(current);
	open.push(current);
	visited.push_back(current->location);
	std::ofstream hnadle("log_g.txt", std::ios::out);

	while (true)
	{
		current = open.pop();
		printf("Choosing (%i, %i), H: %i, G: %i\n", current->location.x, current->location.y, current->h, current->g);
		//open.pop();
		visited.erase(std::find(visited.begin(), visited.end(), current->location));
		//if (open.size() && top()->f() == current->f() && top()->h < current->h)
		//	continue;

		closed.push_back(current->location);


		if (current->h == 0)
		{
			char matrix[8][13]{ ' ' };
			matrix[startPosition.x][startPosition.y] = 'M';
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
					printf("[%c]", !map[i][j]->isPassable ? 'N' : (map[i][j]->cost == 2 ? WATER : (matrix[i][j] == '*' ? matrix[i][j] : matrix[i][j])));

				printf("\n");
			}


			return;
		}

		std::vector<Node*> neighbours = getNeighbours(current);
		printf("Got %i neighbours..\n", neighbours.size());
		for (Node* neighbour : neighbours)
		{
			if (!neighbour->isPassable)
				printf("(%i, %i) not passable\n", neighbour->location.x, neighbour->location.y);
			if (std::find(closed.begin(), closed.end(), neighbour->location) != closed.end())
				printf("(%i, %i) already closed\n", neighbour->location.x, neighbour->location.y);
			if (!neighbour->isPassable || std::find(closed.begin(), closed.end(), neighbour->location) != closed.end())
				continue;

			size_t newG = current->cost == 2 ? 20 : getCostFromStart(current->location, neighbour->location);
			newG += current->g;
			if (neighbour->g == 0 || newG <= neighbour->g)
			{
				//Node* del = neighbour;
				//neighbour = new Node(*del);
				//map[neighbour->location.x][neighbour->location.y] = neighbour;// new Node(*neighbour);

				neighbour->parent = current;
				neighbour->g = newG;
				hnadle << "(" << neighbour->location.x << ", " << neighbour->location.y << ") - G: " << neighbour->g << "\n";

				neighbour->h = calculateHeuristics(neighbour);

				printf("neighbour pos (%i, %i), H: %i, G: %i\n", neighbour->location.x, neighbour->location.y, neighbour->h, neighbour->g);
				//system("pause");


					open.push(neighbour);
					visited.push_back(neighbour->location);
				
			}
			else
				printf("Something's wrong with (%i, %i), H: %i, G: %i (%i)\n", neighbour->location.x, neighbour->location.y, neighbour->h, neighbour->g, newG);
		}
	}
	hnadle.close();
}
