#pragma once
//#include <string>
#include "Global.h"
class CCrypt
{
public:
	static bool Encrypt(std::string &source, std::string &destination, unsigned long length);
	static bool Decrypt(std::string &source, std::string &destination, unsigned long length);
};