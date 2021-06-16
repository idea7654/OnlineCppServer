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
		//mFreePointer에는 리턴하는 블록 앞에 4바이트로 있던 주소가 들어감

		return ReturnPointer;
	}

	static void operator delete(void* deletePointer)
	{
		*reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer;
		//delete된 블록의 next에 현재 mFreePointer의 주소를 넣어줌
		mFreePointer = static_cast<UCHAR*>(deletePointer);
		//mFreePointer업데이트
	}
private:
	static void allockBlock()
	{
		mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];
		//mFreePointer에 사용할 크기의 메모리를 할당

		UCHAR **Current = reinterpret_cast<UCHAR**>(mFreePointer);
		//할당된 메모리의 첫 블록의 포인터를 Current안에 넣음
		UCHAR *Next = mFreePointer;
		//할당된 메모리 첫 블록

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