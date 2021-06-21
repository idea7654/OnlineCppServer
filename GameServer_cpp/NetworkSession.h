#pragma once
#include "ThreadSync.h"
#include "Global.h"
#include "CircularQueue.h"
class CNetworkSession : public CMultiThreadSync<CNetworkSession>
{
public:
	CNetworkSession(void);
	virtual ~CNetworkSession(void);

private:
	OVERLAPPED_EX	mAcceptOverlapped;
	OVERLAPPED_EX	mReadOverlapped;
	OVERLAPPED_EX	mWriteOverlapped;

	byte			mReadBuffer[MAX_BUFFER_LENGTH];
	SOCKADDR_IN		mUdpRemoteInfo;

	SOCKET			mSocket;

	HANDLE			mReliableUdpThreadHandle;
	HANDLE			mReliableUdpThreadStartupEvent;
	HANDLE			mReliableUdpThreadDestroyEvent;
	HANDLE			mReliableUdpThreadWakeUpEvent;
	HANDLE			mReliableUdpWriteCompleteEvent;

	CCircularQueue	mReliableWriteQueue;

	bool			mIsReliableUdpSending;

public:
	bool			Begin(VOID);
	bool			End(VOID);

	bool			Listen(int port, int backLog);
	bool			Accept(SOCKET listenSocket);
	bool			TcpBind(void);
	bool			UdpBind(int port);

	bool			GetLocalIP(std::string pIP);
	USHORT			GetLocalPort(void);

	bool			InitializeReadForIocp(void);
	bool			ReadForIocp(BYTE *data, DWORD &dataLength);
	bool			ReadForEventSelect(BYTE *data, DWORD &dataLength);

	bool			Write(BYTE *data, DWORD dataLength);

	bool			InitializeReadFromForIocp(VOID);
	bool			ReadFromForIocp(LPSTR remoteAddress, USHORT &remotePort, BYTE *data, DWORD &dataLength);
	BOOL			ReadFromForEventSelect(LPSTR remoteAddress, USHORT &remotePort, BYTE *data, DWORD &dataLength);

	BOOL			WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE *data, DWORD dataLength);
	BOOL			WriteTo2(LPSTR remoteAddress, USHORT remotePort, BYTE *data, DWORD dataLength);

	bool			Connect(std::string &address, int port);

	SOCKET			GetSocket(VOID);

	BOOL			GetRemoteAddressAfterAccept(LPTSTR remoteAddress, USHORT &remotePort);

	VOID			ReliableUdpThreadCallback(VOID);

	inline VOID		SetUdpWriteCompleteEvent(VOID)
	{
		SetEvent(mReliableUdpWriteCompleteEvent);
	}
};