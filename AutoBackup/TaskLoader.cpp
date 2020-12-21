#include "TaskLoader.h"
#include <Windows.h>
#include <taskschd.h>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "../AutoBackupConsole/Files.h"
#include "../AutoBackupConsole/BackupProperties.h"
#include "Time.h"

using namespace std;

std::vector<BackupProperties> TaskLoader::loadTasks()
{
	_logger->info("Wczytywanie zadañ");
	_tasks.clear();

	char tempName[L_tmpnam];
	tmpnam_s(tempName);

	fstream schedulerFile(_scheduleFileName);
	ofstream temp(tempName);
	string line;
	while (getline(schedulerFile, line))
	{
		try
		{
			BackupProperties backup = readTaskFromLine(line);
			if (isAwaiting(backup))
			{
				_tasks.push_back(backup);
				backup = prepareForNextTime(backup);
				line = backupprops::to_string(backup);
			}
			temp << line << endl;
		}
		catch (exception e)
		{
			cerr << "Incorrect task definition. Skipped: " << line;
		}
	}
	schedulerFile.close();
	temp.close();

	int result1 = remove(_scheduleFileName.c_str());
	int result2 = rename(tempName, _scheduleFileName.c_str());

	return _tasks;
}

BackupProperties TaskLoader::readTaskFromLine(std::string line)
{
	BackupProperties task;
	istringstream iss(line);
	iss >> task.firstTime.date
		>> task.firstTime.time
		>> task.interval.days
		>> task.interval.hoursMins
		>> task.srcDir
		>> task.destDir
		>> task.compress;
	return task;
}

bool TaskLoader::isAwaiting(BackupProperties backup)
{
	using namespace std;
	auto now = std::chrono::system_clock::now();
	time_t now_time_t = std::chrono::system_clock::to_time_t(now);

	struct tm timeInfo;
	localtime_s(&timeInfo, &now_time_t);


	string date = backup.firstTime.date;
	string time = backup.firstTime.time;

	int day = stoi(date.substr(0, 2));
	int mon = stoi(date.substr(3, 2));
	int year = stoi(date.substr(6, 4));
	int hour = stoi(time.substr(0, 2));
	int min = stoi(time.substr(3, 2));
	int sec = stoi(time.substr(6, 2));

	timeInfo.tm_year = year - 1900;
	timeInfo.tm_mon = mon - 1;
	timeInfo.tm_mday = day;
	timeInfo.tm_hour = hour;
	timeInfo.tm_min = min;
	timeInfo.tm_sec = sec;

	time_t taskTime_time_t = mktime(&timeInfo);

	bool result = difftime(now_time_t, taskTime_time_t) >= 0;

	return result;
}

BackupProperties TaskLoader::prepareForNextTime(BackupProperties& backup)
{
	string sIntervalDays = backup.interval.days.substr(0, 2);
	string sIntervalHours = backup.interval.hoursMins.substr(0, 2);
	string sIntervalMins = backup.interval.hoursMins.substr(3, 2);

	string sPrevHours = backup.firstTime.time.substr(0, 2);
	string sPrevMins = backup.firstTime.time.substr(3, 2);

	int iDays, iHours, iMins, iPrevHours, iPrevMins;

	iDays = stoi(sIntervalDays);
	iHours = stoi(sIntervalHours);
	iMins = stoi(sIntervalMins);
	iPrevHours = stoi(sPrevHours);
	iPrevMins = stoi(sPrevMins);

	iHours += iPrevHours;
	iMins += iPrevMins;

	iHours += iMins / 60;
	iMins %= 60;
	iDays += iHours / 24;
	iHours %= 24;

	backup.firstTime.date = timeutils::addDaysToDate(backup.firstTime.date, iDays);

	ostringstream time;
	if (iHours < 10)
		time << "0";
	time << iHours << ":";
	if (iMins < 10)
		time << "0";
	time << iMins << ":00";
	
	backup.firstTime.time = time.str();
	return backup;
}
