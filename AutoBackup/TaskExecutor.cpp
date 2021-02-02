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
	WIN32_FIND_DATAA findFileData;
	HANDLE findHandle = FindFirstFile(source.c_str(), &findFileData);
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		bool success = false;
		if (findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			FindClose(findHandle);
			if (source.back() != '\\')
				source += "\\";
			source += "*.*";
			source.append(1, '\0');
			if (destination.back() != '\\')
				destination += "\\";
			destination.append(1, '\0');
			std::cout << source << " " << destination << endl;
			SHFILEOPSTRUCT shFileOperationStructure = { 0 };
			shFileOperationStructure.wFunc = FO_COPY;
			shFileOperationStructure.fFlags = FOF_SILENT;
			shFileOperationStructure.pFrom = source.c_str();
			shFileOperationStructure.pTo = destination.c_str();
			shFileOperationStructure.fFlags = FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR | FOF_SILENT;
			SHFileOperation(&shFileOperationStructure);
		}
		else
		{
			const int bufferSize = 256;
			HANDLE inputHandle, outputHandle;
			DWORD inputBytes, outputBytes;
			CHAR buffer[bufferSize];
			if (destination.back() != '\\')
				destination += "\\";
			destination.append(findFileData.cFileName);
			FindClose(findHandle);
			inputHandle = CreateFile(source.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			outputHandle = CreateFile(destination.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (inputHandle != INVALID_HANDLE_VALUE && outputHandle != INVALID_HANDLE_VALUE)
				success = true;
			if (success == true)
				while (ReadFile(inputHandle, buffer, bufferSize, &inputBytes, NULL) && inputBytes > 0)
					WriteFile(outputHandle, buffer, inputBytes, &outputBytes, NULL);
			CloseHandle(outputHandle);
			CloseHandle(inputHandle);
		}


	}
}
