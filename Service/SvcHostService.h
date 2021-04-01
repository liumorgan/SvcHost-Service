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

#ifndef SVCHOST_SERVICE_H_SX382_
#define SVCHOST_SERVICE_H_SX382_
#pragma once

#include <windows.h>

class SvcHostService {
 public:
  SvcHostService();
  virtual ~SvcHostService();

  virtual void Run();

  void Pause();
  void Resume();

  void Stop();

 public:
  static SERVICE_STATUS_HANDLE ServiceStatusHandle;
  static bool UpdateServiceRunningStatus(DWORD runningStatus);
 protected:
  bool IsPaused();
  bool IsStopped();

 protected:
  HANDLE hPauseEvent_;
  HANDLE hStopEvent_;
  HANDLE hHasStoppedEvent_;
};
#endif  //!SVCHOST_SERVICE_H_SX382_