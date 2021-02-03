#pragma once
#include <string>
#include<vector>
#include "BackupProperties.h"

enum class Command
{
	Status,
	Start,
	Stop,
	NewTask,
	ClearTasks,
	ShowSchedule,
	Shutdown
};

class Dialog
{
private:
	struct canceledException {};
	const std::string SCHEDULE_FILE_NAME = "schedule.dat";
public:
	Command showMainDialog();
private:
	void refreshConsole();
	void printMainMenu();
	void printTasks(std::vector<BackupProperties>& tasks);
	int readOption();
	void createNewBackupTask();
	void saveTask(const BackupProperties& backup);
	void clearTasks(); //Usuwa zadania z harmonogramu
	void showStatus();
	void showMessage(std::string text);
	void startAutoBackupProcess();
	void stopAutoBackupProcess();
	void showSchedule();
	void pause();
	std::vector<BackupProperties> loadTasksFromFile(std::string fileName);
	BackupProperties showNewTaskDialog();
	std::string showDateDialog();
	std::string showTimeDialog();
	BackupInterval showIntervalDialog();
	std::string showDirDialog(std::string text);
	bool showCompressDialog();
	void showCancelDialog(std::string text);
	bool scanAsBool();
};