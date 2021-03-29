#pragma once

void EventLogTraceW(LPCWSTR pszServiceName, WORD wType, LPCWSTR lpFormat, ...);
void EventLogTraceA(LPCSTR pszServiceName, WORD wType, LPCSTR lpFormat, ...);