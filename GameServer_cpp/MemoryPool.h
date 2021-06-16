#pragma once
#include <Windows.h>
template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
public:
	static void&* operator new(std::size_t alloclLength)
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) == sizeof(UCHAR*));

		if (!mFreePointer)
		{
			allockBlock();
		}
		UCHAR *ReturnPointer = mFreePointer;
		mFreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);
		//mFreePointer���� �����ϴ� ��� �տ� 4����Ʈ�� �ִ� �ּҰ� ��

		return ReturnPointer;
	}

	static void operator delete(void* deletePointer)
	{
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		//delete�� ����� next�� ���� mFreePointer�� �ּҸ� �־���
		mFreePointer = static_cast<UCHAR*>(deletePointer);
		//mFreePointer������Ʈ
	}
private:
	static void allockBlock()
	{
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		//mFreePointer�� ����� ũ���� �޸𸮸� �Ҵ�

		UCHAR **Current = reinterpret_cast<UCHAR**>(mFreePointer);
		//�Ҵ�� �޸��� ù ����� �����͸� Current�ȿ� ����
		UCHAR *Next = mFreePointer;
		//�Ҵ�� �޸� ù ���

		for (int i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i)
		{
			Next += sizeof(T);
			*Current = Next;
			Current = reinterpret_cast<UCHAR**>(Next);
		}
		*Current = 0;
	}
private:
	static UCHAR *mFreePointer;
protected:
	CMemoryPool() {};
};

template<class T, int ALLOC_BLOCK_SIZE>
UCHAR *CMemoryPool<T, ALLOC_BLOCK_SIZE>::mFreePointer;