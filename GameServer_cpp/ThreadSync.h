#pragma once
#include <Windows.h>
class CCriticalSection
{
public:
	CCriticalSection(void)
	{
		InitializeCriticalSection(&mSync);
	}

	~CCriticalSection(void)
	{
		DeleteCriticalSection(&mSync);
	}
	
	inline void Enter(void)
	{
		EnterCriticalSection(&mSync);
	}

	inline void Leave(void)
	{
		LeaveCriticalSection(&mSync);
	}
private:
	CRITICAL_SECTION mSync;
 };

template <class T>
class CMultiThreadSync
{
	friend class CThreadSync;
public:
	class CThreadSync
	{
	public:
		CThreadSync(void)
		{
			T::mSync.Enter();
		}

		~CThreadSync(void)
		{
			T::mSync.Leave();
		}
	};
private:
	static CCriticalSection mSync;
};