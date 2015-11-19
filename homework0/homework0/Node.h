#ifndef NODE_H
#define NODE_H

struct Node
{
	// Distance from starting node
	size_t g;
	// Distance to target node
	size_t h;
	size_t f() const { return g + h; }

	size_t cost;
	bool isPassable;
	Point location;

	Node* parent;

};


#endif // NODE_H
