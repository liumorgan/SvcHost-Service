#include "ServiceInstaller.h"
#include "Registry.h"

ServiceInstaller::ServiceInstaller(const std::wstring& serviceName)
    : serviceName_(serviceName) {}

ServiceInstaller::~ServiceInstaller() {}

bool ServiceInstaller::Install(const std::wstring& dllPath) {
  
}

bool ServiceInstaller::Uninstall() {}

bool ServiceInstaller::Start() {}

bool ServiceInstaller::Stop() {}
