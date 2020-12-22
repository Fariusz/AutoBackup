#include "AutoBackupProcess.h"
#pragma warning(disable:4244)
#pragma warning(disable:4996 4005)
//#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#ifdef UNICODE
# undef UNICODE
# define _MBCS
#endif
#include <Windows.h>
#include <TlHelp32.h>

AutoBackupProcess::AutoBackupProcess() : _hProcess(NULL)
{

}
AutoBackupProcess::~AutoBackupProcess()
{
}
AutoBackupProcess AutoBackupProcess::retrieveProcess()
{
    AutoBackupProcess processInstance;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, _processName) == 0)
            {
                processInstance._hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
                break;
            }
        }
    }
    CloseHandle(snapshot);
    return processInstance;
}
bool AutoBackupProcess::start()
{
    BOOL result;



    return false;
}

bool AutoBackupProcess::stop()
{
    BOOL result;
    return false;
}

bool AutoBackupProcess::isRunning()
{
    BOOL bRunning;
    if (_hProcess != NULL && _hProcess != INVALID_HANDLE_VALUE)
    {
        DWORD dwCode;
        GetExitCodeProcess(_hProcess, &dwCode);
        bRunning = (dwCode == STILL_ACTIVE);
    }
    else
        bRunning = false;
    return bRunning;
}
