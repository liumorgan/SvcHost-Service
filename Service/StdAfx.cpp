#include "StdAfx.h"

SERVICE_STATUS_HANDLE gServiceStatusHandle;
SERVICE_STATUS gServiceStatus = {SERVICE_WIN32_SHARE_PROCESS,
                                 SERVICE_START_PENDING,
                                 SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN |
                                     SERVICE_ACCEPT_PAUSE_CONTINUE};