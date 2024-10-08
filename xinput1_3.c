#include "xinput1_3.h"

XInputSetStatePtr realXInputSetState = NULL;

DWORD WINAPI XInputSetState(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)
{
    if (realXInputSetState)
    {
        return realXInputSetState(dwUserIndex, pVibration);
    }

    return ERROR_DEVICE_NOT_CONNECTED;
}

void initXInputWrapper(void)
{
    HMODULE xinput1_3dll;
    char path[MAX_PATH];

    GetSystemDirectoryA(path, MAX_PATH);
    strcat_s(path, MAX_PATH, "\\xinput1_3.dll");

    LOG("loading XInput1_3 from path %s", path);

    xinput1_3dll = LoadLibrary(path);

    if (!xinput1_3dll)
    {
        LOG("can't load the real XInput1_3 dll");
        return;
    }

    realXInputSetState = (XInputSetStatePtr)GetProcAddress(xinput1_3dll, "DirectInput8Create");

    LOG("loaded XInput1_3");
}
