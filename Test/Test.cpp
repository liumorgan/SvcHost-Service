#include <windows.h>
#include <Shlwapi.h>
#include <stdio.h>
#include <strsafe.h>
#include <thread>

#pragma comment(lib, "Shlwapi.lib")

typedef void(*PServiceMainTest)(DWORD dwArgc, LPCWSTR* lpszArgv);

int main()
{
  WCHAR szDllPath[MAX_PATH] = { 0 };
  GetModuleFileNameW(NULL, szDllPath, MAX_PATH);
  PathRemoveFileSpecW(szDllPath);
  PathCombineW(szDllPath, szDllPath, L"Service.dll");

  HMODULE hDll = LoadLibraryW(szDllPath);
  if (!hDll) {
    printf("LoadLibrary failed, GLE => %d\n", GetLastError());
    return 1;
  }

  PServiceMainTest pMainFunc = (PServiceMainTest)GetProcAddress(hDll, "__ServiceMainTest");
  if (!pMainFunc) {
    printf("GetProcAddress failed, GLE => %d\n", GetLastError());
    FreeLibrary(hDll);
    return 1;
  }

  printf("Usage:\n start \n stop \n pause \n resume \n exit \n");

  WCHAR szCmd[MAX_PATH] = { 0 };

  do 
  {
    printf(">>");
    fgetws(szCmd, MAX_PATH, stdin);

    // remove \n
    szCmd[wcslen(szCmd) - 1] = 0;
    
    if (wcscmp(szCmd, L"exit") == 0) {
      break;
    }

    LPWSTR p = szCmd;
    std::thread s = std::thread([p, pMainFunc]() {
      pMainFunc(1, (LPCWSTR*)&p);
    });
    s.detach();

    if (wcscmp(szCmd, L"stop") == 0) {
      FreeLibrary(hDll);
      hDll = NULL;
    }
  } while (true);

  if (hDll) {
    FreeLibrary(hDll);
    hDll = NULL;
  }

  return 0;
}