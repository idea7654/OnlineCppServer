#include "NetworkSession.h"

DWORD WINAPI ReliableUdpThreadCallback(LPVOID parameter)
{
	CNetworkSession *Owner = (CNetworkSession*)parameter;
	Owner->ReliableUdpThreadCallback();

	return 0;
}

CNetworkSession::CNetworkSession(void)
{
	ZeroMemory(&mAcceptOverlapped, sizeof(mAcceptOverlapped));
	ZeroMemory(&mReadOverlapped, sizeof(mReadOverlapped));
	ZeroMemory(&mWriteOverlapped, sizeof(mWriteOverlapped));
	ZeroMemory(&mReadBuffer, sizeof(mReadBuffer));
	ZeroMemory(&mUdpRemoteInfo, sizeof(mUdpRemoteInfo)); //udp����Ҷ� ����ּ� ����

	mSocket = NULL;
	mReliableUdpThreadHandle = NULL; //Reliable UDP���ú���
	mReliableUdpThreadStartupEvent = NULL;
	mReliableUdpThreadDestroyEvent = NULL;
	mReliableUdpThreadWakeUpEvent = NULL;
	mReliableUdpWriteCompleteEvent = NULL;
	mIsReliableUdpSending = false;

	mAcceptOverlapped.IoType = IO_ACCEPT;
	mReadOverlapped.IoType = IO_READ;
	mWriteOverlapped.IoType = IO_WRITE;
	
	mAcceptOverlapped.Object = this;
	mReadOverlapped.Object = this;
	mWriteOverlapped.Object = this;
}

bool CNetworkSession::Begin(void)
{
	CThreadSync Sync;
	if (mSocket)
	{
		return false;
	}

	ZeroMemory(mReadBuffer, sizeof(mReadBuffer));
	//UDP�� ����� �� �����͸� ���� ��� ������ �����ϱ� ���� ����
	ZeroMemory(&mUdpRemoteInfo, sizeof(mUdpRemoteInfo));

	mSocket = NULL;

	mReliableUdpThreadHandle = NULL;
	mReliableUdpThreadHandle = NULL; //Reliable UDP���ú���
	mReliableUdpThreadStartupEvent = NULL;
	mReliableUdpThreadDestroyEvent = NULL;
	mReliableUdpThreadWakeUpEvent = NULL;
	mReliableUdpWriteCompleteEvent = NULL;
	mIsReliableUdpSending = false;

	return true;
}

bool CNetworkSession::End(void)
{
	CThreadSync Sync;
	if (!mSocket)
	{
		return false;
	}

	shutdown(mSocket, SD_BOTH);
	closesocket(mSocket);

	mSocket = NULL;

	if (mReliableUdpThreadHandle)
	{
		SetEvent(mReliableUdpThreadDestroyEvent);

		//�����尡 ����ɶ����� ���Ѵ��
		//3~5�� ��ٸ� �� ������ ������� terminateThread�� ���������ϴ°͵� ����
		WaitForSingleObject(mReliableUdpThreadHandle, INFINITE);
		CloseHandle(mReliableUdpThreadHandle);
	}

	if (mReliableUdpThreadDestroyEvent)
	{
		CloseHandle(mReliableUdpThreadDestroyEvent);
	}
	if (mReliableUdpThreadStartupEvent)
	{
		CloseHandle(mReliableUdpThreadStartupEvent);
	}
	if (mReliableUdpThreadWakeUpEvent)
	{
		CloseHandle(mReliableUdpThreadWakeUpEvent);
	}
	if (mReliableUdpWriteCompleteEvent)
	{
		CloseHandle(mReliableUdpWriteCompleteEvent);
	}

	mReliableWriteQueue.End();

	return true;
}

bool CNetworkSession::TcpBind(void)
{
	CThreadSync Sync;

	if (mSocket)
	{
		return false;
	}

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
	{
		return false;
	}
	//TCP_NODELAY�ɼ�
	//bool NoDelay = true;
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR*) &NoDelay, sizeof(NoDelay));
	return true;
}

bool CNetworkSession::Listen(int port, int backLog)
{
	CThreadSync Sync;
	if (port <= 0 || backLog <= 0)
	{
		return false;
	}
	if (!mSocket)
	{
		return false;
	}
	SOCKADDR_IN ListenSocketInfo;
	
	ListenSocketInfo.sin_family = AF_INET;
	ListenSocketInfo.sin_port = htons(port);
	ListenSocketInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	if (bind(mSocket, (struct sockaddr*) &ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();
		return false;
	}

	if (listen(mSocket, backLog) == SOCKET_ERROR)
	{
		End();
		return false;
	}

	LINGER Linger; //������ ���� �� ���� ������ �ְų� �޴� �����Ͱ� ���� ��� �� �����͸� ��� ���� �� ������ ����
	//���� but ���ῡ �ð� ���̰ɸ�
	Linger.l_onoff = 1;
	Linger.l_linger = 0;

	if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		End();
		return false;
	}

	return true;
}

