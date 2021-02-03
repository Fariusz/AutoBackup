#pragma once
#include <vector>
#include "../AutoBackupConsole/BackupProperties.h"
#include <fstream>
#include <windows.h>

class TaskExecutor
{
public:
	void execute(std::vector<BackupProperties> tasks);
	static void DoBackup(std::string source, std::string destination, bool compress);
	void ErrorHandler(LPTSTR lpszFunction);
};

