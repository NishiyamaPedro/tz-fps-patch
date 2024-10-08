#include "dllmain.h"

BOOL WINAPI DllMain(HINSTANCE, DWORD reasonForDllLoad, LPVOID)
{
    switch (reasonForDllLoad)
    {
    case DLL_PROCESS_ATTACH:
        initXInputWrapper();
        patchFPS();

        break;
    default:
        break;
    }

    return TRUE;
}
