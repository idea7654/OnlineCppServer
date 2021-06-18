#include <iostream>
#include <Windows.h>
#include "MemoryLeak.h"
int main()
{
   // std::cout << "Hello World!\n";
	byte *pData = new byte[1024];
	ZeroMemory(pData, 1024 * sizeof(byte));

	return 0;
}