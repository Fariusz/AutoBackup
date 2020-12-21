#include "TaskExecutor.h"

using namespace std;

void TaskExecutor::execute(std::vector<BackupProperties> tasks)
{
	// TODO implement 
	ofstream temp("temp.temp");

	for(BackupProperties t : tasks)
	{
		temp << t.srcDir << " -> " << t.destDir << " kompresja:" << t.compress << endl;
	}
	temp.close();
}
