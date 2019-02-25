#include "InputManager.h"

bool InputManager::Initialize()
{
	// Filling the entire keyStates array with false
	FillMemory(keyStates, sizeof(keyStates), false);

	return true;
}

void InputManager::Release()
{
}

void InputManager::SetKeyDown(unsigned int keyNum)
{
	keyStates[keyNum] = true;
}

void InputManager::SetKeyUp(unsigned int keyNum)
{
	keyStates[keyNum] = false;
}

bool InputManager::IsKeyDown(unsigned int keyNum)
{
	return keyStates[keyNum];
}

bool InputManager::IsKeyUp(unsigned int keyNum)
{
	return !IsKeyDown(keyNum);
}
