#include "NetworkSession.h"

CNetworkSession::CNetworkSession(void)
{
	ZeroMemory(&mAcceptOverlapped, sizeof(mAcceptOverlapped));
	ZeroMemory(&mReadOverlapped, sizeof(mReadOverlapped));
	ZeroMemory(&mWriteOverlapped, sizeof(mWriteOverlapped));
	ZeroMemory(&mReadBuffer, sizeof(mReadBuffer));
	ZeroMemory(&mUdpRemoteInfo, sizeof(mUdpRemoteInfo)); //udp사용할때 상대주소 정보

	mSocket = NULL;
	mReliableUdpThreadHandle = NULL; //Reliable UDP관련변수
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
	//UDP를 사용할 때 데이터를 보낼 상대 정보를 저장하기 위한 변수
	ZeroMemory(&mUdpRemoteInfo, sizeof(mUdpRemoteInfo));

	mSocket = NULL;

	mReliableUdpThreadHandle = NULL;
	mReliableUdpThreadHandle = NULL; //Reliable UDP관련변수
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

		//스레드가 종료될때까지 무한대기
		//3~5초 기다린 후 응답이 없을경우 terminateThread로 강제종료하는것도 좋음
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
	//TCP_NODELAY옵션
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

	LINGER Linger; //소켓을 닫을 때 현재 보내고 있거나 받는 데이터가 있을 경우 그 데이터를 모두 보낸 후 소켓을 닫음
	//안전 but 종료에 시간 많이걸림
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

	SOCKADDR_IN RemoteAddressInfo; //접속할 주소의 정보를 저장하는 구조체

	RemoteAddressInfo.sin_family = AF_INET;
	RemoteAddressInfo.sin_port = htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr = inet_addr(address.c_str());
	
	//위에 설정된 주소로 접속 시도
	if (WSAConnect(mSocket, (LPSOCKADDR)&RemoteAddressInfo, sizeof(LPSOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSAEWOULDBLOCK) //실패지만 대기상태라고 결과가 나왔을 경우 성공으로 인식
		{
			End();
			return false;
		}
	}

	return true;
}