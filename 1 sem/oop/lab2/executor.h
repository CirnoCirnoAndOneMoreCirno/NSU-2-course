#pragma once
#include "Parser.h"
#include "Validator.h"

class Executor
{
public:
	static void execute(std::map<int, IWorker*> *blocks, std::vector<int> *order, bool commandlineI, bool commandlineO, std::string IPath, std::string OPath);
};

