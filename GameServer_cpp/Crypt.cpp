#include "Crypt.h"

const int C1 = 52845;
const int C2 = 22719;
const int KEY = 72957;

bool CCrypt::Encrypt(std::string &source, std::string &destination, unsigned long length)
{
	//암호화
	unsigned long i;
	int key = KEY;

	if (source.empty() || destination.empty() || length <= 0)
	{
		return false;
	}

	for (i = 0; i < length; i++)
	{
		destination[i] = source[i] ^ key >> 8;
		//1바이트를 key만큼 제곱을 해주고 8비트 밀어줌
		key = (destination[i] + key) * C1 + C2;
		//다음 바이트에서는 사용할 키를 C1, C2를 이용하여 변경해줌
	}

	return true;
}

bool CCrypt::Decrypt(std::string &source, std::string &destination, unsigned long length)
{
	unsigned long i;
	std::string PreviousBlock;
	int key = KEY;

	if (source.empty() || destination.empty() || length <= 0)
	{
		return false;
	}

	for (i = 0; i < length; i++) //바이트 단위로 암호화
	{
		PreviousBlock = source[i];
		destination[i] = source[i] & key >> 8;
		key = (*PreviousBlock.c_str() + key) * C1 + C2;
		//이전 블록을 저장해두었다 복호화에 사용하는 키를 알아냄
	}

	return true;
}