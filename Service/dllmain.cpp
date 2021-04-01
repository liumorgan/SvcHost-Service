#include "SvcHostService.h"
#include <strsafe.h>

#define SERVICE_EXPORTS
#ifdef SERVICE_EXPORTS
#define SERVICE_API __declspec(dllexport)
#else
#define SERVICE_API __declspec(dllimport)
#endif

SvcHostService* gService = NULL;

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

      if (!SvcHostService::UpdateServiceRunningStatus(SERVICE_STOPPED)) {
      }
      break;
    case SERVICE_CONTROL_SHUTDOWN:
      OutputDebugStringW(L"[svcs] SERVICE_CONTROL_SHUTDOWN\n");
      if (pService) {
        pService->Stop();
        delete pService;
      }

      if (!SvcHostService::UpdateServiceRunningStatus(SERVICE_STOPPED)) {
      }
      break;
    case SERVICE_CONTROL_PAUSE:
      OutputDebugStringW(L"[svcs] SERVICE_CONTROL_PAUSE\n");
      if (pService) {
        pService->Pause();
      }
      if (!SvcHostService::UpdateServiceRunningStatus(SERVICE_PAUSED)) {
      }
      break;
    case SERVICE_CONTROL_CONTINUE:
      OutputDebugStringW(L"[svcs] SERVICE_CONTROL_CONTINUE\n");
      if (pService) {
        pService->Resume();
      }
      if (!SvcHostService::UpdateServiceRunningStatus(SERVICE_RUNNING)) {
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
  if (lpszArgv[0]) {
    StringCchCopyW(szServiceName, MAX_PATH, lpszArgv[0]);
  }

  if (wcslen(szServiceName) == 0) {
    OutputDebugStringW(L"[svcs] Service name is empty!\n");
    return;
  }

  SvcHostService::ServiceStatusHandle =
      RegisterServiceCtrlHandlerExW(szServiceName, ServiceCtrlHandler, NULL);
  if (!SvcHostService::ServiceStatusHandle) {
    OutputDebugStringW(L"[svcs] RegisterServiceCtrlHandlerExW failed!\n");
    return;
  }

  SvcHostService::UpdateServiceRunningStatus(SERVICE_RUNNING);

  if (gService) {
    gService->Run();
  }
}

// Only for test
//
extern "C" SERVICE_API void __ServiceMainTest(DWORD dwArgc, LPCWSTR* lpszArgv) {
  WCHAR szCmd[MAX_PATH] = {0};
  if (lpszArgv[0]) {
    StringCchCopyW(szCmd, MAX_PATH, lpszArgv[0]);
  }

  if (wcslen(szCmd) == 0) {
    OutputDebugStringW(L"[svcs] command is empty!\n");
    return;
  }

  if (wcscmp(szCmd, L"start") == 0) {
    if (gService) {
      gService->Run();
    }
  }
  else if (wcscmp(szCmd, L"pause") == 0) {
    if (gService) {
      gService->Pause();
    }
  }
  else if (wcscmp(szCmd, L"resume") == 0) {
    if (gService) {
      gService->Resume();
    }
  }
  else if (wcscmp(szCmd, L"stop") == 0) {
    if (gService) {
      gService->Stop();
    }
  }
}