#ifndef MAZE_H
#define MAZE_H

#include <vector>

#include "PQueue.h"
#include "Point.h"

struct Node;

class Maze
{
public:
	Maze(const Point &, const Point &);
	~Maze();

public:
	std::vector<std::string> shortestPath();

public:
	void printPath(const Node *) const;
	void loadCsv(const std::string &);

private:
	Maze(const Maze &) = delete;
	Maze& operator=(const Maze &) = delete;

private:
	std::vector<Node*> getNeighbours(const Node *);
	size_t calculateHeuristics(Node *) const;
	size_t getCostFromStart(const Point &, const Point &) const;

private:
	void parseCsv(const std::vector<std::string> &);

private:
	Point startPosition;
	Point endPosition;

	size_t rows;
	size_t columns;

private:
	Node*** map;
	
	PQueue open;
	std::vector<Point> closed;
};

#endif // MAZE_H
