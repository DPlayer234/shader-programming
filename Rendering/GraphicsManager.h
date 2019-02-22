#pragma once
#include <Windows.h>
#include "DX3D.h"
#include "Camera.h"
#include "ColorShader.h"
#include "Model.h"
#include <vector>

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsManager
{
public:
	GraphicsManager();

	bool Frame();

	bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
	void Update(float totalTime);
	void Release();

	template<class T>
	Model* AddModel();

private:
	bool Render();

	Camera* camera = nullptr;
	DX3D* dx3d = nullptr;
	HWND hwnd;
	std::vector<Model*> models;
};

