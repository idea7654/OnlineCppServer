#pragma once
#include <Windows.h>
#include <string>
class CRegistry
{
public:
	CRegistry(void);
	virtual ~CRegistry(void);
public:
	bool Open(HKEY rootKey, std::string subKey);
	bool Close(void);

	bool CreateKey(HKEY rootKey, std::string subKey);
	bool DeleteKey(HKEY rootKey, std::string subKey);

	bool SetValue(std::string valueName, std::string value);
	bool SetValue(std::string valueName, unsigned long value);
	bool SetValueFromMultiSz(std::string valueName, std::string value, unsigned long byteLength);

	bool GetValue(std::string valueName, std::string value, unsigned long* bufferLength);
	bool GetValue(std::string valueName, unsigned long* value);

	bool DeleteValue(std::string valueName);
private:
	HKEY mRootKey;
	bool mIsOpened;
};