#pragma once

// Include DirectX Components
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "typedefs.h"

class DX3D
{
public:
	DX3D();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(Matrix* outputMatrix);
	void GetOrthoMatrix(Matrix* outputMatrix);
	void GetVideoCardInfo(char** cardName, int* memory);

	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear);
	void Release();

private:
	bool vsyncEnabled;
	int videoCardMemory;
	char videoCardDescription[128];
	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11RasterizerState* rasterizerState = nullptr;

	Matrix projectionMatrix;
	Matrix orthoMatrix;
};

