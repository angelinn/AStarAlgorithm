#ifndef MAZE_H
#define MAZE_H

const char WHITE_SPACE = ' ';
const char WATER = '~';
const char PLAYER = 'M';
const char TARGET = 'X';
const char WALL = 'N';

#include <vector>
#include <queue>

#include "Point.h"
#include "Node.h"

struct LessThanByFCost
{
	bool operator()(const Node* lhs, const Node* rhs) const
	{
		return lhs->f() > rhs->f();
	}
};

struct Node;

class Maze
{
public:
	Maze(const Point &, const Point &);
	~Maze();

public:
	void shortestPath();
	void loadCsv(const std::string &);

private:
	Maze(const Maze &) = delete;
	Maze& operator=(const Maze &) = delete;

private:
	std::vector<Node*> getNeighbours(const Node *);
	size_t calculateHeuristics(Node *) const;
	size_t calculate(Node *, Point) const;
	size_t getCostFromStart(const Point &, const Point &) const;
	size_t calculateStepsFromStart(Node *) const;

private:
	void parseCsv(const std::vector<std::string> &);

private:
	Point startPosition;
	Point endPosition;

	size_t rows;
	size_t columns;

private:
	Node*** map;
	std::priority_queue<Node*, std::vector<Node*>, LessThanByFCost> open;
	std::vector<Point> closed;
	std::vector<Point> visited;
};

#endif // MAZE_H
