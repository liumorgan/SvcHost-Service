/*******************************************************************************
 * Copyright (C) 2021 - 2026, winsoft666, <winsoft666@outlook.com>.
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Expect bugs
 *
 * Please use and enjoy. Please let me know of any bugs/improvements
 * that you have found/implemented and I will fix/incorporate them into this
 * file.
 *******************************************************************************/

#include "ServiceInstaller.h"
#include "Registry.h"

namespace {
const std::wstring kGroupSuffix = L"Group";
}

ServiceInstaller::ServiceInstaller(const std::wstring &serviceName, bool isX64Service)
    : serviceName_(serviceName), isX64Service_(isX64Service) {}

ServiceInstaller::~ServiceInstaller() {}

bool ServiceInstaller::Install(const std::wstring &dllPath,
                               const std::wstring &displayName,
                               const std::wstring &description) {
  const std::wstring svcHostGroupName = serviceName_ + kGroupSuffix;
  do {
    HRESULT hr;
    RegKey regKey(HKEY_LOCAL_MACHINE,
                  L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost");
    REGSAM samDesired = KEY_ALL_ACCESS;
    if (isX64Service_)
      samDesired |= KEY_WOW64_64KEY;
    else
      samDesired |= KEY_WOW64_32KEY;

    hr = regKey.Open(samDesired, false);
    if (hr != S_OK) {
      return false;
    }

    hr = regKey.SetMultiSZValue(svcHostGroupName.c_str(), {serviceName_});
    if (hr != S_OK) {
      return false;
    }
  } while (false);

  bool bRet = false;
  SC_HANDLE schSCManager = NULL;
  SC_HANDLE schService = NULL;
  do {
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_CREATE_SERVICE);
    if (!schSCManager) {
      break;
    }

    std::wstring imagePath = L"%SystemRoot%\\System32\\svchost.exe -k " + svcHostGroupName;

    // Install the service into SCM by calling CreateService
    schService =
        CreateService(schSCManager,                 // SCManager database
                      serviceName_.c_str(),         // Name of service
                      displayName.c_str(),          // Name to display
                      SERVICE_ALL_ACCESS,           // Desired access
                      SERVICE_WIN32_SHARE_PROCESS,  // Service type
                      SERVICE_AUTO_START,           // Service start type
                      SERVICE_ERROR_NORMAL,         // Error control type
                      imagePath.c_str(),            // Service's binary
                      NULL,                         // No load ordering group
                      NULL,                         // No tag identifier
                      NULL,                         // Dependencies
                      NULL,                         // Service running account
                      NULL                          // Password of the account
        );
    if (!schService) {
      break;
    }

    // Set service description
    if (description.length() > 0) {
      WCHAR *pDesc = (WCHAR *)malloc((description.length() + 1) * sizeof(WCHAR));
      memset(pDesc, 0, (description.length() + 1) * sizeof(WCHAR));
      memcpy(pDesc, description.c_str(), description.length() * sizeof(WCHAR));
      SERVICE_DESCRIPTION ServiceDesc;
      ServiceDesc.lpDescription = pDesc;
      ::ChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &ServiceDesc);
    }
    bRet = true;
  } while (false);

  // Centralized cleanup for all allocated resources.
  if (schSCManager) {
    CloseServiceHandle(schSCManager);
    schSCManager = NULL;
  }
  if (schService) {
    CloseServiceHandle(schService);
    schService = NULL;
  }

  if (!bRet)
    return false;

  do {
    HRESULT hr;
    RegKey regKey(HKEY_LOCAL_MACHINE,
                  (L"SYSTEM\\CurrentControlSet\\Services\\" + serviceName_ + L"\\Parameters").c_str());
    REGSAM samDesired = KEY_ALL_ACCESS | KEY_WOW64_64KEY;
    hr = regKey.Open(samDesired, true);
    if (hr != S_OK) {
      return false;
    }

    hr = regKey.SetExpandSZValue(L"ServiceDll", dllPath);
    if (hr != S_OK) {
      return false;
    }
  } while (false);

  return true;
}

bool ServiceInstaller::Uninstall() {
  bool bRet = false;
  SC_HANDLE schSCManager = NULL;
  SC_HANDLE schService = NULL;
  SERVICE_STATUS ssSvcStatus = {};

  do {
    // Open the local default service control manager database
    schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
    if (schSCManager == NULL) {
      break;
    }

    // Open the service with delete, stop, and query status permissions
    schService = OpenService(schSCManager,
                             serviceName_.c_str(),
                             SERVICE_STOP | SERVICE_QUERY_STATUS | DELETE);
    if (schService == NULL) {
      break;
    }

    // Try to stop the service
    if (ControlService(schService, SERVICE_CONTROL_STOP, &ssSvcStatus)) {
      Sleep(1000);

      while (QueryServiceStatus(schService, &ssSvcStatus)) {
        if (ssSvcStatus.dwCurrentState == SERVICE_STOP_PENDING) {
          Sleep(1000);
        }
        else {
          break;
        }
      }

      // ssSvcStatus.dwCurrentState == SERVICE_STOPPED
    }

    if (!DeleteService(schService)) {
      break;
    }
    bRet = true;
  } while (false);

  if (schSCManager) {
    CloseServiceHandle(schSCManager);
    schSCManager = NULL;
  }
  if (schService) {
    CloseServiceHandle(schService);
    schService = NULL;
  }

  if (!bRet)
    return false;

  const std::wstring svcHostGroupName = serviceName_ + kGroupSuffix;
  return RegKey::DeleteKey(HKEY_LOCAL_MACHINE,
                           L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost",
                           svcHostGroupName.c_str(),
                           isX64Service_);
}

bool ServiceInstaller::Start() {
  if (serviceName_.length() == 0)
    return false;

  SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
  if (schSCManager == NULL)
    return false;

  SC_HANDLE schService = OpenService(schSCManager, serviceName_.c_str(), SERVICE_ALL_ACCESS);
  if (schService == NULL)
    return false;

  SERVICE_STATUS status;
  QueryServiceStatus(schService, &status);

  if (status.dwCurrentState == SERVICE_STOPPED)
    StartService(schService, 0, NULL);

  CloseServiceHandle(schSCManager);
  CloseServiceHandle(schService);

  return true;
}

bool ServiceInstaller::Stop() {
  if (serviceName_.length() == 0)
    return false;

  SC_HANDLE schSCManager = OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
  if (schSCManager == NULL)
    return false;

  SC_HANDLE schService = OpenService(schSCManager, serviceName_.c_str(), SERVICE_ALL_ACCESS);
  if (schService == NULL)
    return false;

  SERVICE_STATUS status;
  QueryServiceStatus(schService, &status);

  if (status.dwCurrentState == SERVICE_RUNNING)
    ControlService(schService, SERVICE_CONTROL_STOP, &status);

  CloseServiceHandle(schSCManager);
  CloseServiceHandle(schService);

  return true;
}
