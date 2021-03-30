﻿/*******************************************************************************
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

#ifndef REGISTRY_H_23DRF_
#define REGISTRY_H_23DRF_
#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <vector>
#include <string>

class RegKey {
 public:
  // hkeyRoot can be one of :
  // HKEY_CLASSES_ROOT
  // HKEY_CURRENT_CONFIG
  // HKEY_CURRENT_USER
  // HKEY_LOCAL_MACHINE
  // HKEY_USERS
  //
  RegKey(HKEY hkeyRoot, LPCWSTR pszSubKey);
  ~RegKey(void);

  // samDesired:
  // https://docs.microsoft.com/zh-cn/windows/desktop/SysInfo/registry-key-security-and-access-rights
  // samDesired can be one of: KEY_ALL_ACCESS, KEY_QUERY_VALUE, KEY_READ, KEY_WRITE,
  // KEY_WOW64_32KEY, KEY_WOW64_64KEY and so on.
  //
  HRESULT Open(REGSAM samDesired, bool bCreate);
  bool IsOpen(void) const;
  HKEY GetHandle(void) const;
  void Attach(HKEY hkey);
  void Detach(void);
  void Close(void);
  HRESULT WatchForChange(DWORD dwChangeFilter, bool bWatchSubtree);
  HRESULT WaitForChange(DWORD dwChangeFilter, bool bWatchSubtree);
  static bool DeleteKey(HKEY hKey, LPCWSTR pszSubKey, LPCWSTR pszValName, bool bPrefer64View);
  static bool DeleteSubKeys(HKEY hKeyRoot, LPCTSTR lpSubKey, bool bPrefer64View);
  HRESULT GetDWORDValue(LPCWSTR pszValueName, DWORD* pdwDataOut) const;
  HRESULT GetBINARYValue(LPCWSTR pszValueName, LPBYTE pbDataOut, int cbDataOut) const;
  HRESULT GetSZValue(LPCWSTR pszValueName, OUT std::wstring& strValue) const;
  HRESULT GetMultiSZValue(LPCWSTR pszValueName, OUT std::vector<std::wstring>& vStrValues) const;
  int GetValueBufferSize(LPCWSTR pszValueName) const;
  HRESULT SetDWORDValue(LPCWSTR pszValueName, DWORD dwData);
  HRESULT SetBINARYValue(LPCWSTR pszValueName, const LPBYTE pbData, int cbData);
  HRESULT SetSZValue(LPCWSTR pszValueName, const std::wstring& strData);
  HRESULT SetExpandSZValue(LPCWSTR pszValueName, const std::wstring& strData);
  HRESULT SetMultiSZValue(LPCWSTR pszValueName, const std::vector<std::wstring>& vStrValues);

  HRESULT GetSubKeys(std::vector<std::wstring>& subKeys);

 protected:
  void OnChange(HKEY hkey);

 private:
  RegKey(const RegKey& rhs);
  RegKey& operator=(const RegKey& rhs);
  HKEY m_hkeyRoot;
  mutable HKEY m_hkey;
  HANDLE m_hChangeEvent;
  HANDLE m_hNotifyThr;
  DWORD m_dwChangeFilter;
  std::wstring m_strSubKey;
  bool m_bWatchSubtree;

  HRESULT GetValue(LPCWSTR pszValueName, DWORD dwTypeExpected, LPBYTE pbData, DWORD cbData) const;
  HRESULT SetValue(LPCWSTR pszValueName, DWORD dwValueType, const LPBYTE pbData, int cbData);
  LPWSTR CreateDoubleNulTermList(const std::vector<std::wstring>& vStrValues) const;
  static unsigned int _stdcall NotifyWaitThreadProc(LPVOID pvParam);
  static bool RegDeleteKey32_64(HKEY hKey, LPCWSTR pszSubKey, bool bPrefer64View);
  static bool RegDeleteSubKeys(HKEY hKey, bool bPrefer64View);
  static BOOL RegDelSubKeysRecurse(HKEY hKeyRoot, LPTSTR lpSubKey, bool bPrefer64View);
};
#endif  // ! REGISTRY_H_23DRF_
