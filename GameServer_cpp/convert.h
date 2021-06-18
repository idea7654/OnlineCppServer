#pragma once
#include <string>
#include <Windows.h>
static wchar_t* CharToWChar(const char* pstrSrc)
{
	//ASSERT(pstrSrc);
	int nLen = strlen(pstrSrc) + 1;
	size_t ConvertedChars;
	wchar_t* pwstr = (LPWSTR)malloc(sizeof(wchar_t)* nLen);
	//mbstowcs(pwstr, pstrSrc, nLen);
	mbstowcs_s(&ConvertedChars, pwstr, nLen, pstrSrc, nLen);
	return pwstr;
}