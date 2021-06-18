#pragma once
#include <string>
#include <tchar.h>
#include <Windows.h>
class CService
{
	std::string mServiceName;
	SERVICE_STATUS mServiceStatus;
	SERVICE_STATUS_HANDLE mServiceStatusHandle;
	bool Install(std::string &serviceName);
	bool Uninstall(std::string &serviceName);
	bool Begin(std::string &serviceName);
	bool End(void);
	void RunCallback(unsigned long argumentCouint, std::string *arguments);
};

void OnStarted(){}

void OnStopped(){}
