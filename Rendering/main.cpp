#include "System.h"
#include "defines.h"

// WinMain - Program Entry Point
// hInstance - Handle to the current instance
// hPrevInstance - Unused since Win16
// pSCmdLine - Command line command
// iCmnShow - Flag indicating whether the window is supposed to be minimized, maximized or fullscreen
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pSCmdLine, int iCmdShow)
{
	System* system = new System;
	if (!system) return ERROR_INVALID_DATA;

	if (system->Initialize())
	{
		system->Run();
	}

	RELEASE(system);

	return ERROR_SUCCESS;
}
