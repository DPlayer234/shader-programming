#pragma once
#include <Windows.h>

// Manages the input state
class InputManager
{
public:
	// Initializes the InputManager
	bool Initialize();

	// Releases all memory associated with this object
	void Release();

	// Sets a key as down/pressed
	void SetKeyDown(unsigned int);

	// Sets a key as up/not pressed
	void SetKeyUp(unsigned int);

	// Returns whether a key is down/pressed
	bool IsKeyDown(unsigned int);

	// Returns whether a key is up/not pressed
	bool IsKeyUp(unsigned int);

private:
	// The total amount of keys
	const static int KEY_STATE_COUNT = 256;

	// Stores the state of all keys
	bool keyStates[InputManager::KEY_STATE_COUNT];
};

