#include <stdio.h>
#include <string>

#include "Maze.h"

const int TARGET_ARGC = 5;

int main(int argc, const char* argv[])
{

	//if (argc != TARGET_ARGC)
	//{
	//	printf("Error: Incorrect number of arguments.\n");
	//	return -1;
	//}


	//std::string csvFile(argv[0]);
	//Point start { atoi(argv[1]), atoi(argv[2]) };
	//Point food { atoi(argv[3]), atoi(argv[4]) };

	std::string csvFile("C:\\Users\\Betra\\Desktop\\map.csv");
	Point start{ 6, 0 };
	Point end{ 5, 7 };

	Maze maze(start, end);
	maze.loadCsv(csvFile);

	return 0;
}