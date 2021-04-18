#include <iostream>
#include <fstream>
#include <string>
#include "executor.h"


int main(int argc, char **argv)
{
	std::ifstream workflow;
	std::string inputPath;
	std::string outputPath;
	bool withCommandlineIn = false;
	bool withCommandlineOut = false;

	try 
	{
		if (argc < 2)
			throw std::invalid_argument("Too few arguments");

		workflow.open(argv[1]);
		if (!workflow.is_open())
			throw std::invalid_argument("Can't open workflow file");

		if ((argc == 3) || (argc == 5))
			throw std::invalid_argument("Wrong count of arguments!");

		for (int i = 2; i < argc; i++)
		{
			if ((std::string)argv[i] == "-i")
			{
				if (i == argc - 1)
					throw std::invalid_argument("No input file at arguments");
				withCommandlineIn = true;
				inputPath = argv[i + 1];
			}
			else if ((std::string)argv[i] == "-o")
			{
				if (i == argc - 1)
					throw std::invalid_argument("No output file at arguments");
				 withCommandlineOut = true;
				 outputPath = argv[i + 1];
			}
		}
		

	}
	catch (std::invalid_argument& problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
	
	std::map<int, IWorker*> *blocks = Parser::parseBlocks(workflow);
	std::vector<int> *order = Parser::getOrder(workflow);
	Validator::checkValid(blocks, order, withCommandlineIn, withCommandlineOut);
	Executor::execute(blocks, order, withCommandlineIn, withCommandlineOut, inputPath, outputPath);
	return 0;
}

//TODO:
