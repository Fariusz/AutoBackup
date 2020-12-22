#pragma once
#include <string>
#include <sstream>

struct BackupInterval {
	std::string days;
	std::string hoursMins;
};

struct BackupProperties {
	std::string srcDir;
	std::string destDir;
	bool compress = false;
	struct {
		std::string date;
		std::string time;
	} firstTime;
	BackupInterval interval;
};

namespace backupprops
{
	std::string to_string(BackupProperties backup);
}