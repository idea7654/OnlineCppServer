#include "NetworkSession.h"

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