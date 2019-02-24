#pragma once

class InputManager
{
public:
	bool Initialize();
	void Release();

	void SetKeyDown(unsigned int);
	void SetKeyUp(unsigned int);

	bool IsKeyDown(unsigned int);
	bool IsKeyUp(unsigned int);

private:
	const static int KEY_STATE_COUNT = 256;
	bool keyStates[InputManager::KEY_STATE_COUNT];
};

