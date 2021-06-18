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

	//���� ���� �ɼ����� SCM�� ����
	ServiceControlManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (!ServiceControlManager)
	{
		return false;
	}

	ServiceHandle = CreateService(ServiceControlManager, serviceName.c_str(), serviceName.c_str, SERVICE_ALL_ACCESS, SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, ServiceFileName, NULL, NULL, NULL, NULL, NULL);
	if (!ServiceHandle)
	{
		//����
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
	//StartServiceDispatcher���� ����� ���� ȯ�� ����
	SERVICE_TABLE_ENTRY DispatchTable[] =
	{
		//{serviceName.c_str, ::RunCallback},
		//���� �̸���  ���񽺰� ����Ǿ��� �� ȣ��� �ݹ�
		{NULL, NULL}
	};

	_tcscpy(mServiceName.c_str, serviceName.c_str);

	OnStarted(); //�����Լ�
	if (!StartServiceCtrlDispatcher(DispatchTable))
	{
		//�����ϰ��
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

	//���¸� service_start_pending���� �س��� control callback�� ���
	//mServiceStatusHandle = RegisterServiceCtrlHandler(mServiceName.c_str, ::CtrlHandlerCallback);
}

*/