#include "executor.h"

void Executor::execute(std::map<int, IWorker*>* blocks, std::vector<int>* order, 
	bool commandlineI, bool commandlineO, std::string IPath, std::string OPath)
{
	int len = (*order).size();
	std::vector<std::string> text;
	if (commandlineI)
	{
		text = (new Readfile(IPath))->work(text);
	}
	for (int i = 0; i < len; ++i)
	{
		text = (*blocks)[(*order)[i]]->work(text);
	}
	if (commandlineO)
	{
		text = (new Writefile(OPath))->work(text);
	}
}