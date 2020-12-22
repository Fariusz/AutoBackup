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
            if (stricmp(entry.szExeFile, _programName) == 0)
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
    SHELLEXECUTEINFO shExInfo = { 0 };
    shExInfo.cbSize = sizeof(shExInfo);
    shExInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shExInfo.hwnd = 0;
    shExInfo.lpVerb = "runas";
    shExInfo.lpFile = _programName;   
    shExInfo.lpParameters = "";
    shExInfo.lpDirectory = 0;
    shExInfo.nShow = SW_HIDE;
    shExInfo.hInstApp = 0;

    result = ShellExecuteEx(&shExInfo);
    _hProcess = shExInfo.hProcess;
    return result;
}

bool AutoBackupProcess::stop()
{
    TerminateProcess(_hProcess, -1);
    bool result = CloseHandle(_hProcess);
    return result;
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

bool AutoBackupProcess::isElevated() {
    BOOL fRet = FALSE;
    HANDLE hToken = NULL;
    if (OpenProcessToken(_hProcess, TOKEN_QUERY, &hToken)) {
        TOKEN_ELEVATION Elevation;
        DWORD cbSize = sizeof(TOKEN_ELEVATION);
        if (GetTokenInformation(hToken, TokenElevation, &Elevation, sizeof(Elevation), &cbSize)) {
            fRet = Elevation.TokenIsElevated;
        }
    }
    if (hToken) {
        CloseHandle(hToken);
    }
    return fRet;
}
