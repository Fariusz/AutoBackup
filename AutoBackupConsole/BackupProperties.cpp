#include "BackupProperties.h"

std::string backupprops::to_string(BackupProperties backup)
{
	using namespace std;
	ostringstream oss;
	oss << backup.firstTime.date
		<< " " << backup.firstTime.time
		<< " " << backup.interval.days
		<< " " << backup.interval.hoursMins
		<< " " << backup.srcDir
		<< " " << backup.destDir
		<< " " << backup.compress;

	return oss.str();
}
