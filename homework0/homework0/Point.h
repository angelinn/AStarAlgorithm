#ifndef POINT_H
#define POINT_H

struct Point
{
	Point() : Point(0, 0)
	{  }

	Point(int x, int y) : x(x), y(y)
	{  }

	int x;
	int y;
};

#endif // POINT_H
