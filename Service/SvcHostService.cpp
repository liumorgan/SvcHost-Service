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

#include "SvcHostService.h"
#include "EventLog.h"
#include "ServiceMain.h"

SERVICE_STATUS_HANDLE SvcHostService::ServiceStatusHandle = NULL;

bool SvcHostService::UpdateServiceRunningStatus(DWORD runningStatus) {
  if (!SvcHostService::ServiceStatusHandle)
    return false;

  SERVICE_STATUS serviceStatus = {
      SERVICE_WIN32_SHARE_PROCESS, SERVICE_START_PENDING,
      SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN |
          SERVICE_ACCEPT_PAUSE_CONTINUE};

  serviceStatus.dwCurrentState = runningStatus;

  return !!SetServiceStatus(SvcHostService::ServiceStatusHandle,
                            &serviceStatus);
}

SvcHostService::SvcHostService() {
  OutputDebugStringW(L"[svcs] Service\n");

  hPauseEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);
  hStopEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);
  hHasStoppedEvent_ = CreateEvent(NULL, TRUE, FALSE, NULL);
}

SvcHostService::~SvcHostService() {
  OutputDebugStringW(L"[svcs] ~Service\n");

  if (hHasStoppedEvent_) {
    WaitForSingleObject(hHasStoppedEvent_, INFINITE);
  }

  if (hPauseEvent_) {
    CloseHandle(hPauseEvent_);
    hPauseEvent_ = NULL;
  }

  if (hStopEvent_) {
    CloseHandle(hStopEvent_);
    hStopEvent_ = NULL;
  }

  if (hHasStoppedEvent_) {
    CloseHandle(hHasStoppedEvent_);
    hHasStoppedEvent_ = NULL;
  }
}

void SvcHostService::Run() {
  OutputDebugStringW(L"[svcs] Run\n");

  ServiceMain(hPauseEvent_, hStopEvent_);

  if (!IsStopped()) {
    if (hStopEvent_)
      SetEvent(hStopEvent_);
    UpdateServiceRunningStatus(SERVICE_STOPPED);
  }

  if (hHasStoppedEvent_) {
    SetEvent(hHasStoppedEvent_);
  }
}

void SvcHostService::Pause() {
  OutputDebugStringW(L"[svcs] Pause\n");

  if (hPauseEvent_) {
    SetEvent(hPauseEvent_);
  }
}

void SvcHostService::Resume() {
  OutputDebugStringW(L"[svcs] Resume\n");

  if (hPauseEvent_) {
    ResetEvent(hPauseEvent_);
  }
}

void SvcHostService::Stop() {
  OutputDebugStringW(L"[svcs] Stop\n");

  if (hStopEvent_) {
    SetEvent(hStopEvent_);
  }
}

bool SvcHostService::IsPaused() {
  return (hPauseEvent_ && WaitForSingleObject(hPauseEvent_, 50) == WAIT_OBJECT_0);
}

bool SvcHostService::IsStopped() {
  return (hStopEvent_ && WaitForSingleObject(hStopEvent_, 50) == WAIT_OBJECT_0);
}
