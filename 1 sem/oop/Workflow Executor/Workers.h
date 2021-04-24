#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <sstream>
#include <algorithm>


class IWorker
{
private:
public:
	virtual std::vector<std::string> work(std::vector<std::string> text) = 0;
	virtual ~IWorker() {};
};

class Readfile : public IWorker
{
private:
	std::ifstream fin;
public:
	Readfile(std::string path);
	std::vector<std::string> work(std::vector<std::string> text) override;
	~Readfile() override;
};

class Writefile : public IWorker
{
private:
	std::ofstream fout;
public:
	Writefile(std::string path);
	~Writefile() override;
	std::vector<std::string> work(std::vector<std::string> text) override;
};


class Grep : public IWorker
{
private:
	std::string word;
public:
	std::vector<std::string> work(std::vector<std::string> text) override;
	Grep(std::string wordToGrep);
	~Grep() override;
};

class Sort : public IWorker
{
public:
	std::vector<std::string> work(std::vector<std::string> text) override;
	Sort();
	~Sort() override;
};

class Replace : public IWorker
{
private:
	std::string replacer;
	std::string replaceable;
public:
	std::vector<std::string> work(std::vector<std::string> text) override;
	Replace(std::string wordReplacer, std::string wordReplaceable);
	~Replace();
};

class Dump : public IWorker
{
private:
	std::ofstream fout;
public:
	Dump(std::string path);
	~Dump();
	std::vector<std::string> work(std::vector<std::string> text) override;
};
