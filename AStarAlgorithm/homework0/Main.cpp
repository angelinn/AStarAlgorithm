/*******************************************************************************************************

										    A * Algorithm
										  Angelin Nedelchev
										Faculty Number: 71497

									  Coded using Visual Studio 2015

								  NOTES: CSV file must not end in new line

********************************************************************************************************/



#include <stdio.h>
#include <string>

#include "Maze.h"

const int TARGET_ARGC = 5;

int main(int argc, const char* argv[])
{
	if (argc != TARGET_ARGC + 1)
	{
		printf("Error: Incorrect number of arguments.\n");
		return -1;
	}


	std::string csvFile(argv[1]);
	Point start { atoi(argv[2]), atoi(argv[3]) };
	Point end { atoi(argv[4]), atoi(argv[5]) };

	try
	{
		Maze maze(start, end);
		maze.loadCsv(csvFile);

		std::vector<std::string> coordinates = maze.shortestPath();
		
		printf("\n\n");

		for (std::string string : coordinates)
			printf("%s ", string.c_str());

		printf("\n");

		return 0;
	}
	catch (std::exception& e)
	{
		printf("%s\n", e.what());
		return -3;
	}
}