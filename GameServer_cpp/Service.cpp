#include "Service.h"
/*
bool CService::Install(std::string &serviceName)
{
	TCHAR ServiceFileName[MAX_PATH] = { 0, };
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (serviceName.empty())
	{
		return false;
	}

	ServiceFileName[0] = '"';
	GetModuleFileName(NULL, ServiceFileName + 1, MAX_PATH - 1);
	_tcscat(ServiceFileName, _T("\""));

	//서비스 생성 옵션으로 SCM을 오픈
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
	{
		return false;
	}

	ServiceHandle = CreateService(ServiceControlManager, serviceName.c_str(), serviceName.c_str, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, ServiceFileName, NULL, NULL, NULL, NULL, NULL);
	if (!ServiceHandle)
	{
		//실패
		CloseServiceHandle(ServiceControlManager);
		return false;
	}

	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return true;
}

bool CService::Uninstall(std::string &serviceName)
{
	SC_HANDLE ServiceControlManager = NULL;
	SC_HANDLE ServiceHandle = NULL;

	if (serviceName.empty())
	{
		return false;
	}

	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
	{
		return false;
	}

	ServiceHandle = OpenService(ServiceControlManager, serviceName.c_str, DELETE);

	if (!ServiceHandle)
	{
		CloseServiceHandle(ServiceControlManager);
		return false;
	}

	if (!DeleteService(ServiceHandle))
	{
		CloseServiceHandle(ServiceHandle);
		CloseServiceHandle(ServiceControlManager);

		return false;
	}

	CloseServiceHandle(ServiceHandle);
	CloseServiceHandle(ServiceControlManager);

	return true;
}

bool CService::Begin(std::string &serviceName)
{
	if (serviceName.empty())
	{
		return false;
	}
	//StartServiceDispatcher에서 등록할 서비스 환경 정보
	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		//{serviceName.c_str, ::RunCallback},
		//서비스 이름과  서비스가 실행되었을 때 호출될 콜백
		{NULL, NULL}
	};

	_tcscpy(mServiceName.c_str, serviceName.c_str);

	OnStarted(); //가상함수
	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		//실패일경우
		_tprintf(_T(" ## Debig console mode ##\n"));
	}

	return true;
}

bool CService::End(void)
{
	OnStopped();

	return true;
}

void CService::RunCallback(unsigned long argumentCount, std::string *arguments)
{
	unsigned long Status;

	mServiceStatus.dwServiceType = SERVICE_WIN32;
	mServiceStatus.dwCurrentState = SERVICE_START_PENDING;
	mServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE;

	mServiceStatus.dwWin32ExitCode = 0;
	mServiceStatus.dwServiceSpecificExitCode = 0;
	mServiceStatus.dwCheckPoint = 0;
	mServiceStatus.dwWaitHint = 0;

	//상태를 service_start_pending으로 해놓고 control callback을 등록
	//mServiceStatusHandle = RegisterServiceCtrlHandler(mServiceName.c_str, ::CtrlHandlerCallback);
}

*/