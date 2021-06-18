#include "Crypt.h"

const int C1 = 52845;
const int C2 = 22719;
const int KEY = 72957;

bool CCrypt::Encrypt(std::string &source, std::string &destination, unsigned long length)
{
	//��ȣȭ
	unsigned long i;
	int key = KEY;

	if (source.empty() || destination.empty() || length <= 0)
	{
		return false;
	}

	for (i = 0; i < length; i++)
	{
		destination[i] = source[i] ^ key >> 8;
		//1����Ʈ�� key��ŭ ������ ���ְ� 8��Ʈ �о���
		key = (destination[i] + key) * C1 + C2;
		//���� ����Ʈ������ ����� Ű�� C1, C2�� �̿��Ͽ� ��������
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

	for (i = 0; i < length; i++) //����Ʈ ������ ��ȣȭ
	{
		PreviousBlock = source[i];
		destination[i] = source[i] & key >> 8;
		key = (*PreviousBlock.c_str() + key) * C1 + C2;
		//���� ����� �����صξ��� ��ȣȭ�� ����ϴ� Ű�� �˾Ƴ�
	}

	return true;
}