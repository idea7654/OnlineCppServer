#pragma once
#include "convert.h"
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

	bool SetValue(std::string valueName, std::string &value);
	bool SetValue(std::string valueName, unsigned long value);
	bool SetValueFromMultiSz(std::string valueName, std::string &value, unsigned long byteLength);

	bool GetValue(std::string valueName, std::string& value, unsigned long* bufferLength);
	bool GetValue(std::string valueName, unsigned long* value);

	bool DeleteValue(std::string valueName);
private:
	HKEY mRootKey;
	bool mIsOpened;
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