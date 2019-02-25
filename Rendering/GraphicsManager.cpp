#include "GraphicsManager.h"

#include <math.h>
#include "defines.h"

#include "CubeModel.h"
#include "PyramidModel.h"
#include "PlaneModel.h"

#include "UnlitShader.h"
#include "BlinnPhongShader.h"

#include "KeyCode.h"

bool GraphicsManager::Initialize(int screenWidth, int screenHeight, HWND hwnd, InputManager* input)
{
	bool result;

	this->hwnd = hwnd;
	this->input = input;

#pragma region DirectX Initialization
	dx3d = new DX3D;
	if (!dx3d) return false;

	result = dx3d->Initialize(
		screenWidth, screenHeight, VSYNC_ENABLED, hwnd,
		FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR, FIELD_OF_VIEW);

	if (!result)
	{
		MessageBeep(MB_ICONERROR);
		MessageBox(hwnd, L"Could not initialize DX3D.", L"Error", MB_OK);
		return false;
	}
#pragma endregion

#pragma region Camera
	camera = new Camera;
	if (!camera) return false;

	camera->SetPosition(Float3(0.0f, 0.8f, -2.0f));
	camera->SetRotation(Float3(20.0f, 0.0f, 0.0f));
#pragma endregion

#pragma region Skybox
	skybox = AddModel<Skybox>();
	auto skyboxShader = LoadShader<UnlitShader>();
	auto skyboxTexture = LoadTexture("data/skybox.tga");

	if (!skybox || !skyboxShader || !skyboxTexture) return false;

	skybox->InitializeSkybox();
	skyboxShader->SetTexture(skyboxTexture->GetResourceView());
	skybox->SetShader(skyboxShader);
#pragma endregion

#pragma region Floor Plane
	auto floor = AddModel<PlaneModel>();
	auto floorShader = LoadShader<BlinnPhongShader>();
	auto floorTexture = LoadTexture("data/floor.tga");

	if (!floor || !floorShader || !floorTexture) return false;

	floor->SetPosition(Float3(0.0f, -0.5f, 0.0f));
	floor->SetScale(Float3(5.0f, 5.0f, 5.0f));
	floorShader->SetTexture(floorTexture->GetResourceView());
	floorShader->SpecularPower = 4.0f;
	floorShader->SpecularAlbedo = Float4(2.0f, 2.0f, 2.0f, 1.0f);
	floor->SetShader(floorShader);
#pragma endregion

#pragma region Metal Box
	auto metalBox = AddModel<CubeModel>();
	auto metalBoxShader = LoadShader<BlinnPhongShader>();
	auto metalBoxTexture = LoadTexture("data/metal.tga");

	if (!metalBox || !metalBoxShader || !metalBoxTexture) return false;

	metalBox->SetPosition(Float3(0.0f, 0.0f, 0.0f));
	metalBoxShader->SetTexture(metalBoxTexture->GetResourceView());
	metalBoxShader->SpecularPower = 6.0f;
	metalBoxShader->SpecularAlbedo = Float4(1.5f, 1.5f, 1.5f, 1.0f);
	metalBox->SetShader(metalBoxShader);
#pragma endregion

#pragma region Pyramid
	auto pyramid = AddModel<PyramidModel>();
	auto pyramidShader = LoadShader<BlinnPhongShader>();
	auto pyramidTexture = LoadTexture("data/stone01.tga");

	if (!pyramid || !pyramidShader || !pyramidTexture) return false;

	pyramid->SetPosition(Float3(3.0f, 0.0f, 0.0f));
	pyramid->SetRotation(Float3(0.0f, -15.0f, 0.0f));
	pyramidShader->SetTexture(pyramidTexture->GetResourceView());
	pyramidShader->SpecularPower = 0.5f;
	pyramidShader->SpecularAlbedo = Float4(0.5f, 0.5f, 0.5f, 1.0f);
	pyramid->SetShader(pyramidShader);
#pragma endregion

#pragma region Glass Box
	auto glassBox = AddModel<CubeModel>();
	auto glassBoxShader = LoadShader<BlinnPhongShader>();
	auto glassBoxTexture = LoadTexture("data/glass.tga");

	if (!glassBox || !glassBoxShader || !glassBoxTexture) return false;

	glassBox->SetPosition(Float3(-3.0f, 0.0f, 0.0f));
	glassBox->SetRotation(Float3(0.0f, 15.0f, 0.0f));
	glassBoxShader->SetTexture(glassBoxTexture->GetResourceView());
	glassBoxShader->SpecularAlbedo = Float4(1.0f, 1.0f, 1.0f, 1.5f);
	glassBox->SetShader(glassBoxShader);
#pragma endregion

	return true;
}

void GraphicsManager::Release()
{
	DELETE_P(camera);
	RELEASE(dx3d);

	for (Model* model : models) RELEASE(model);
	for (Shader* shader : shaders) RELEASE(shader);
	for (Texture* texture : textures) RELEASE(texture);

	models.clear();
	shaders.clear();
	textures.clear();
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
#pragma region Camera Movement
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
#pragma endregion
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

bool GraphicsManager::IsKeyDown(unsigned int key)
{
	return input
		? input->IsKeyDown(key)
		: false;
}

bool GraphicsManager::Render()
{
	Matrix view, projection;
	auto context = dx3d->GetDeviceContext();

	// First layer, Background/Skybox/etc.
	dx3d->BeginScene();

	camera->Render();

	// Make sure skybox is centered around camera
	skybox->SetPosition(camera->GetPosition());

	camera->GetViewMatrix(&view);
	dx3d->GetProjectionMatrix(&projection);

	Float3 lightPos(100.0f, 100.0f, -100.0f);

	bool result;

	// Render every model and OR the result
	for (Model* const& model : models)
	{
		result |= model->Render(context, view, projection, lightPos);
	}

	dx3d->EndScene();

	if (!result) return false;

	return true;
}
