#include "Parser.h"

bool isNumber(std::string str)
{
	int len = str.length();
	for (int i = 0; i < len; ++i)
	{
		if (!isdigit(str[i]))
			return false;
	}
	return true;
}


std::map<int, IWorker*>* Parser::parseBlocks(std::ifstream &fin)
{

	std::map<int, IWorker*> *mp = new std::map<int, IWorker*>;
	std::string str, blockName;
	int idBlock;
	char ch;
	try 
	{
		std::getline(fin, str);
		if (str != "desc")
			throw std::invalid_argument("Wrong format of workflow file, no \"desc\"");

		std::getline(fin, str);

		while (str != "csed")
		{			
			std::istringstream stream(str);

			stream >> idBlock;
			if (mp->count(idBlock) > 0)
			{
				throw std::invalid_argument("Block is already exists");
			}
			
			stream >> ch; // '='

			if (ch != '=')
				throw std::invalid_argument("Wrong format of workflow file, no '=' char between number and name of block");
			stream >> blockName;

			if (blockName == "grep")
			{
				std::string word;
				if (stream.eof())
					throw std::invalid_argument("No word to grep");
				stream >> word;
				(*mp)[idBlock] = new Grep(word);

			}
			else if (blockName == "sort")
			{
				(*mp)[idBlock] = new Sort();
			}
			else if (blockName == "replace")
			{
				std::string replaceable = "", replacer = "";
				stream >> replaceable >> replacer;
				if (replaceable == "")
					throw std::invalid_argument("No replaceable word");
				//replacer может быть и пустым, чтобы удалить слово в тексте
				(*mp)[idBlock] = new Replace(replacer, replaceable);
			}
			else if (blockName == "dump")
			{
				std::string filename;
				if (stream.eof())
					throw std::invalid_argument("No dump filename");
				stream >> filename;
				(*mp)[idBlock] = new Dump(filename);
			}
			else if (blockName == "readfile")
			{
				std::string filename;
				if (stream.eof())
					throw std::invalid_argument("No filename at readfile");
				stream >> filename;
				(*mp)[idBlock] = new Readfile(filename);
			}
			else if (blockName == "writefile")
			{
				std::string filename;
				if (stream.eof())
					throw std::invalid_argument("No filename at writefile");
				stream >> filename;
				(*mp)[idBlock] = new Writefile(filename);
			}
			else
				throw std::invalid_argument("Unknown command");
			if (fin.eof())
				throw std::invalid_argument("Wrong format of workflow file, no \"csed\"");
			std::getline(fin, str);
		}
	}
	catch (std::invalid_argument &problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}

	return mp;
}

std::vector<int>* Parser::getOrder(std::ifstream &fin)
{
	std::vector<int> *order = new std::vector<int>;
	std::string orderLine, str;
	std::getline(fin, orderLine);
	std::istringstream stream(orderLine);
	int counter = 0;
	try 
	{
		while (!stream.eof())
		{
			stream >> str;
			if (str != "->")
			{
				if (counter % 2 == 1)
					throw std::invalid_argument("No \'->\' between operators");
				if (!isNumber(str))
					throw std::invalid_argument("Not a number in order");
				(*order).emplace_back(std::stoi(str));
			}
			counter++;
		}
	}
	catch (std::invalid_argument& problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
	return order;
}