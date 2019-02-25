#include "System.h"
#include "defines.h"

bool System::Initialize()
{
	bool result;
	int screenWidth = 1;
	int screenHeight = 1;

	InitializeWindow(&screenWidth, &screenHeight);

	input = new InputManager;
	if (!input) return false;
	result = input->Initialize();
	if (!result) return false;

	if (!InitializeHighFrequencyTimer()) return false;

	graphics = new GraphicsManager;
	if (!graphics) return false;
	result = graphics->Initialize(screenWidth, screenHeight, windowHandle, input);
	if (!result) return false;

	return true;
}

void System::Release()
{
	ShutdownWindow();

	RELEASE(input);
	RELEASE(graphics);
}

void System::Run()
{
	MSG msg;
	bool result;
	bool done = false;

	float totalTime = 0.0f;

	ZeroMemory(&msg, sizeof(msg));

	// Game Loop
	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			result = Frame();

			if (!result)
			{
				done = true;
			}

			UpdateHighFrequencyTimer();
			graphics->Update(deltaTime);
		}
	}
}

LRESULT System::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_KEYDOWN:
		input->SetKeyDown((unsigned int)wparam);
		return 0;
	case WM_KEYUP:
		input->SetKeyUp((unsigned int)wparam);
		return 0;
	default:
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
}

bool System::Frame()
{
	if (input->IsKeyDown(VK_ESCAPE))
	{
		return false;
	}

	return graphics->Frame();
}

void System::InitializeWindow(int* width, int* height)
{
	WNDCLASSEX window;
	DEVMODE dmScreenSettings;

	int posX, posY;

	ApplicationHandle = this;

	hInstance = GetModuleHandle(NULL);
	applicationName = L"Direct3D Rendering Engine?";

	// Define the window style
	window.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	window.lpfnWndProc = WndProc;
	window.cbClsExtra = 0;
	window.cbWndExtra = 0;
	window.hInstance = hInstance;
	window.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	window.hIconSm = window.hIcon;
	window.hCursor = LoadCursor(NULL, IDC_HAND);
	window.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	window.lpszMenuName = NULL;
	window.lpszClassName = applicationName;
	window.cbSize = sizeof(window);

	// Register the window class
	ATOM atom = RegisterClassEx(&window);

	// Define size and centered window position
	if (FULL_SCREEN)
	{
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));

		*width = GetSystemMetrics(SM_CXSCREEN);
		*height = GetSystemMetrics(SM_CYSCREEN);

		dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth = *width;
		dmScreenSettings.dmPelsHeight = *height;
		dmScreenSettings.dmBitsPerPel = 24;

		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		posX = posY = 0;
	}
	else
	{
		*width = 800;
		*height = 600;

		posX = (GetSystemMetrics(SM_CXSCREEN) - *width) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - *height) / 2;
	}

	// Create and show window
	windowHandle = CreateWindowEx(
		WS_EX_APPWINDOW,
		applicationName,
		applicationName,
		WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
		posX, posY,
		*width, *height,
		NULL, NULL,
		hInstance,
		NULL);

	ShowWindow(windowHandle, SW_SHOW);
	SetForegroundWindow(windowHandle);
	SetFocus(windowHandle);

	ShowCursor(false);
}

void System::ShutdownWindow()
{
	if (!windowHandle) return;

	ShowCursor(true);

	ChangeDisplaySettings(NULL, 0);
	DestroyWindow(windowHandle);

	windowHandle = NULL;

	UnregisterClass(applicationName, hInstance);
	hInstance = NULL;
}

bool System::InitializeHighFrequencyTimer()
{
	LARGE_INTEGER freq = {};
	if (!QueryPerformanceFrequency(&freq)) return false;
	
	secsPerTick = 1.0f / freq.QuadPart;
	if (!QueryPerformanceCounter(&oldTicks)) return false;

	return true;
}

bool System::UpdateHighFrequencyTimer()
{
	LARGE_INTEGER currentTicks = {};
	QueryPerformanceCounter(&currentTicks);

	deltaTime = (currentTicks.QuadPart - oldTicks.QuadPart) * secsPerTick;

	framesPerSecond = (int)(1.0f / deltaTime);

	frameCounter += 1;
	frameTimer += deltaTime;

	if (frameTimer >= 1.0f)
	{
		frameTimer -= 1.0f;
		framesPerSecondCounted = frameCounter;
		frameCounter = 0;
	}

	oldTicks = currentTicks;

	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	default:
		return ApplicationHandle
			? ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam)
			: 0;
	}
}
