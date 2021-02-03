#include "Dialog.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "Files.h"
#include "Validation.h"
#include "NotImplementedException.h"
#include "AutoBackupProcess.h"
#include "fileapi.h"

using namespace std;

Command Dialog::showMainDialog()
{
	refreshConsole();
	printMainMenu();
	int option = readOption();
	Command command = (Command)(option - 1);
	switch (command)
	{
	case Command::Status:		showStatus();				break;
	case Command::Start:		startAutoBackupProcess();	break;
	case Command::Stop:			stopAutoBackupProcess();	break;
	case Command::ShowSchedule: showSchedule();				break;
	case Command::NewTask:		createNewBackupTask();		break;
	case Command::ClearTasks:	clearTasks();		        break;
	case Command::Shutdown:									break;
	}
	return command;
}

void Dialog::showStatus()
{
	AutoBackupProcess process = AutoBackupProcess::retrieveProcess();
	bool status = process.isRunning();
	if (status)
		showMessage("Program AutoBackup jest uruchomiony.");
	else
		showMessage("Program AutoBackup jest wy��czony.");
}

void Dialog::startAutoBackupProcess()
{
	AutoBackupProcess process = AutoBackupProcess::retrieveProcess();
	bool result = false;
	if (process.isRunning())
		showMessage("Program AutoBackup jest ju� uruchomiony.");
	else if (process.start())
		showMessage("Pomy�lnie uruchomiono program AutoBackup.");
	else
		showMessage("Nie uda�o si� uruchomi� programu AutoBackup.");
}

void Dialog::stopAutoBackupProcess()
{
	AutoBackupProcess process = AutoBackupProcess::retrieveProcess();
	if (!process.isRunning())
		showMessage("Program AutoBackup jest ju� wy��czony.");
	else if (process.stop())
		showMessage("Pomy�lnie zatrzymano program AutoBackup.");
	else
		showMessage("Nie uda�o si� wy��czy� programu AutoBackup.");		
}

void Dialog::showMessage(std::string text)
{
	refreshConsole();
	printf("%s Naci�nij dowolny przycisk...", text.c_str());
	pause();
}

void Dialog::showSchedule()
{
	refreshConsole();
	vector<BackupProperties> tasks = loadTasksFromFile(SCHEDULE_FILE_NAME);
	printTasks(tasks);
	printf("\nNaci�nij dowolny przycisk, aby wr�ci� do menu g��wnego...");
	getchar();
}

void Dialog::printTasks(std::vector<BackupProperties>& tasks)
{
	printf("Data\t\tGodzina\t\tOkres\t\tObiekt �r�d�owy\t\tFolder docelowy\t\tKompresja\n");
	for (BackupProperties t : tasks)
	{
		printf(
			"%s\t%s%\t%s %s\t%-20s\t%-20s\t%5d\n",
			t.firstTime.date.c_str(),
			t.firstTime.time.c_str(),
			t.interval.days.c_str(),
			t.interval.hoursMins.c_str(),
			t.srcDir.c_str(),
			t.destDir.c_str(),
			t.compress
		);
	}
}

void Dialog::pause()
{
	getchar();
	getchar();
}

vector<BackupProperties> Dialog::loadTasksFromFile(std::string fileName)
{
	vector<BackupProperties> tasks;
	ifstream ifs(fileName);
	std::string line;

	while (std::getline(ifs, line)) {
		BackupProperties task;
		istringstream iss(line);
		string intervalMonthsDay, intervalHoursMins;
		iss >> task.firstTime.date;
		iss >> task.firstTime.time;
		iss >> task.interval.days;
		iss >> task.interval.hoursMins;
		iss >> task.srcDir;
		iss >> task.destDir;
		iss >> task.compress;
		tasks.push_back(task);
	}
	return tasks;
}

BackupProperties Dialog::showNewTaskDialog()
{
	BackupProperties backup;
	backup.srcDir = showDirDialog("Podaj �cie�k� do katalogu albo pliku �r�d�owego");
	backup.destDir = showDirDialog("Podaj �cie�k� do katalogu docelowego");
	backup.compress = showCompressDialog();
	backup.firstTime.date = showDateDialog();
	backup.firstTime.time = showTimeDialog();
	backup.interval = showIntervalDialog();
	return backup;
}

void Dialog::createNewBackupTask()
{
	try
	{
		BackupProperties backup = showNewTaskDialog();
		saveTask(backup);
		showMessage("Pomy�lnie dodano zadanie.");
	}
	catch (canceledException) {}
	catch (exception e)
	{
		refreshConsole();
		printf("Wyst�pi� b��d. Naci�nij dowolny przycisk...");
	}
}

