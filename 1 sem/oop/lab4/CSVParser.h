#pragma once
#include <iostream>
#include <vector>
#include <tuple>
#include <fstream>
#include <string>
#include "tupleOstream.h"
#include <sstream>

using namespace std;

bool isNum(string str)
{
	bool res = false;
	for (int i = 0; i < str.size(); i++)
	{
		res = ((str[i] == '.') || (str[i] >= '0' && str[i] <= '9'));
	}
	if (res)
		res = str != ".";
	return res;
}

template<typename T>
void pushToTuple(string& str, T& elem)
{
	istringstream stream(str);
	try 
	{
		if (isNum(str))
		{
			stream >> elem;
		}
		else
		{
			throw invalid_argument("Not a number");
		}
	}
	catch (invalid_argument& problem)
	{
		cerr << problem.what() << endl;
	}
}

void pushToTuple(string& str, string& elem)
{
	elem = str;
}

void pushToTuple(string& str, char& elem)
{
	if (str.size() == 1)
	{
		elem = str[0];
	}
	else
	{
		cerr << str << "Not a char" << endl;
	}
}

void pushToTuple(string& str, bool& elem)
{
	try 
	{
		if (str == (string)"true")
		{
			elem = true;
		}
		else if (str == (string)"false")
		{
			elem = false;
		}
		else if (isNum(str))
		{
			throw invalid_argument("Not a bool");
		}
	}
	catch (invalid_argument& problem)
	{
		cerr << problem.what() << endl;
	}
}

template <typename Tuple, int index>
typename enable_if<(index == 0), void>::type getTuple(Tuple& t, vector<string> words) {}

template <typename Tuple, int index>
typename enable_if<(index != 0), void>::type getTuple(Tuple& t, vector<string> words)
{
	pushToTuple(words[index - 1], get<index - 1>(t));
	getTuple<Tuple, index - 1>(t, words);
}

template <class... Args>
class CSVParser
{
private:
	char colSplit;
	char lineSplit;
	char screeningSym;
	bool screening = false;
	int linesToSkip;
	vector<tuple<Args...>> data;

	vector<string> getWords(string line)
	{
 		vector<string> words;
		bool screenFlag = false; 
		int wordStartIndex = 0;
		int len = line.size();
		for (int i = 0; i < len; i++)
		{
			if (line[i] == screeningSym)
			{
				line.erase(line.begin() + i); //Удаляем символ экранирования
				if (line[i] != screeningSym) 
				{
					screening = !screening;
					i--;
					len--;
				}
			}
			else if ((line[i] == colSplit) && (!screening))
			{
				words.push_back(line.substr(wordStartIndex, i- wordStartIndex));
				wordStartIndex = i + 1;
			}
		}
		if (wordStartIndex != words.size()) 
		{
			words.push_back(line.substr(wordStartIndex, line.size()));
		}
		return words;
	}
	void readLine(ifstream& in, string& line)
	{
		if (lineSplit == '\n')
		{
			getline(in, line);
			return;
		}
		line.clear();
		char c;
		in >> c;
		while (c != lineSplit && !in.eof())
		{
			line.push_back(c);
			in >> c;
		}

	}
public:
	void printData()
	{
		for (int i = 0; i < data.size(); i++)
		{
			cout << data[i] << endl;
		}
	}
	CSVParser(string filename, char col = ';', char line = '\n', char screen = '\"', int linesToSkip = 0)
	{
		colSplit = col; lineSplit = line; screeningSym = screen;
		try 
		{
			ifstream input(filename);
			//input.open(filename);
			if (!input.is_open())
			{
				throw invalid_argument("Can't open input file");
			}
			string line;
			for (int i = 0; i < linesToSkip; i++)
			{
				if (input.eof())
				{
					throw invalid_argument("Input file finished, but there're lines to skip");
				}
				readLine(input, line);
			}
			vector<string> words;
			while (!input.eof())
			{
				readLine(input, line);
				words = getWords(line);
				if (words.size() != sizeof...(Args))
				{
					throw invalid_argument("Count of element in string and size of tuple doesn't match");
				}
				tuple<Args...> t;
				getTuple<decltype(t), sizeof...(Args)>(t, words);
				data.push_back(t);
			}

		}
		catch (invalid_argument& problem)
		{
			cerr << problem.what() << endl;
			exit(1);
		}
	}

	class Iterator
	{
	private:
		vector <tuple<Args...>>* ptr;
		int index;
	public:
		Iterator(int ind, vector <tuple<Args...>>* p) 
		{
			index = ind;
			ptr = p;
		}
		bool operator!=(const Iterator& it) const
		{
			return !(it == *this);
		}
		bool operator==(const Iterator& it) const
		{
			return ((it.ptr == ptr) && (it.index == index));
		}
		const tuple<Args...>& operator*() const
		{
			return (*ptr)[index];
		}
		Iterator& operator++(int)
		{
			index++;
			return *this;
		}
		Iterator& operator++()
		{
			++index;
			return *this;
		}
		tuple<Args...> at()
		{
			return (*ptr)[index];
		}
	};
	Iterator begin()
	{
		return Iterator(0, &data);
	}
	Iterator end()
	{
		return Iterator(data.size(), &data);
	}
	

};