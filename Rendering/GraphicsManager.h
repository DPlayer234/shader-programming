#pragma once
#include <Windows.h>
#include "InputManager.h"
#include "DX3D.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class System;

class GraphicsManager
{
	friend class System;

public:
	GraphicsManager();

	bool Frame();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Update(float deltaTime);
	void Release();

	bool IsKeyDown(unsigned int);

	template<class T>
	T* AddModel();

	template<class T>
	T* LoadShader();

	Texture* LoadTexture(const char* filename);

private:
	bool Render();

	InputManager* input = nullptr;
	Camera* camera = nullptr;
	DX3D* dx3d = nullptr;
	HWND hwnd;

	std::vector<Model*> models;
	std::vector<Shader*> shaders;
	std::vector<Texture*> textures;
};

