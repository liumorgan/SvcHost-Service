#include "StdAfx.h"
#include "ServiceHandler.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      break;

    case DLL_THREAD_ATTACH:
      break;

    case DLL_THREAD_DETACH:
      break;

    case DLL_PROCESS_DETACH:
      break;
  }
  return TRUE;
}

// svchost.exe will call this function.
extern "C" __declspec(dllexport) VOID WINAPI
    ServiceMain(DWORD dwArgc, LPCWSTR* lpszArgv) {
  gServiceStatusHandle =
      RegisterServiceCtrlHandlerExW(SERVICE_NAME, ServiceHandler, nullptr);
  if (!gServiceStatusHandle) {
    return;
  }

  gServiceStatus.dwCurrentState = SERVICE_RUNNING;

  SetServiceStatus(gServiceStatusHandle, &gServiceStatus);
}