#include "TaskExecutor.h"
#include "TimeUtils.h"
#include <windows.h>
#include <iostream>
using namespace std;

void TaskExecutor::execute(std::vector<BackupProperties> tasks)
{
	// TODO implement 

	for(BackupProperties t : tasks)
	{
		DoBackup(t.srcDir, t.destDir, t.compress);
	}

}

void TaskExecutor::DoBackup(string source, string destination, bool compress)
{
	const int bufferSize = 256;
	HANDLE inputHandle, outputHandle, findHandle;
	DWORD inputBytes, outputBytes;
	CHAR buffer[bufferSize];
	WIN32_FIND_DATAA findFileData;
	findHandle = FindFirstFile(source.c_str(), &findFileData);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		bool success = false;
		if (findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			FindClose(findHandle);
			/*inputHandle = CreateFile(source.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);
			std::cout << "input: " << GetLastError() << endl;//////////////////
			outputHandle = CreateFile(destination.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_DIRECTORY, NULL);
			std::cout << "output: " << GetLastError() << endl;//////////////////
			if (inputHandle != INVALID_HANDLE_VALUE && outputHandle != INVALID_HANDLE_VALUE)
				success = true;*/
			// backup folderu

		}
		else
		{
			destination.append(findFileData.cFileName);
			FindClose(findHandle);
			inputHandle = CreateFile(source.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			outputHandle = CreateFile(destination.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (inputHandle != INVALID_HANDLE_VALUE && outputHandle != INVALID_HANDLE_VALUE)
				success = true;
		}
		if(success == true)
			while (ReadFile(inputHandle, buffer, bufferSize, &inputBytes, NULL) && inputBytes > 0)
				WriteFile(outputHandle, buffer, inputBytes, &outputBytes, NULL);
		CloseHandle(outputHandle);
		CloseHandle(inputHandle);
	}
}
