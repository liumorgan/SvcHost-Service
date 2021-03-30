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

#pragma once

#include <string>

class ServiceInstaller {
 public:
  ServiceInstaller(const std::wstring& serviceName, bool isX64Service);
  virtual ~ServiceInstaller();

  bool Install(const std::wstring& dllPath, const std::wstring& displayName, const std::wstring& description);
  bool Uninstall();
  bool Start();
  bool Stop();

 protected:
  bool isX64Service_;
  std::wstring serviceName_;
};