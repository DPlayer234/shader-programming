#pragma once
#include <Windows.h>
#include "InputManager.h"
#include "GraphicsManager.h"

#define WIN32_LEAN_AND_MEAN

class GraphicsManager;

// This class handles creating/removing the window as well as handling other components
class System
{
public:
	// Initializes the System
	bool Initialize();

	// Releases all memory associated with this object
	void Release();

	// Runs the application
	void Run();

	// Handles incoming windows messages
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

private:
	// Executes one frame in the window
	bool Frame();

	// Initializes the window and writes its width/height to the parameters
	void InitializeWindow(int& width, int& height);

	// Shuts down/closes the window
	void ShutdownWindow();

	// The name of the application
	LPCWSTR applicationName;

	// The handle to the opened window
	HWND windowHandle = nullptr;

	// The module handle instance
	HINSTANCE hInstance = nullptr;

	// The InputManager in use
	InputManager* input = nullptr;

	// The GraphicsManager in use
	GraphicsManager* graphics = nullptr;

	/* High Frequency Timer */

	// Initializes the high frequency timer
	bool InitializeHighFrequencyTimer();

	// Updates the high frequency timer
	bool UpdateHighFrequencyTimer();

	/* Self-explanatory variables for the high frequency timer */

	float secsPerTick = 0.0f;
	float deltaTime = 0.0f;
	int framesPerSecond = 0;
	int frameCounter = 0;
	int framesPerSecondCounted = 0;
	float frameTimer = 0.0f;
	LARGE_INTEGER oldTicks;
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static System* ApplicationHandle;

