#include <Windows.h>
#include <iostream>
#include <thread>
#include "Helper.h"
#include "Invoker.h"
#include <vector>

HMODULE ProjecthModule;

DWORD __stdcall EjectThread(LPVOID lpParameter) {
    Sleep(100);
    FreeLibraryAndExitThread(ProjecthModule, 0);
}

struct Vector3
{
    float x, y, z;
};

DWORD WINAPI MainThread(HMODULE hModule) {

    // CONFIGURATION //

    AllocConsole();
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    std::cout << "FiveM Cheat - Varsovie\n" << std::endl;
    std::cout << "[1] Get Player Informations" << std::endl;
    std::cout << "[2] GodMode" << std::endl;

    Sleep(2000);

    // IF YOU WANT A CUSTOM ADDRESS //
    // DWORD moduleBase = (DWORD)GetModuleHandle("MODULE_NAME.exe");

    // FEATURES //

    std::atomic<bool> GodMode(false);

    // START //

    while (!GetAsyncKeyState(VK_INSERT)) {
        if (GetAsyncKeyState(VK_NUMPAD1)) {
            std::int32_t player = NativeInvoke::Invoke<0xD80958FC74E988A6, std::int32_t>();

            Vector3 pos = NativeInvoke::Invoke<0x3FEF770D40960D5A, Vector3>(player, false);

            float speed = NativeInvoke::Invoke<0xD5037BA82E12416F, float>(player);

            int health = NativeInvoke::Invoke<0xEEF059FAD016D209, int>(player);

            Vector3 velocity = NativeInvoke::Invoke<0x4805D2B1D8CF94A9, Vector3>(player);

            std::cout << "\nPlayer ID: " << player << std::endl;
            std::cout << "Coordinates: (" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
            std::cout << "Speed: " << speed << std::endl;
            std::cout << "Health: " << health << std::endl;
            std::cout << "Velocity: (" << velocity.x << ", " << velocity.y << ", " << velocity.z << ")\n" << std::endl;
        }
        if (GetAsyncKeyState(VK_NUMPAD2)) {
            GodMode = !GodMode;
            std::int32_t player = NativeInvoke::Invoke<0xD80958FC74E988A6, std::int32_t>();
            if (GodMode) {
                NativeInvoke::Invoke<0x3882114BDE571AD4, bool>(player, true);
                std::cout << "[+] GodMode" << std::endl;
            }
            else {
                NativeInvoke::Invoke<0x3882114BDE571AD4, bool>(player, false);
                std::cout << "[-] GodMode" << std::endl;
            }
        }
        Sleep(100);
    }

    // STOP //

    printf("Extinction de la console...");

    fclose(fp);
    FreeConsole();
    CreateThread(0, 0, EjectThread, 0, 0, 0);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        ProjecthModule = hModule;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}