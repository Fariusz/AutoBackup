#include <iostream>
#include "TaskLoader.h"
#include "TaskExecutor.h"
#include <vector>
#include <windows.h>

using namespace std;
int main()
{
    Logger logger("log.log");
    TaskLoader taskLoader("schedule.dat", &logger);
    TaskExecutor taskExecutor;

    while (true)
    {
        vector<BackupProperties> tasks = taskLoader.loadTasks();
        taskExecutor.execute(tasks);
        Sleep(30000);
    }
}
