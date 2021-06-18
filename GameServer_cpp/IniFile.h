#pragma once
#include "convert.h"
class CIniFile
{
public:
	CIniFile(void);
	virtual ~CIniFile(void);
private:
	std::string mFileName[MAX_PATH];
public:
	bool Open(std::string &fileName);
	bool Close(void);

	bool SetValue(std::string keyName, std::string valueName, std::string value);
	bool SetValue(std::string keyName, std::string valueName, unsigned long value);
	bool SetValue(std::string keyName, std::string valueName, float value);

	bool GetValue(std::string keyName, std::string valueName, std::string value, unsigned long *bufferLength);
	bool GetValue(std::string keyName, std::string valueName, unsigned long *value);
	bool GetValue(std::string keyName, std::string valueName, float *value);
};
/*
wchar_t* CharToWChar(const char* pstrSrc)
{
	//ASSERT(pstrSrc);
	int nLen = strlen(pstrSrc) + 1;
	size_t ConvertedChars;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	//mbstowcs(pwstr, pstrSrc, nLen);
	mbstowcs_s(&ConvertedChars, pwstr, nLen, pstrSrc, nLen);
	return pwstr;
}
*/