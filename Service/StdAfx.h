#pragma once
#include <windows.h>
#include <strsafe.h>
#include "EventLog.h"

#define SERVICE_EXPORTS
#ifdef SERVICE_EXPORTS
#define SERVICE_API __declspec(dllexport)
#else
#define SERVICE_API __declspec(dllimport)
#endif

extern SERVICE_STATUS_HANDLE gServiceStatusHandle;

bool UpdateServiceRunningStatus(DWORD runningStatus);