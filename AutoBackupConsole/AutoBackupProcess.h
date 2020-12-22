#pragma once

class AutoBackupProcess
{
	constexpr static const char* _processName = "AutoBackup.exe";
	void* _hProcess;
	AutoBackupProcess();

public:
	~AutoBackupProcess();
	static AutoBackupProcess retrieveProcess();
	bool start();
	bool stop();
	bool isRunning();
};