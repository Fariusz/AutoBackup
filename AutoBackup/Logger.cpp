#include "Logger.h"
#include "Windows.h"
#include <chrono>
#include <ctime>
using namespace std;
using std::chrono::system_clock;
using time_point = std::chrono::time_point<std::chrono::system_clock>;
using std::time_t;
using std::ctime;

Logger::Logger(std::string fileName, bool blockMode)
{
	fName = fileName; // checking path may be required
	if (blockMode)
		logFile = std::ofstream(fileName, ios_base::app);
}

Logger::~Logger()
{
	if (logFile.is_open())	logFile.close();
}

void Logger::error(std::string message)
{
	if (fName != "")
	{
		time_t time = system_clock::to_time_t(system_clock::now());
		if (bMode == false)
			logFile = std::ofstream(fName, ios_base::app);
		logFile << "ERROR:\t" << message.c_str() << "\t" << ctime(&time);
		if (bMode == false)
			logFile.close();
	}
}

void Logger::info(std::string message)
{
	if (fName != "")
	{
		time_t time = system_clock::to_time_t(system_clock::now());
		if (bMode == false)
			logFile = std::ofstream(fName, ios_base::app);
		logFile << "INFO:\t" << message.c_str() << "\t" << ctime(&time);
		if (bMode == false)
			logFile.close();
	}
}

void Logger::warning(std::string message)
{
	if (fName != "")
	{
		time_t time = system_clock::to_time_t(system_clock::now());
		if (bMode == false)
			logFile = std::ofstream(fName, ios_base::app);
		logFile << "WARNING:\t" << message.c_str() << "\t" << ctime(&time);
		if (bMode == false)
			logFile.close();
	}
}

void Logger::clear()
{
	if (bMode == false)
	{
		logFile = std::ofstream(fName, std::ofstream::out | std::ofstream::trunc);
		logFile.close();
	}
	else
	{
		if(logFile.is_open())
			logFile.close();
		
		logFile.open(fName, std::ofstream::out | std::ofstream::trunc);
		logFile.close();
	}
}

void Logger::waitForReady()
{
	while (logFile.is_open())
	{
		::Sleep(5);
	}
}
