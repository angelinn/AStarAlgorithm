#ifndef PQUEUE_H
#define PQUEUE_H

#include "Node.h"
#include <unordered_set>

class PQueue
{
public:
	PQueue();

public:
	const Node* top() const;
	Node* pop();
	void push(Node *);
	bool exists(const Node *) const;

private:
	PQueue(const PQueue &) = delete;
	PQueue& operator=(const PQueue &) = delete;

private:
	std::unordered_set<Node *> front;
};

#endif // PQUEUE_H
