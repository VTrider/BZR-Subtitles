// dllmain.cpp : Defines the entry point for the DLL application.
#include "dx9hook.h"
#include "gui.h"

#include <Windows.h>

#include <exception>
#include <thread>

inline auto p_userObject = (void**)0x00917AFC;

void gui_thread()
{
	try
	{
		gui::Setup();
		hooks::Setup();
	}
	catch (const std::exception& error)
	{
		MessageBeep(MB_ICONERROR);
		MessageBoxA(
			0,
			error.what(),
			"subtitles error",
			MB_OK | MB_ICONEXCLAMATION
		);

		goto UNLOAD;
	}

	while (*p_userObject)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	}

UNLOAD:
	hooks::Destroy();
	gui::Destroy();
	return;
}

BOOL APIENTRY DllMain( [[maybe_unused]] HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       [[maybe_unused]] LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		std::thread t(gui_thread);
		t.detach();
		break;
	}
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