void Dialog::saveTask(const BackupProperties& backup)
{
	ofstream ofs("schedule.dat", ios::app);
	ofs << backupprops::to_string(backup) << "\n";
	ofs.close();
}

void Dialog::clearTasks()
{
	std::ofstream ofs;
	ofs.open("schedule.dat", std::ofstream::out | std::ofstream::trunc);
	ofs.close();
	/*
		if(!(DeleteFileA((LPCSTR)TEXT(".\schedule.dat"))))
		{	
			refreshConsole();
			printf("Wyst�pi� b��d. Naci�nij dowolny przycisk...");

			DWORD error = GetLastError();
			if (error == ERROR_ACCESS_DENIED)
			{
				showMessage("Odmowa dost�pu\n");
			}
			else if (error = ERROR_FILE_NOT_FOUND)
			{
				showMessage("Nie znaleziono pliku\n");
			}
		}
		else
		{
			showMessage("Usuni�to wszystkie zadania\n");
		}
			*/
}

string Dialog::showDirDialog(string text)
{
	bool pathExists = false;
	string dir;
	do
	{
		refreshConsole();
		printf("%s: ", text.c_str());
		cin >> dir;
		cin.clear();
		pathExists = files::exists(dir);
		if (!pathExists)
			showCancelDialog("�cie�ka " + dir + " nie istnieje. Czy chcesz wprowadzi� �cie�k� ponownie?");
	} while (!pathExists);
	return dir;
}

string Dialog::showDateDialog()
{
	bool isFormatCorrect = false;
	string date;
	do
	{
		refreshConsole();
		printf("Podaj dat� pierwszego backupu (dd-mm-yyyy): ");
		cin >> date;
		cin.clear();
		isFormatCorrect = validation::validateDate(date);
		if (!isFormatCorrect)
			showCancelDialog("Niepoprawna data. Czy chcesz wprowadzi� dat� ponownie?");
	} while (!isFormatCorrect);
	return date;
}

std::string Dialog::showTimeDialog()
{
	bool isFormatCorrect = false;
	string time;
	do
	{
		refreshConsole();
		printf("Podaj godzin� pierwszego backupu (hh:mm:ss): ");
		cin >> time;
		cin.clear();
		isFormatCorrect = validation::validateTime(time);
		if (!isFormatCorrect)
			showCancelDialog("Niepoprawny czas. Czy chcesz wprowadzi� czas ponownie?");
	} while (!isFormatCorrect);

	return time;
}

BackupInterval Dialog::showIntervalDialog()
{
	bool isFormatCorrect = false;
	BackupInterval interval;
	do
	{
		refreshConsole();
		printf("Podaj okres pomi�dzy kopiami (dd hh:mm): ");
		cin >> interval.days >> interval.hoursMins;
		cin.clear();
		string sInterval = interval.days + " " + interval.hoursMins;
		isFormatCorrect = validation::validateInterval(sInterval);
		if (!isFormatCorrect)
			showCancelDialog("Niepoprawny okres. Czy chcesz okres ponownie?");
	} while (!isFormatCorrect);
	return interval;
}

bool Dialog::showCompressDialog()
{
	refreshConsole();
	printf("Czy skompresowa� kopi� (1/0): ");
	bool compress = scanAsBool();
	return compress;
}

void Dialog::showCancelDialog(string text)
{
	refreshConsole();
	printf("%s (1/0): ", text.c_str());
	bool result = scanAsBool();
	if (!result)
		throw canceledException();
}

void Dialog::refreshConsole()
{
	system("cls");
	printf("\t\t\t --------------------------------------------------------------------\n");
	printf("\t\t\t| Auto Backup - program do automatycznego tworzenia kopi zapasowych  |\n");
	printf("\t\t\t --------------------------------------------------------------------\n\n\n\n");
}

void Dialog::printMainMenu()
{
	printf("Wybierz opcj�:\n");
	printf("1 - Status programu \n2 - Uruchom program \n3 - Przerwij program \n4 - Utw�rz zadanie \n5 - Wyczy�� zadania \n6 - Wy�wietl harmonogram \n7 - Wy��cz konsol�\n\n");
}

int Dialog::readOption()
{
	bool correctOption, optionInScope;
	int option;
	do
	{
		int iChar = _getch();
		char cOption = (char)iChar;
		correctOption = isalnum(cOption) && !isalpha(cOption);
		option = cOption - '0';
		optionInScope = option > -1 && option < 8;
	} while (!correctOption || !optionInScope);
	return option;
}

bool Dialog::scanAsBool()
{
	int iChar = _getch();
	bool result;
	if (isdigit(iChar))
		result = iChar - '0';
	else
		result = true;
	cin.clear();
	return result;
}
