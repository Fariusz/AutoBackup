#pragma once
#include <string>
#include "windows.h"
namespace files
{
	bool exists(std::string path) {
		DWORD dwFileAttributes = GetFileAttributesA(path.c_str());
		if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
			return false;
		else
			return true;
	}
}