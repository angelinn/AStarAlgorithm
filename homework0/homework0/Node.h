#ifndef NODE_H
#define NODE_H

struct Node
{
public:
	Node() : g(0), h(0), cost(0), isPassable(false), parent(NULL)
	{  }

public:
	size_t f() const { return g + h; }

public:
	// Distance from starting node
	size_t g;
	// Distance to target node
	size_t h;

public:
	bool isPassable;
	Point location;
	size_t cost;

	Node* parent;
};


#endif // NODE_H
