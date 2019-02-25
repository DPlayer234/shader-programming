#pragma once
#include <Windows.h>
#include <vector>
#include "InputManager.h"
#include "DX3D.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Skybox.h"

/* Define various variables describing the render state */
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = false;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;
const float FIELD_OF_VIEW = DirectX::XM_PI * 0.4f;

class System;

// Manages graphics state related operations.
class GraphicsManager
{
public:
	// Initializes the GraphicsManager and all its sub-components
	bool Initialize(int screenWidth, int screenHeight, HWND hwnd, InputManager* input);

	// Releases all memory associated with this object
	void Release();

	// Renders a frame to the screen
	bool Frame();

	// Updates the state. deltaTime describes the time since the last update.
	void Update(float deltaTime);

	// Adds a model to the models vector to be rendered and returns it
	template<class T>
	T* AddModel();

	// Loads a shader, adds it to the shaders vector and returns it
	template<class T>
	T* LoadShader();

	// Loads a texture, adds it to the textures vector and returns it
	Texture* LoadTexture(const char* filename);

private:
	// Wraps input->IsKeyDown
	bool IsKeyDown(unsigned int);

	// Renders a frame; does the actual handling and is called within GraphicsManager::Frame()
	bool Render();

	// The input manager.
	InputManager* input = nullptr;

	// The camera used for screen-space projections
	Camera* camera = nullptr;

	// The DX3D Object used for rendering
	DX3D* dx3d = nullptr;

	// The handle to the rendering window
	HWND hwnd = 0;

	// The pointer to the skybox model
	Skybox* skybox = nullptr;

	// Position of the light
	Float3 lightPos = Float3(100.0f, 100.0f, -100.0f);

	// Model representing the light
	Model* lightModel = nullptr;

	/* Storage vectors */

	// Stores all created models
	std::vector<Model*> models;

	// Stores all loaded shaders
	std::vector<Shader*> shaders;

	// Stores all loaded textures
	std::vector<Texture*> textures;
};

