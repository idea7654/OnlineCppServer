#include "IniFile.h"
#include <tchar.h>
CIniFile::CIniFile(void)
{
	ZeroMemory(mFileName, sizeof(mFileName));
}

CIniFile::~CIniFile(void) {};

bool CIniFile::Open(std::string &fileName)
{
	if (fileName.empty())
	{
		return false;
	}

	_tcsncpy(CharToWChar(mFileName->c_str()), CharToWChar(fileName.c_str()), MAX_PATH);
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

	*value = GetPrivateProfileInt(CharToWChar(keyName.c_str()), CharToWChar(valueName.c_str()), 0, CharToWChar(mFileName->c_str()));

	return true;
}

bool CIniFile::GetValue(std::string keyName, std::string valueName, float *value)
{
	if (keyName.empty() || valueName.empty() || !value)
	{
		return false;
	}

	TCHAR valueChar[16] = {0,};
	//float���� ���� �������� �Լ��� ���� ������ �ϴ� ���ڿ� ���·� �����ͼ� ��ȯ�� ����
	GetPrivateProfileString(CharToWChar(keyName.c_str()), CharToWChar(valueName.c_str()), _T(""), valueChar, 16, CharToWChar(mFileName->c_str()));

	*value = (float)_tstof(valueChar);

	return true;
}

bool CIniFile::GetValue(std::string keyName, std::string valueName, std::string value, unsigned long *bufferLength)
{
	if (keyName.empty() || valueName.empty() || value.empty() || !bufferLength)
	{
		return false;
	}

	*bufferLength = GetPrivateProfileString(CharToWChar(keyName.c_str()), CharToWChar(valueName.c_str()), _T(""), CharToWChar(value.c_str()), *bufferLength, CharToWChar(mFileName->c_str()));

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

	WritePrivateProfileString(CharToWChar(keyName.c_str()), CharToWChar(valueName.c_str()), Value, CharToWChar(mFileName->c_str()));

	return true;
}

bool CIniFile::SetValue(std::string keyName, std::string valueName, std::string value)
{
	if (keyName.empty() || valueName.empty())
	{
		return false;
	}

	WritePrivateProfileString(CharToWChar(keyName.c_str()), CharToWChar(valueName.c_str()), CharToWChar(value.c_str()), CharToWChar(mFileName->c_str()));

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

	WritePrivateProfileString(CharToWChar(keyName.c_str()), CharToWChar(valueName.c_str()), Value, CharToWChar(mFileName->c_str()));

	return true;
}