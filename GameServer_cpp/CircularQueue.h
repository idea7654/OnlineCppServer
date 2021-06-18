#pragma once
#include <Windows.h>

template<class T>
class CCircularQueue2 : public CMultiThreadSync<CCircularQueue>
{
public:
	CCircularQueue2(void)
	{
		ZeroMemory(mQueue, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;
	}
	~CCircularQueue2(void) {};

private:
	int MAX_QUEUE_LENGTH = 5;
	T mQueue[MAX_QUEUE_LENGTH];
	unsigned long mQueueHead;
	unsigned long mQueueTail;

public:
	bool Begin(void)
	{
		ZeroMemory(mQueue, sizeof(mQueue));
		mQueueHead = mQueueTail = 0;

		return true;
	}

	bool End(void) { return true; }
	bool Push(T data)
	{
		CThread Sync;

		unsigned long TempTail = (mQueueTail + 1) % MAX_QUEUE_LENGTH;
		if (TempTail == mQueueHead)
		{
			return false;
		}

		CopyMemory(&mQueue[TempTail], &data, sizeof(T));

		mQueueTail = TempTail;

		return true;
	}

	bool Pop(T& data)
	{
		CThread Sync;

		if (mQueueHead == mQueueTail)
		{
			return false;
		}

		unsigned long TempHead = (mQueueHead + 1) % MAX_QUEUE_LENGTH;
		CopyMemory(&data, &mQueue[TempHead], sizeof(T));
		mQueueHead = TempHead;
		return true;
	}

	bool IsEmpty(void)
	{
		CThreadSync Sync;
		if (mQueueHead == mQueueTail) return true;
		return false;
	}
};