bool CNetworkSession::Connect(std::string &address, int port)
{
	CThreadSync Sync;
	if (address.empty() || port <= 0)
	{
		return false;
	}
	if (!mSocket)
	{
		return false;
	}

	SOCKADDR_IN RemoteAddressInfo; //������ �ּ��� ������ �����ϴ� ����ü

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_port = htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(address.c_str());
	
	//���� ������ �ּҷ� ���� �õ�
	if (WSAConnect(mSocket, (LPSOCKADDR)&RemoteAddressInfo, sizeof(LPSOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK) //�������� �����¶�� ����� ������ ��� �������� �ν�
		{
			End();
			return false;
		}
	}

	return true;
}

bool CNetworkSession::Accept(SOCKET listenSocket)
{
	CThreadSync Sync;

	if (!listenSocket)
	{
		return false;
	}
	if (mSocket)
	{
		return false;
	}
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mSocket == INVALID_SOCKET)
	{
		End();
		return false;
	}
	//bool Nodelay = true;
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&Nodelay, sizeof(Nodelay));
	if (!AcceptEx(listenSocket, mSocket, mReadBuffer, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &mAcceptOverlapped.Overlapped))
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();
			return false;
		}
	}
	return true;
}
//IOCP�ʱ�ޱ�
bool CNetworkSession::InitializeReadForIocp(void)
{
	CThreadSync Sync;

	if (!mSocket)
	{
		return false;
	}
	WSABUF wsaBuf;
	unsigned long ReadBytes = 0;
	unsigned long ReadFlag = 0;

	wsaBuf.buf = (CHAR*)mReadBuffer;
	wsaBuf.len = MAX_BUFFER_LENGTH;

	int ReturnValue = WSARecv(mSocket, &wsaBuf, 1, &ReadBytes, &ReadFlag, &mReadOverlapped.Overlapped, NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();
		return false;
	}
	return true;
}

bool CNetworkSession::ReadForIocp(byte *data, unsigned long &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
	{
		return false;
	}
	if (!data || dataLength <= 0)
	{
		return false;
	}
	CopyMemory(data, mReadBuffer, dataLength);
	return true;
}

bool CNetworkSession::ReadForEventSelect(byte *data, unsigned long &dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
	{
		return false;
	}
	if (!data)
	{
		return false;
	}
	if (!mSocket)
	{
		return false;
	}
	WSABUF wsaBuf;
	unsigned long ReadBytes = 0;
	unsigned long ReadFlag = 0;

	wsaBuf.buf = (char *)mReadBuffer;
	wsaBuf.len = MAX_BUFFER_LENGTH;

	int ReturnValue = WSARecv(mSocket, &wsaBuf, 1, &ReadBytes, &ReadFlag, &mReadOverlapped.Overlapped, NULL);
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();
		return false;
	}

	CopyMemory(data, mReadBuffer, ReadBytes);
	dataLength = ReadBytes;
	return true;
}

bool CNetworkSession::Write(byte *data, unsigned long dataLength)
{
	CThreadSync Sync;
	if (!mSocket)
	{
		return false;
	}
	if (!data || dataLength <= 0)
	{
		return false;
	}
	WSABUF wsaBuf;
	unsigned long writeBytes = 0;
	unsigned long writeFlag = 0;
	wsaBuf.buf = (char*)data;
	wsaBuf.len = dataLength;

	int ReturnValue = WSASend(mSocket, &wsaBuf, 1, &writeBytes, &writeFlag, &mWriteOverlapped.Overlapped, NULL);
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();
		return false;
	}
	return true;
}

bool CNetworkSession::UdpBind(int port)
{
	CThreadSync Sync;

	if (mSocket)
	{
		return false;
	}
	SOCKADDR_IN RemoteAddressInfo;

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_port = htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

	mSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (mSocket == INVALID_SOCKET)
	{
		return false;
	}
	if (bind(mSocket, (struct sockaddr*)&RemoteAddressInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();
		return false;
	}
	mReliableUdpThreadDestroyEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpThreadDestroyEvent == NULL)
	{
		End();
		return false;
	}
	mReliableUdpThreadStartupEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpThreadStartupEvent == NULL)
	{
		End();
		return false;
	}
	mReliableUdpThreadWakeUpEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpThreadWakeUpEvent == NULL)
	{
		End();
		return false;
	}
	mReliableUdpWriteCompleteEvent = CreateEvent(0, false, false, 0);
	if (mReliableUdpWriteCompleteEvent == NULL)
	{
		End();
		return false;
	}
	unsigned long ReliableUdpThreadID = 0;
	mReliableUdpThreadHandle = CreateThread(NULL, 0, ::ReliableUdpThreadCallback, this, 0, &ReliableUdpThreadID);
	WaitForSingleObject(mReliableUdpThreadStartupEvent, INFINITE);
	return true;
}

void CNetworkSession::ReliableUdpThreadCallback(void)
{
	unsigned long EventID = 0;
	HANDLE ThreadEvents[2] = { mReliableUdpThreadDestroyEvent, mReliableUdpThreadWakeUpEvent };

	char RemoteAddress[32] = { 0, };
	int RemotePort = 0;
	byte Data[MAX_BUFFER_LENGTH] = { 0, };
	unsigned long DataLength = 0;
	void *object = NULL;

	while (true)
	{
		SetEvent(mReliableUdpThreadStartupEvent);
		EventID = WaitForMultipleObjects(2, ThreadEvents, false, INFINITE);
		switch (EventID)
		{
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
		NEXT_DATA:
			if (mReliableWriteQueue.Pop(&object, Data, DataLength, RemoteAddress, RemotePort))
			{
			RETRY:
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength))
				{
					return;
				}
				unsigned long Result = WaitForSingleObject(mReliableUdpWriteCompleteEvent, 10);
			}
		}
	}
}