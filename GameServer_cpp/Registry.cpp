#include "Registry.h"
#include <cstdlib>
CRegistry::CRegistry(void)
{
	mRootKey = NULL; //사용할 레지스트리 키를 저장하는 변수
	mIsOpened = false; //해당 레지스트리를 열었는지 체크하는 변수
}

CRegistry::~CRegistry(void) {};

bool CRegistry::Open(HKEY rootKey, std::string subKey)
{
	if (subKey.empty())
	{
		return false;
	}

	if (mIsOpened)
	{
		return false;
	}

	if (RegOpenKey(rootKey, subKey.c_str, &mRootKey) != ERROR_SUCCESS) {
		return false;
	}

	mIsOpened = true;

	return true;
}

bool CRegistry::Close(void)
{
	if (RegCloseKey(mRootKey) != ERROR_SUCCESS) {
		return false;
	}
	mIsOpened = false;

	return true;
}

bool CRegistry::CreateKey(HKEY rootKey, std::string subKey)
{
	if (subKey.empty())
	{
		return false;
	}

	if (mIsOpened)
	{
		return false;
	}

	if (RegCreateKey(rootKey, subKey.c_str, &mRootKey) != ERROR_SUCCESS)
	{
		return false;
	}

	mIsOpened = true;

	return true;
}

bool CRegistry::DeleteKey(HKEY rootKey, std::string subKey)
{
	if (subKey.empty())
	{
		return false;
	}

	if (RegDeleteKey(rootKey, subKey.c_str) != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CRegistry::SetValue(std::string valueName, std::string value)
{
	if (valueName.empty() || value.empty())
	{
		return false;
	}

	if (!mIsOpened)
	{
		return false;
	}

	if (RegSetValueEx(mRootKey, valueName.c_str, 0, REG_SZ, value.c_str, std::strtoul(value.c_str(), NULL, 16)) != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CRegistry::SetValueFromMultiSz(std::string valueName, std::string value, unsigned long byteLength)
{
	if (valueName.empty() || value.empty())
	{
		return false;
	}
	if (!mIsOpened)
	{
		return false;
	}

	if (RegSetValueEx(mRootKey, valueName.c_str, 0, REG_MULTI_SZ, value.c_str, byteLength) != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CRegistry::SetValue(std::string valueName, unsigned long value)
{
	if (valueName.empty())
	{
		return false;
	}
	if (!mIsOpened)
	{
		return false;
	}

	if (RegSetValueEx(mRootKey, valueName.c_str, 0, REG_DWORD, (BYTE*)&value, sizeof(unsigned long)) != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CRegistry::GetValue(std::string valueName, std::string value, unsigned long* bufferLength)
{
	unsigned long valueType = 0;

	if (valueName.empty() || value.empty() || !bufferLength)
	{
		return false;
	}

	if (!mIsOpened)
	{
		return false;
	}

	if (RegQueryValueEx(mRootKey, valueName.c_str, 0, &valueType, value.c_str, bufferLength) != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CRegistry::GetValue(std::string valueName, unsigned long* value)
{
	unsigned long ValueType = 0;
	unsigned long BufferLength = sizeof(unsigned long);

	if (valueName.empty() || !value)
	{
		return false;
	}

	if (!mIsOpened)
	{
		return false;
	}

	if (RegQueryValueEx(mRootKey, valueName.c_str, 0, &ValueType, (BYTE*)value, &BufferLength) != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}

bool CRegistry::DeleteValue(std::string valueName)
{
	if (valueName.empty())
	{
		return false;
	}

	if (!mIsOpened)
	{
		return false;
	}

	return true;
}