#pragma once
#include <fstream>

class Logger
{
	std::ofstream logFile;
	bool bMode{};				// constant blocking file or not
	std::string fName;

public:
	/* if releasing file is not required, to accelerate logs set blockMode to true */
	Logger() = delete;
	Logger(std::string fileName, bool blockMode = false);
	~Logger();

	bool isBlockMode() { return bMode; }
	std::string getPath() { return fName; }

	void error(std::string message);
	void info(std::string message);
	void warning(std::string message);
	void clear();
	void waitForReady();
};