#include "fps.h"

static DWORD timestep = 0x0217b464;
static DWORD fpsConst = 0x00D977B8;
static DWORD frameTime1 = 0x021308c0;
static DWORD frameTime2 = 0x00d97818;
static DWORD frameTimePhys = 0x00d562d8;
static DWORD updateTime1 = 0x021308b0;
static DWORD updateTime2 = 0x0213089c;

static double lastTime;
static LARGE_INTEGER perfFrequency;
static LARGE_INTEGER perfCounter;

void calcFrameTimeHook(void)
{
    // Calculate the frame time instead of using the fixed game values
    // This will hopefully fix the physics in scenarios like the priest in Lastonbell,
    // he will get a bit stuck but will reach the top of the stairs
    LARGE_INTEGER perfCounterNow;
    QueryPerformanceCounter(&perfCounterNow);
    double currentTime = (double)((perfCounterNow.QuadPart - perfCounter.QuadPart) * 1000.0 / (double)perfFrequency.QuadPart);
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    double FPS = 1.0 / (deltaTime / 1000);
    double frameTime = 1 / FPS;

    DWORD oldProt;

    // Force the time step to always be 1
    VirtualProtect((LPVOID)timestep, 8, PAGE_READWRITE, &oldProt);
    *(DWORD *)(timestep) = 0x1;
    *(DWORD *)(timestep + 4) = 0x1;
    VirtualProtect((LPVOID)timestep, 8, oldProt, NULL);

    // Set updateTime based on framerate
    // The game won't run in slow motion when below target FPS,
    // nor will it run twice as fast when above
    // But to target 60 FPS+, we would need to patch other parts of the game
    VirtualProtect((LPVOID)updateTime1, 4, PAGE_READWRITE, &oldProt);
    *(float *)(updateTime1) = (float)(60.0 / FPS);
    VirtualProtect((LPVOID)updateTime1, 4, oldProt, NULL);
    VirtualProtect((LPVOID)updateTime2, 4, PAGE_READWRITE, &oldProt);
    *(float *)(updateTime2) = (float)(60.0 / FPS);
    VirtualProtect((LPVOID)updateTime2, 4, oldProt, NULL);

    // Set our calculated frame time
    VirtualProtect((LPVOID)frameTime1, 8, PAGE_READWRITE, &oldProt);
    *(float *)(frameTime1) = (float)frameTime;
    *(float *)(frameTime1 + 4) = (float)frameTime;
    VirtualProtect((LPVOID)frameTime1, 8, oldProt, NULL);

    VirtualProtect((LPVOID)frameTime2, 8, PAGE_READWRITE, &oldProt);
    *(double *)(frameTime2) = frameTime;
    VirtualProtect((LPVOID)frameTime2, 8, oldProt, NULL);

    VirtualProtect((LPVOID)frameTimePhys, 4, PAGE_READWRITE, &oldProt);
    *(float *)(frameTimePhys) = (float)frameTime;
    VirtualProtect((LPVOID)frameTimePhys, 4, oldProt, NULL);

    VirtualProtect((LPVOID)fpsConst, 8, PAGE_READWRITE, &oldProt);
    *(double *)(fpsConst) = FPS;
    VirtualProtect((LPVOID)fpsConst, 8, oldProt, NULL);
}

void patchFPS(void)
{
    BYTE calcFrameTimeSig[] = {0x55, 0x8B, 0xEC, 0x83, 0xEC, 0x08, 0x56, 0x8B, 0x35};

    queryImage();

    void *calcFrameTimeAddr = findAddress(calcFrameTimeSig, 9);

    if (!calcFrameTimeAddr)
    {
        LOG("could not find calcFrameTime address, the patch will not be applied");
        return;
    }

    LOG("found calcFrameTime at 0x%08X", calcFrameTimeAddr);
    LOG("patching FPS");

    lastTime = 0;
    QueryPerformanceFrequency(&perfFrequency);
    QueryPerformanceCounter(&perfCounter);

    timestep = convertAddress(timestep);
    fpsConst = convertAddress(fpsConst);
    frameTime1 = convertAddress(frameTime1);
    frameTime2 = convertAddress(frameTime2);
    frameTimePhys = convertAddress(frameTimePhys);
    updateTime1 = convertAddress(updateTime1);
    updateTime2 = convertAddress(updateTime2);

    detourFunction(calcFrameTimeAddr, calcFrameTimeHook);
}
