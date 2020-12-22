#pragma once

class AutoBackupProcess
{
	constexpr static const char* _programName = "AutoBackup.exe";
	void* _hProcess;
	AutoBackupProcess();

public:
	~AutoBackupProcess();
	static AutoBackupProcess retrieveProcess();
	bool start();
	bool stop();
	bool isRunning();
private:
	bool isElevated();
};