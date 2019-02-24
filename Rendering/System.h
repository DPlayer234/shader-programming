#pragma once
#include <Windows.h>
#include "InputManager.h"
#include "GraphicsManager.h"

#define WIN32_LEAN_AND_MEAN

class GraphicsManager;

class System
{
public:
	bool Initialize();
	void Release();

	void Run();
	LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

private:
	bool Frame();
	void InitializeWindow(int& width, int& height);
	void ShutdownWindow();

	LPCWSTR applicationName;
	HWND windowHandle = nullptr;
	HINSTANCE hInstance = nullptr;

	InputManager* input = nullptr;
	GraphicsManager* graphics = nullptr;

	// High Frequency Timer
	bool InitializeHighFrequencyTimer();
	bool UpdateHighFrequencyTimer();

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

