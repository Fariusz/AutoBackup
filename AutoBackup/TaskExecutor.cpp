#include "TaskExecutor.h"
#include "TimeUtils.h"

using namespace std;

void TaskExecutor::execute(std::vector<BackupProperties> tasks)
{
	// TODO implement 
	ofstream temp("temp.temp", ios::app);

	for(BackupProperties t : tasks)
	{
		temp << t.srcDir << " -> "
			<< t.destDir << " kompresja:"
			<< t.compress << timeutils::now()
			<< endl;
	}
	temp.close();
}