
#include "memory.h"

static DWORD defaultBase = 0x0400000;
static DWORD imageBase = 0;
static DWORD imageSize = 0;

void queryImage(void)
{
    MODULEINFO moduleInfo;
    if (GetModuleInformation(GetCurrentProcess(), GetModuleHandle(NULL), &moduleInfo, sizeof(moduleInfo)))
    {
        imageBase = (DWORD)moduleInfo.lpBaseOfDll;
        imageSize = moduleInfo.SizeOfImage;
    }
}

DWORD convertAddress(DWORD addr)
{
    return imageBase - defaultBase + addr;
}

// Based on SpecialK's SK_Scan
void *findAddress(PBYTE pattern, BYTE len)
{
    if (!imageBase || !imageSize)
        return NULL;

    PBYTE base = (PBYTE)imageBase;
    PBYTE cur = base;
    PBYTE end = base + imageSize;
    BYTE i = 0;

    while (cur < end - len)
    {
        if (*cur == pattern[i])
        {
            if (++i == len)
                return base;
            cur++;
        }
        else
        {
            cur = ++base;
            i = 0;
        }
    }

    return NULL;
}

void detourFunction(void *addr, void *hook)
{
    DWORD oldProt;
    VirtualProtect(addr, 7, PAGE_READWRITE, &oldProt);
    *(BYTE *)addr = 0xB8;
    *(DWORD *)(addr + 1) = (DWORD)(hook);
    *(WORD *)(addr + 5) = 0xE0FF;
    VirtualProtect(addr, 7, oldProt, NULL);
}
