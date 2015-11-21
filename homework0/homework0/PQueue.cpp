#include "PQueue.h"

PQueue::PQueue()
{  }

Node* PQueue::pop()
{
	Node* min = const_cast<Node*>(top());
	front.erase(min);
	return min;
}

const Node* PQueue::top() const
{
	Node* min = NULL;
	size_t priority = INT_MAX;

	for (auto iter : front)
	{
		if (iter->f() < priority)
		{
			priority = iter->f();
			min = iter;
		}
	}
	return min;
}

void PQueue::push(Node* node)
{
	front.insert(node);
}