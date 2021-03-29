#include "StdAfx.h"
#include "EventLog.h"

void EventLogTraceW(LPCWSTR pszServiceName, WORD wType, LPCWSTR lpFormat, ...) {
  if (!lpFormat)
    return;

  wchar_t* pMsgBuffer = NULL;
  unsigned int iMsgBufCount = 0;

  va_list arglist;
  va_start(arglist, lpFormat);
  HRESULT hr = STRSAFE_E_INSUFFICIENT_BUFFER;

  while (hr == STRSAFE_E_INSUFFICIENT_BUFFER) {
    iMsgBufCount += 1024;

    if (pMsgBuffer) {
      free(pMsgBuffer);
      pMsgBuffer = NULL;
    }

    pMsgBuffer = (wchar_t*)malloc(iMsgBufCount * sizeof(wchar_t));

    if (!pMsgBuffer) {
      break;
    }

    hr = StringCchVPrintfW(pMsgBuffer, iMsgBufCount, lpFormat, arglist);
  }

  va_end(arglist);

  if (hr == S_OK) {
    if (pMsgBuffer) {
      HANDLE hEventSource = NULL;
      LPCWSTR lpszStrings[2] = {NULL, NULL};

      hEventSource = RegisterEventSource(NULL, pszServiceName);
      if (hEventSource) {
        lpszStrings[0] = pszServiceName;
        lpszStrings[1] = pMsgBuffer;

        ReportEvent(hEventSource,  // Event log handle
                    wType,         // Event type
                    0,             // Event category
                    0,             // Event identifier
                    NULL,          // No security identifier
                    2,             // Size of lpszStrings array
                    0,             // No binary data
                    lpszStrings,   // Array of strings
                    NULL           // No binary data
        );

        DeregisterEventSource(hEventSource);
      }
    }
  }

  if (pMsgBuffer) {
    free(pMsgBuffer);
    pMsgBuffer = NULL;
  }
}

void EventLogTraceA(LPCSTR pszServiceName, WORD wType, LPCSTR lpFormat, ...) {
  if (!lpFormat)
    return;

  char* pMsgBuffer = NULL;
  unsigned int iMsgBufCount = 0;

  va_list arglist;
  va_start(arglist, lpFormat);
  HRESULT hr = STRSAFE_E_INSUFFICIENT_BUFFER;

  while (hr == STRSAFE_E_INSUFFICIENT_BUFFER) {
    iMsgBufCount += 1024;

    if (pMsgBuffer) {
      free(pMsgBuffer);
      pMsgBuffer = NULL;
    }

    pMsgBuffer = (char*)malloc(iMsgBufCount * sizeof(char));

    if (!pMsgBuffer) {
      break;
    }

    hr = StringCchVPrintfA(pMsgBuffer, iMsgBufCount, lpFormat, arglist);
  }

  va_end(arglist);

  if (hr == S_OK) {
    if (pMsgBuffer) {
      HANDLE hEventSource = NULL;
      LPCSTR lpszStrings[2] = {NULL, NULL};

      hEventSource = RegisterEventSourceA(NULL, pszServiceName);
      if (hEventSource) {
        lpszStrings[0] = pszServiceName;
        lpszStrings[1] = pMsgBuffer;

        ReportEventA(hEventSource,  // Event log handle
                     wType,         // Event type
                     0,             // Event category
                     0,             // Event identifier
                     NULL,          // No security identifier
                     2,             // Size of lpszStrings array
                     0,             // No binary data
                     lpszStrings,   // Array of strings
                     NULL           // No binary data
        );

        DeregisterEventSource(hEventSource);
      }
    }
  }

  if (pMsgBuffer) {
    free(pMsgBuffer);
    pMsgBuffer = NULL;
  }
}