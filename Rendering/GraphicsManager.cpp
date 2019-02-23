#include "GraphicsManager.h"

#include <math.h>
#include "defines.h"

#include "CubeModel.h"
#include "UnlitShader.h"
#include "BlinnPhongShader.h"

#include "KeyCode.h"

GraphicsManager::GraphicsManager()
{
}

bool GraphicsManager::Frame()
{
	bool result;
	
	result = Render();
	if (!result) return false;

	return true;
}

void GraphicsManager::Update(float deltaTime)
{
	const static float MOVE_SPEED = 5.0f;
	const static float ROT_SPEED = 120.0f;

	Float3 position = camera->GetPosition();
	Float3 rotation = camera->GetRotation();

	float yRRadians = rotation.y * DEG_TO_RAD;
	float fMoveSpeed = MOVE_SPEED * deltaTime;
	float fRotSpeed = ROT_SPEED * deltaTime;

	if (IsKeyDown(KeyCode_W))
	{
		position.x += sin(yRRadians) * fMoveSpeed;
		position.z += cos(yRRadians) * fMoveSpeed;
	}

	if (IsKeyDown(KeyCode_S))
	{
		position.x -= sin(yRRadians) * fMoveSpeed;
		position.z -= cos(yRRadians) * fMoveSpeed;
	}

	if (IsKeyDown(KeyCode_A))
	{
		position.x -= cos(yRRadians) * fMoveSpeed;
		position.z += sin(yRRadians) * fMoveSpeed;
	}

	if (IsKeyDown(KeyCode_D))
	{
		position.x += cos(yRRadians) * fMoveSpeed;
		position.z -= sin(yRRadians) * fMoveSpeed;
	}

	if (IsKeyDown(KeyCode_Q))
	{
		position.y -= fMoveSpeed;
	}

	if (IsKeyDown(KeyCode_E))
	{
		position.y += fMoveSpeed;
	}

	if (IsKeyDown(KeyCode_Left))
	{
		rotation.y -= fRotSpeed;
	}

	if (IsKeyDown(KeyCode_Right))
	{
		rotation.y += fRotSpeed;
	}

	if (IsKeyDown(KeyCode_Up))
	{
		rotation.x -= fRotSpeed;
	}

	if (IsKeyDown(KeyCode_Down))
	{
		rotation.x += fRotSpeed;
	}

	camera->SetPosition(position);
	camera->SetRotation(rotation);
}

bool GraphicsManager::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	this->hwnd = hwnd;

#pragma region DirectX Initialization
	dx3d = new DX3D;
	if (!dx3d) return false;

	result = dx3d->Initialize(
		screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
		FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);

	if (!result)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(hwnd, L"Could not initialize DX3D.", L"Error", MB_OK);
		return false;
	}
#pragma endregion

	camera = new Camera;
	if (!camera) return false;

	camera->SetPosition(Float3(0.0f, 0.8f, -2.0f));
	camera->SetRotation(Float3(20.0f, 0.0f, 0.0f));

#pragma region First Model
	auto model1 = AddModel<CubeModel>();
	auto shader1 = LoadShader<BlinnPhongShader>();
	auto stone01 = LoadTexture("data/stone01.tga");

	if (!model1 || !shader1 || !stone01) return false;

	model1->SetPosition(Float3(0.0f, 0.0f, 0.0f));
	shader1->SetTexture(stone01->GetResourceView());
	model1->SetShader(shader1);
#pragma endregion

	return true;
}

void GraphicsManager::Release()
{
	DELETE_P(camera);
	RELEASE(dx3d);

	for (Model* model : models)
	{
		RELEASE(model);
	}
	
	models.clear();
}

bool GraphicsManager::IsKeyDown(unsigned int key)
{
	return input
		? input->IsKeyDown(key)
		: false;
}

template<class T>
T* GraphicsManager::AddModel()
{
	bool result;

	T* model = new T;
	if (!model) return false;

	result = model->Initialize(dx3d->GetDevice(), this->hwnd);
	if (!result) return nullptr;

	models.push_back(model);

	return model;
}

template<class T>
T* GraphicsManager::LoadShader()
{
	bool result;

	T* shader = new T;
	if (!shader) return nullptr;

	result = shader->Initialize(dx3d->GetDevice(), this->hwnd);
	if (!result) return nullptr;

	shaders.push_back(shader);

	return shader;
}

Texture* GraphicsManager::LoadTexture(const char * filename)
{
	Texture* texture = new Texture;
	if (!texture) return false;

	bool result = texture->Initialize(dx3d->GetDevice(), dx3d->GetDeviceContext(), filename);
	if (!result) return false;

	textures.push_back(texture);

	return texture;
}

bool GraphicsManager::Render()
{
	Matrix view, projection;
	auto context = dx3d->GetDeviceContext();

	// First layer, Background/Skybox/etc.
	dx3d->BeginScene(0.1f, 0.1f, 0.1f, 1.0f);

	camera->Render();

	camera->GetViewMatrix(&view);
	dx3d->GetProjectionMatrix(&projection);

	Float3 lightPos(100.0f, 100.0f, -100.0f);

	bool result;

	for (Model* const& model : models)
	{
		result |= model->Render(context, view, projection, lightPos);
	}

	dx3d->EndScene();

	if (!result) return false;

	return true;
}
