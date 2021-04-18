#include "Workers.h"


Readfile::Readfile(std::string path)
{
	try
	{
		fin.open(path);
		if (!fin.is_open())
		{
			throw std::invalid_argument("Can't open input file at Readfile block");
		}
	}
	catch (std::invalid_argument& problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
}

std::vector<std::string> Readfile::work(std::vector<std::string> text)
{
	std::string line;
	while (!fin.eof())
	{
		std::getline(fin, line);
		text.emplace_back(line);
	}
	return text;
}

Readfile::~Readfile() { fin.close(); }

Writefile::Writefile(std::string path)
{
	try
	{
		fout.open(path);
		if (!fout.is_open())
			throw std::invalid_argument("Can't open output file at Writefile block");
	}
	catch (std::invalid_argument& problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
}

std::vector<std::string> Writefile::work(std::vector<std::string> text)
{
	std::string line;
	std::vector<std::string>::iterator cur = text.begin();
	for (; cur < text.end(); ++cur)
	{
		fout << *cur << std::endl;
	}
	return text;
}

Writefile::~Writefile() { fout.close(); }

Grep::Grep(std::string wordToGrep)
{
	word = wordToGrep;
}

std::vector<std::string> Grep::work(std::vector<std::string> text)
{
	int len = text.size();
	std::string curWord;
	std::vector<std::string> choosenStrings;
	for (int i = 0; i < len; ++i)
	{
		std::istringstream line(text[i]);
		while (!line.eof()) 
		{
			line >> curWord;
			if (curWord == this->word)
			{
				choosenStrings.emplace_back(text[i]);
				break;
			}
		}
	}
	return choosenStrings;
}

Grep::~Grep() {}

Sort::Sort()  {}
Sort::~Sort() {}

std::vector<std::string> Sort::work(std::vector<std::string> text)
{
	std::sort(text.begin(), text.end());
	return text;
}

Replace::Replace(std::string wordReplacer, std::string wordReplaceable)
{
	replacer = wordReplacer;
	replaceable = wordReplaceable;
}

std::vector<std::string> Replace::work(std::vector<std::string> text)
{
	int len = text.size();
	for (int i = 0; i < len; ++i)
	{
		std::istringstream stream(text[i]);
		std::string newString;
		while (!stream.eof())
		{
			std::string word;
			stream >> word;
			if (word == replaceable)
			{
				newString += replacer + ' ';
			}
			else
			{		
				newString += word + ' ';
			}
		}
		newString.pop_back(); //Удаляем последний пробел
		text[i] = newString;
	}
	return text;
}

Replace::~Replace(){}

Dump::Dump(std::string path)
{
	try
	{
		fout.open(path);
		if (!fout.is_open())
		{
			throw std::invalid_argument("Can't open dump file at Dump");
		}
	}
	catch(std::invalid_argument &problem)
	{
		std::cerr << problem.what() << std::endl;
		exit(1);
	}
}

std::vector<std::string> Dump::work(std::vector<std::string> text)
{
	int len = text.size();
	for (int i = 0; i < len; ++i)
	{
		fout << text[i] << std::endl;
	}
	return text;
}

Dump::~Dump() { fout.close(); }