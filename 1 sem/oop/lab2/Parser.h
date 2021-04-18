#pragma once
#include "Workers.h"
#include <map>

bool isNumber(std::string str);

class Parser
{
public:
	static std::map<int, IWorker*>* parseBlocks(std::ifstream &fin);
	static std::vector<int>* getOrder(std::ifstream &fin);
};

