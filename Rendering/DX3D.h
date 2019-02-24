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
	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear, float fieldOfView);
	void Release();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	void GetProjectionMatrix(Matrix* outputMatrix);
	void GetOrthoMatrix(Matrix* outputMatrix);
	void GetVideoCardInfo(char** cardName, int* memory);

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

	// Used format (32bit)
	const DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Display mode flags
	const UINT flags = DXGI_ENUM_MODES_INTERLACED;

	// Gets the best supported display mode
	bool GetDisplayMode(DXGI_MODE_DESC* displayMode, int screenWidth, int screenHeight, IDXGIOutput* adapterOutput);

	// Loads the description of the video card in videoCardMemory and videoCardDescription
	bool LoadVideoCardDesc(IDXGIAdapter* adapter);

	// Buffer Swapping Settings
	bool GetSwapChainDesc(DXGI_SWAP_CHAIN_DESC* swapChainDesc, HWND hwnd, DXGI_MODE_DESC displayMode, bool fullscreen);

	// Gets the Feature Level (Should be DirectX 11.0)
	bool GetFeatureLevel(D3D_FEATURE_LEVEL* featureLevel);

	// Description of the depth buffer texture
	bool GetDepthBufferDesc(D3D11_TEXTURE2D_DESC* depthBufferDesc, int screenWidth, int screenHeight);

	// Description of the depth stencil buffer
	bool GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* depthStencilDesc);

	// Description of the depth stencil view
	bool GetDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* depthStencilViewDesc);

	// Description of the rasterizer
	bool GetRasterizerDesc(D3D11_RASTERIZER_DESC* rasterizerDesc);

	// Viewport Dimensions and other
	bool GetViewport(D3D11_VIEWPORT* viewport, int screenWidth, int screenHeight);
};

