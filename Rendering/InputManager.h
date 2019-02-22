#pragma once

class InputManager
{
public:
	InputManager();

	void SetKeyDown(unsigned int);
	void SetKeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
	bool IsKeyUp(unsigned int);

	bool Initialize();
	void Release();

private:
	const static int KEY_STATE_COUNT = 256;
	bool keyStates[InputManager::KEY_STATE_COUNT];
};

