#pragma once
#include <Windows.h>
#include <string>
class CIniFile
{
public:
	CIniFile(void);
	virtual ~CIniFile(void);
private:
	std::string mFileName[MAX_PATH];
public:
	bool Open(std::string fileName);
	bool Close(void);

	bool SetValue(std::string keyName, std::string valueName, std::string value);
	bool SetValue(std::string keyName, std::string valueName, unsigned long value);
	bool SetValue(std::string keyName, std::string valueName, float value);

	bool GetValue(std::string keyName, std::string valueName, std::string value, unsigned long *bufferLength);
	bool GetValue(std::string keyName, std::string valueName, unsigned long *value);
	bool GetValue(std::string keyName, std::string valueName, float *value);
};