#include <Windows.h>
#include <iostream>
#include <thread>
#include "Invoker/Invoker.h"
#include <vector>

HINSTANCE unhookinst;

struct Vector3
{
    float x, y, z;
};

void MainThread(void) {

    // CREATE CONSOLE //

    AllocConsole();
    freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);

    // FEATURES //

    bool GodMode{ false };

    // START //

    std::cout << "[+] Injected !\n" << std::endl;
    std::cout << "[1] Get Player Informations" << std::endl;
    std::cout << "[2] GodMode" << std::endl;

    while (!GetAsyncKeyState(VK_INSERT)) {
        if (GetAsyncKeyState(VK_NUMPAD1)) {
            auto player = NativeInvoke::Invoke<0xD80958FC74E988A6, std::int32_t>();
            auto position = NativeInvoke::Invoke<0x3FEF770D40960D5A, Vector3>(player, false);
            float speed = NativeInvoke::Invoke<0xD5037BA82E12416F, float>(player);
            int health = NativeInvoke::Invoke<0xEEF059FAD016D209, int>(player);

            std::cout << "\nPlayer ID: " << player << std::endl;
            std::cout << "Coordinates: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
            std::cout << "Speed: " << speed << std::endl;
            std::cout << "Health: " << health << std::endl;
        }
        if (GetAsyncKeyState(VK_NUMPAD2)) {
            GodMode = !GodMode;
            auto player = NativeInvoke::Invoke<0xD80958FC74E988A6, std::int32_t>();
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

    FreeConsole();
    fclose(stdout);
    FreeLibraryAndExitThread(unhookinst, 0);
}

BOOL __stdcall DllMain(HINSTANCE instance, unsigned long reason, void* reserved) {
    switch (reason) {
        case DLL_PROCESS_ATTACH:
            unhookinst = instance;
            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0);
        default:
            break;
    }
    return true;
}