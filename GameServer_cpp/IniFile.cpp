#include "IniFile.h"
#include <tchar.h>
CIniFile::CIniFile(void)
{
	ZeroMemory(mFileName, sizeof(mFileName));
}

CIniFile::~CIniFile(void) {};

bool CIniFile::Open(std::string fileName)
{
	if (fileName.empty())
	{
		return false;
	}

	_tcsncpy(mFileName->c_str, fileName.c_str, MAX_PATH);
	return true;
}

bool CIniFile::Close(void)
{
	return true;
}

bool CIniFile::GetValue(std::string keyName, std::string valueName, unsigned long *value)
{
	if (keyName.empty() || valueName.empty() || !value)
	{
		return false;
	}

	*value = GetPrivateProfileInt(keyName.c_str, valueName.c_str, 0, mFileName->c_str);

	return true;
}

bool CIniFile::GetValue(std::string keyName, std::string valueName, float *value)
{
	if (keyName.empty() || valueName.empty() || !value)
	{
		return false;
	}

	TCHAR valueChar[16] = {0,};
	//float형을 따로 가져오는 함수가 없기 때문에 일단 문자열 형태로 가져와서 변환을 해줌
	GetPrivateProfileString(keyName.c_str, valueName.c_str, _T(""), valueChar, 16, mFileName->c_str);

	*value = (float)_tstof(valueChar);

	return true;
}

bool CIniFile::GetValue(std::string keyName, std::string valueName, std::string value, unsigned long *bufferLength)
{
	if (keyName.empty() || valueName.empty() || value.empty() || !bufferLength)
	{
		return false;
	}

	*bufferLength = GetPrivateProfileString(keyName.c_str, valueName.c_str, _T(""), value.c_str, *bufferLength, mFileName->c_str);

	return true;
}

bool CIniFile::SetValue(std::string keyName, std::string valueName, unsigned long value)
{
	if (keyName.empty() || valueName.empty())
	{
		return false;
	}

	TCHAR Value[16] = { 0, };
	_itot(value, Value, 10);

	WritePrivateProfileString(keyName.c_str, valueName.c_str, Value, mFileName->c_str);

	return true;
}

bool CIniFile::SetValue(std::string keyName, std::string valueName, std::string value)
{
	if (keyName.empty() || valueName.empty())
	{
		return false;
	}

	WritePrivateProfileString(keyName.c_str, valueName.c_str, value.c_str, mFileName->c_str);

	return true;
}

bool CIniFile::SetValue(std::string keyName, std::string valueName, float value)
{
	if (keyName.empty() || valueName.empty())
	{
		return false;
	}

	TCHAR Value[16]{ 0, };
	_stprintf(Value, _T("%f"), value);

	WritePrivateProfileString(keyName.c_str, valueName.c_str, Value, mFileName->c_str);

	return true;
}