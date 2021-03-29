#pragma once

#include <string>

class ServiceInstaller {
 public:
  ServiceInstaller(const std::wstring& serviceName);
  virtual ~ServiceInstaller();

  bool Install(const std::wstring& dllPath);
  bool Uninstall();
  bool Start();
  bool Stop();

 protected:
  std::wstring serviceName_;
};