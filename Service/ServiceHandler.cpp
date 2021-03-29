#include "StdAfx.h"
#include "ServiceHandler.h"

DWORD WINAPI ServiceHandler(DWORD dwControl,
                            DWORD dwEventType,
                            LPVOID lpEventData,
                            LPVOID lpContext) {
  switch (dwControl) {
    case SERVICE_CONTROL_STOP:
    case SERVICE_CONTROL_SHUTDOWN:
      gServiceStatus.dwCurrentState = SERVICE_STOPPED;
      break;
    case SERVICE_CONTROL_PAUSE:
      gServiceStatus.dwCurrentState = SERVICE_PAUSED;
      break;
    case SERVICE_CONTROL_CONTINUE:
      gServiceStatus.dwCurrentState = SERVICE_RUNNING;
      break;
    case SERVICE_CONTROL_INTERROGATE:
      break;
    default:
      break;
  };

  if (!SetServiceStatus(gServiceStatusHandle, &gServiceStatus)) {
    OutputDebugStringW(L"SetServiceStatus failed.");
  }

  return NO_ERROR;
}