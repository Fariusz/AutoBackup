#pragma once
#include <vector>
#include "../AutoBackupConsole/BackupProperties.h"
#include <fstream>
class TaskExecutor
{
public:
	void execute(std::vector<BackupProperties> tasks);
};

