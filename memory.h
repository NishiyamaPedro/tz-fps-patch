#pragma once

#include "includes.h"
#include <psapi.h>

void queryImage(void);
DWORD convertAddress(DWORD addr);
void *findAddress(PBYTE pattern, BYTE len);
void detourFunction(void *addr, void *hook);
