#pragma warning(disable:4244)
#pragma warning(disable:4996 4005)
#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#ifdef UNICODE
# undef UNICODE
# define _MBCS
#endif
#if defined(_MSVC_LANG) && (_MSVC_LANG>=201703L)
# define _HAS_STD_BYTE 0
# define _HAS_STD_BOOLEAN 0
#endif

#include "TaskExecutor.h"
#include "TimeUtils.h"
#include <windows.h>
#include <cstdlib>
#include <iostream>
#include <tchar.h>
#include <strsafe.h>
using namespace std;

#define BUF_SIZE 255

DWORD WINAPI MyThreadFunction(LPVOID lpParam);


//Struktura danych do przekazywania parametrów do w¹tków
typedef struct threadData
{
	string srcDir;
	string destDir;
	bool compress = false;
}MYDATA, * PMYDATA;
//Przekazywanie przez pusty wskaŸnik LPVOID, mo¿na stosowaæ dowolny typ danych

void TaskExecutor::execute(std::vector<BackupProperties> tasks)
{
	PMYDATA* pDataArray = new PMYDATA[tasks.size()];
	DWORD* dwThreadIdArray = new DWORD[tasks.size()];
	HANDLE* hThreadArray = new HANDLE[tasks.size()];

	for (int i = 0; i < tasks.size(); i++)
	{
		tasks[i].destDir += '\0';
		tasks[i].srcDir += '\0';

		//Alokacja pamiêci pod dane w¹tków
		pDataArray[i] = (PMYDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(MYDATA));
		if (pDataArray[i] == NULL)
		{
			ExitProcess(2);
		}
		pDataArray[i]->srcDir = new char[tasks[i].srcDir.size()];
		for (int ch = 0; ch < tasks[i].srcDir.size(); ch++)
		{
			pDataArray[i]->srcDir[ch] = tasks[i].srcDir[ch];
		}
		pDataArray[i]->destDir = new char[tasks[i].destDir.size()];
		for (int ch = 0; ch < tasks[i].destDir.size(); ch++)
		{
			pDataArray[i]->destDir[ch] = tasks[i].destDir[ch];
		}
		pDataArray[i]->compress = tasks[i].compress;
		dwThreadIdArray[i] = ' ';

		//Je¿eli nie powiedzie siê alokacja pamiêci to koñczymy bo nasz system nie ma ju¿ wolnego ramu
		if (pDataArray[i] == NULL)
		{
			ExitProcess(2);
		}

		//Tworzenie w¹tków
		hThreadArray[i] = CreateThread(
			NULL,
			0,
			MyThreadFunction,
			pDataArray[i],
			0,
			&dwThreadIdArray[i]);

		//Je¿eli tworzenie w¹tka siê nie powiedzie to zg³aszamy b³¹d 
		if (hThreadArray[i] == NULL)
		{
			ErrorHandler((LPTSTR)TEXT("error"));
			//ExitProcess(3);
		}
	}
	DWORD count = tasks.size();

	//Czekanie na zakoñczenie wszystkich dzia³ajacych watków
	WaitForMultipleObjects(count, hThreadArray, TRUE, INFINITE);

	//Zamykanie uchwytów i czyszczenie pamiêci po wykonanych w¹tkach
	for (int i = 0; i < tasks.size(); i++)
	{
		CloseHandle(hThreadArray[i]);
		if (pDataArray[i] != NULL)
		{
			HeapFree(GetProcessHeap(), 0, pDataArray[i]);
			pDataArray[i] = NULL;
		}
	}
	delete[] pDataArray;
	delete[] dwThreadIdArray;
	delete[] hThreadArray;
	pDataArray = nullptr;
	dwThreadIdArray = nullptr;
	hThreadArray = nullptr;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	HANDLE hStdout;
	PMYDATA pDataArray;

	TCHAR msgBuf[BUF_SIZE];
	size_t cchStringSize;
	DWORD dwChars;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdout == INVALID_HANDLE_VALUE)
		return 1;

	pDataArray = (PMYDATA)lpParam;

	StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Tworze backup, kompresja: %d\n"), pDataArray->compress);
	StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
	WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);

	TaskExecutor::DoBackup(pDataArray->srcDir, pDataArray->destDir, pDataArray->compress);

	return 0;
}

void TaskExecutor::DoBackup(string source, string destination, bool compress)
{
	if (source.back() == '\\')
		source.pop_back();
	WIN32_FIND_DATAA findFileData;
	HANDLE findHandle = FindFirstFile(source.c_str(), &findFileData);
	cout << "Source: " << source << endl;
	cout << "Error: " << GetLastError() << endl;
	if (findHandle != INVALID_HANDLE_VALUE)
	{
		cout << "Attr: " << findFileData.dwFileAttributes << endl;
		bool success = false;

		if (findFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			FindClose(findHandle);
			if (source.back() != '\\')
				source += "\\";
			source += "*.*";
			source.append(1, '\0');
			cout << "Source append: " << source << endl;
			if (destination.back() != '\\')
				destination += "\\";
			destination.append(1, '\0');
			//std::cout << source << " " << destination << endl;
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
void TaskExecutor::ErrorHandler(LPTSTR lpszFunction)
{
	// Otrzymanie wiadomoœci o b³êdach wykonania programu
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	//Wyœwietlanie komunikatów o b³êdach
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	//Czyszczenie buforów 
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}


