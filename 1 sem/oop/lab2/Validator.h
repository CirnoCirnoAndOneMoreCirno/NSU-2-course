#pragma once
#include "executor.h"

enum IOType  { IONothing = 0, IOText = 1 };

class Validator
{
private:
	static void checkExistance(std::map<int, IWorker*>* blocks, std::vector<int>* order);
	static void checkEmptiness(std::map<int, IWorker*> *blocks, std::vector<int> *order);
	static void checkInputOutputOfBlocks(std::map<int, IWorker*> *blocks, std::vector<int> *order, bool commandLineInput, bool commandLineOutput);
	static IOType kindOfOutput(IWorker* block); 
	static IOType kindOfInput(IWorker* block);
public:	
	static void checkValid(std::map<int, IWorker*> *blocks, std::vector<int> *order, bool commandLineInput, bool commandLineOutput);
};

