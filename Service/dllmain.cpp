#include "SvcHostService.h"
#include <strsafe.h>

#define SERVICE_EXPORTS
#ifdef SERVICE_EXPORTS
#define SERVICE_API __declspec(dllexport)
#else
#define SERVICE_API __declspec(dllimport)
#endif

SvcHostService* gService = NULL;
SERVICE_STATUS_HANDLE gServiceStatusHandle = NULL;

namespace {
bool UpdateServiceRunningStatus(DWORD runningStatus) {
  if (!gServiceStatusHandle)
    return false;

  SERVICE_STATUS serviceStatus = {
      SERVICE_WIN32_SHARE_PROCESS,
      SERVICE_START_PENDING,
      SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_PAUSE_CONTINUE};

  serviceStatus.dwCurrentState = runningStatus;

  return !!SetServiceStatus(gServiceStatusHandle, &serviceStatus);
}
}  // namespace

// Dll Main
//
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      OutputDebugStringW(L"[svcs] DLL_PROCESS_ATTACH");
      gService = new SvcHostService();
      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
    case DLL_PROCESS_DETACH:
      OutputDebugStringW(L"[svcs] DLL_PROCESS_DETACH");
      if (gService) {
        delete gService;
        gService = NULL;
      }
      break;
  }
  return TRUE;
}


static DWORD WINAPI ServiceCtrlHandler(DWORD dwControl,
                                       DWORD dwEventType,
                                       LPVOID lpEventData,
                                       LPVOID lpContext) {
  SvcHostService* pService = gService;

  switch (dwControl) {
    case SERVICE_CONTROL_STOP:
      OutputDebugStringW(L"[svcs] SERVICE_CONTROL_STOP\n");
      if (pService) {
        pService->Stop();
      }

      if (!UpdateServiceRunningStatus(SERVICE_STOPPED)) {
      }
      break;
    case SERVICE_CONTROL_SHUTDOWN:
      OutputDebugStringW(L"[svcs] SERVICE_CONTROL_SHUTDOWN\n");
      if (pService) {
        pService->Stop();
        delete pService;
      }

      if (!UpdateServiceRunningStatus(SERVICE_STOPPED)) {
      }
      break;
    case SERVICE_CONTROL_PAUSE:
      OutputDebugStringW(L"[svcs] SERVICE_CONTROL_PAUSE\n");
      if (pService) {
        pService->Pause();
      }
      if (!UpdateServiceRunningStatus(SERVICE_PAUSED)) {
      }
      break;
    case SERVICE_CONTROL_CONTINUE:
      OutputDebugStringW(L"[svcs] SERVICE_CONTROL_CONTINUE\n");
      if (pService) {
        pService->Resume();
      }
      if (!UpdateServiceRunningStatus(SERVICE_RUNNING)) {
      }
      break;
    default:
      break;
  };

  return NO_ERROR;
}

// svchost.exe will call this function.
//
extern "C" SERVICE_API void ServiceMain(DWORD dwArgc, LPCWSTR* lpszArgv) {
  WCHAR szServiceName[MAX_PATH] = {0};
  StringCchCopyW(szServiceName, MAX_PATH, lpszArgv[0]);

  gServiceStatusHandle =
      RegisterServiceCtrlHandlerExW(szServiceName, ServiceCtrlHandler, NULL);
  if (!gServiceStatusHandle) {
    return;
  }

  UpdateServiceRunningStatus(SERVICE_RUNNING);

  if (gService) {
    gService->Run();
  }
}