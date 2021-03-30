#include <windows.h>
#include <Shlwapi.h>
#include "ServiceInstaller.h"

#pragma comment(lib, "Shlwapi.lib")

int main() {
  bool isX64 = false;
#ifdef _WIN64
  isX64 = true;
#endif

  ServiceInstaller installer(L"DemoService", isX64);

  printf("Press any key to install service\n");
  getchar();

  WCHAR szDllPath[MAX_PATH] = {0};
  GetModuleFileNameW(NULL, szDllPath, MAX_PATH);
  PathRemoveFileSpecW(szDllPath);
  PathCombineW(szDllPath, szDllPath, L"Service.dll");
  if (!installer.Install(szDllPath,
                         L"Demo-Service-Display-Name",
                         L"Demo-Service-Description")) {
    printf("Install service failed, GLE => %d\n", GetLastError());
    return 1;
  }
  printf("Install OK!\n");

  printf("Press any key to start service\n");
  getchar();

  if (!installer.Start()) {
    printf("Start service failed, GLE => %d\n", GetLastError());
    return 1;
  }
  printf("Start OK!\n");

  printf("Press any key to stop service\n");
  getchar();

  if (!installer.Stop()) {
    printf("Stop service failed, GLE => %d\n", GetLastError());
    return 1;
  }
  printf("Stop OK!\n");

  printf("Press any key to uninstall service\n");
  getchar();
  if (!installer.Uninstall()) {
    printf("Uninstall service failed, GLE => %d\n", GetLastError());
    return 1;
  }
  printf("Uninstall OK!\n");

  return 0;
}
