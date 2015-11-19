#ifndef NODE_H
#define NODE_H

struct Node
{
	// Distance from starting node
	size_t g;
	// Distance to target node
	size_t h;
	size_t f;

	size_t cost;
	bool isPassable;

	Node* parent;
};


#endif // NODE_H
