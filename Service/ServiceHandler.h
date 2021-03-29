#pragma once

DWORD WINAPI ServiceHandler(DWORD dwControl,
                       DWORD dwEventType,
                       LPVOID lpEventData,
                       LPVOID lpContext);
