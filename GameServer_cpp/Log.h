#pragma once
#include <string>
#include <Windows.h>
#include <tchar.h>
class CLog
{
public:
	static bool WriteLog(std::string data, ...)
	{
		const int MAX_BUFFER_LENGTH = 256;
		SYSTEMTIME systemTime;
		std::string CurrentDate[32] = { 0, };
		std::string CurrentFileName[MAX_PATH] = { 0, };
		FILE* FilePtr = NULL;
		std::string DebugLog[MAX_BUFFER_LENGTH] = { 0, };

		va_list ap;
		std::string Log[MAX_BUFFER_LENGTH] = { 0, };

		va_start(ap, data);
		_vswprintf(Log->c_str, data.c_str, ap);
		va_end(ap);

		GetLocalTime(&systemTime);
		_snwprintf(CurrentDate->c_str, 32, _T("%d-%d-%d %d:%d:%d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
		_snwprintf(CurrentFileName->c_str, MAX_PATH, _T("LOG_%d-%d-%d %d.log"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog->c_str);
		_tprintf(_T("%s"), DebugLog->c_str);

		return true;
	}
};