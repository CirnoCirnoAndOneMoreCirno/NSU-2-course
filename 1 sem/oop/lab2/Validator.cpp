#include "Validator.h"

IOType Validator::kindOfInput(IWorker* block)
{
	if (dynamic_cast<Readfile*>(block))
		return IONothing;
	return IOText;
}

IOType Validator::kindOfOutput(IWorker* block)
{
	if (dynamic_cast<Writefile*>(block))
		return IONothing;
	return IOText;
}

void Validator::checkExistance(std::map<int, IWorker*>* blocks, std::vector<int>* order)
{
	try
	{
		int len = order->size();
		int idx;
		for (int i = 0; i < len; ++i)
		{
			idx = (*order)[i];
			if (blocks->count(idx) == 0)
				throw std::invalid_argument("No block with such id");
		}
	}
	catch (std::invalid_argument& problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
}

void Validator::checkEmptiness(std::map<int, IWorker*> *blocks, std::vector<int> *order)
{
	try 
	{
		if (blocks->empty())
			throw std::invalid_argument("No blocks in workflow");
		if (order->empty())
			throw std::invalid_argument("Empty order in workflow");
	}
	catch (std::invalid_argument& problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
}

void Validator::checkInputOutputOfBlocks(std::map<int, IWorker*> *blocks, std::vector<int> *order, bool commandLineInput, bool commandLineOutput)
{
	try 
	{
		if (!commandLineInput && !dynamic_cast<Readfile*>((*blocks)[(*order)[0]]))
		{
			throw std::invalid_argument("No input file at begin of file or in command line");
		}
		int lastIdxInOrder = (*order)[order->size() - 1];
		if (!commandLineOutput && !dynamic_cast<Writefile*>((*blocks)[lastIdxInOrder]))
		{
			throw std::invalid_argument("No output file at end of file or in command line");
		}
		IOType Ot = IOText; //Первый блок всегда Readfile, он возвращает текст
		IOType It;
		int len = order->size();
		for (int i = 1; i < len; i++)
		{
			int idx = (*order)[i];
			It = kindOfInput((*blocks)[idx]);
			if (It != Ot)
			{
				throw std::invalid_argument("Input and output don't match");
			}
			Ot = kindOfOutput((*blocks)[idx]);
		}
	}
	catch (std::invalid_argument& problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
}


void Validator::checkValid(std::map<int, IWorker*> *blocks, std::vector<int> *order, bool commandLineInput, bool commandLineOutput)
{
	checkExistance(blocks, order);
	checkEmptiness(blocks, order);
	checkInputOutputOfBlocks(blocks, order, commandLineInput, commandLineOutput);
}
