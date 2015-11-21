#ifndef PQUEUE_H
#define PQUEUE_H

#include "Node.h"
#include <set>

class PQueue
{
public:
	PQueue();

public:
	const Node* top() const;
	Node* pop();
	void push(Node *);

private:
	PQueue(const PQueue &) = delete;
	PQueue& operator=(const PQueue &) = delete;

private:
	std::set<Node*> front;
};

#endif // PQUEUE_H
