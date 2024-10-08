#pragma once

#include "includes.h"
#include <xinput.h>

typedef DWORD WINAPI (*XInputSetStatePtr)(DWORD, XINPUT_VIBRATION *);
void initXInputWrapper(void);
