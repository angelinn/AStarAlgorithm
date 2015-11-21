/*******************************************************************************************************

										    A * Algorithm
										  Angelin Nedelchev
										Faculty Number: 71497

									  Coded using Visual Studio 2015

								  NOTES: CSV file must not end in new line

********************************************************************************************************/



#include <stdio.h>
#include <string>
#include <crtdbg.h>

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
	{
		std::string csvFile("C:\\Users\\Betra\\Desktop\\map1.csv");
		Point start{ 5, 0 };
		Point end{ 4, 7 };

		Maze maze(start, end);
		maze.loadCsv(csvFile);
		auto s = maze.shortestPath();

		for (auto string : s)
			printf("%s ", string.c_str());
	}
	_CrtDumpMemoryLeaks();
	return 0;
}