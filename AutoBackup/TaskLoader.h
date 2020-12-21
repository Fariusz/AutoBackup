#pragma once
#include <string>
#include <map>
#include <vector>
#include "../AutoBackupConsole/BackupProperties.h"
#include "Logger.h"

class TaskLoader
{
private:
	std::string _scheduleFileName;
	std::vector<BackupProperties> _tasks;
	Logger* _logger;

public:
	TaskLoader(std::string schedulerFileName, Logger* logger)
		: _scheduleFileName(schedulerFileName), _logger(logger)
	{}
	std::vector<BackupProperties> loadTasks();

private:
	bool isAwaiting(BackupProperties backup);
	BackupProperties prepareForNextTime(BackupProperties& backup);
	BackupProperties readTaskFromLine(std::string line);
};