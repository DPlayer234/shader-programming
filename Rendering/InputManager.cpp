#include "InputManager.h"

InputManager::InputManager()
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

bool InputManager::Initialize()
{
	for (int i = 0; i < InputManager::KEY_STATE_COUNT; i++)
	{
		keyStates[i] = false;
	}

	return true;
}

void InputManager::Release()
{
}
