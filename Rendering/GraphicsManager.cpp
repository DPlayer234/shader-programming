#include "GraphicsManager.h"

#include "ModelDefinitions.h"

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

void GraphicsManager::Update(float totalTime)
{
	Float3 rotation(0.0f, totalTime * 20.0f, 0.0f);
	models[0]->SetRotation(DirectX::XMLoadFloat3(&rotation));
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

	camera->SetPosition(0.0f, 0.8f, -1.0f);
	camera->SetRotation(20.0f, 0.0f, 0.0f);

#pragma region Add Models
	Float3 position;

	Model* model = AddModel<CubeModelBP>();
	if (!model) return false;

	position = Float3(0.0f, 0.0f, 0.0f);
	model->SetPosition(DirectX::XMLoadFloat3(&position));
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

template<class T>
Model* GraphicsManager::AddModel()
{
	bool result;

	Model* model = new T;
	result = model->Initialize(dx3d->GetDevice(), this->hwnd);
	if (!result) return nullptr;

	models.push_back(model);

	return model;
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

	Float3 lightPos(100.0f, 100.0f, 100.0f);

	bool result;

	for (Model* const& model : models)
	{
		result |= model->Render(context, view, projection, lightPos);
	}

	dx3d->EndScene();

	if (!result) return false;

	return true;
}
