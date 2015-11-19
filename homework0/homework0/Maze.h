#ifndef MAZE_H
#define MAZE_H

const char WHITE_SPACE = ' ';
const char WATER = '~';
const char PLAYER = 'M';
const char TARGET = 'X';
const char WALL = 'N';

#include <vector>
#include "Point.h"

struct Node;

class Maze
{
public:
	Maze(const std::string &, const Point &, const Point &);
	~Maze();

public:
	void shortestPath(Node *);

private:
	Maze(const Maze &) = delete;
	Maze& operator=(const Maze &) = delete;

private:
	std::string readCsv(const std::string &) const;
	void parseCsv(const std::vector<std::string> &);
	std::vector<std::string> split(const std::string &, char = '\n') const;

private:
	Point startPosition;
	Point endPosition;

private:
	Node** map;
	std::vector<Node*> open;
	std::vector<Node*> closed;
};


#endif // MAZE_H
