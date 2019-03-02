#pragma once

// Include DirectX Components
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "typedefs.h"

// Handles initializing and managing DirectX and Direct3D components.
class DX3D
{
public:
	// Initializes the Direct3D Components and sets up the rendering pipeline
	bool Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear, float fieldOfView);

	// Releases memory associated with this object
	void Release();

	// Sets up the rendering process start by clearing the screen with the specified color
	void BeginScene(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f);

	// Ends the rendering process by presenting the rendered content to the window
	void EndScene();

	// Gets the ID3D11Device used
	ID3D11Device* GetDevice();

	// Gets the associated ID3D11DeviceContext used
	ID3D11DeviceContext* GetDeviceContext();

	// Writes the projection matrix to the outputMatrix
	void GetProjectionMatrix(Matrix* outputMatrix);

	// Writes the ortho matrix to the outputMatrix
	void GetOrthoMatrix(Matrix* outputMatrix);

	// Writes the char* with the video card name and the available video card memory to the specified variables
	void GetVideoCardInfo(const char** cardName, int* memory);

private:
	// Whether or not VSync was enabled
	bool vsyncEnabled;

	// The available video card memory
	int videoCardMemory;

	// The description/name of the video card
	char videoCardDescription[128];

	/* Direct3D Rendering components */

	IDXGISwapChain* swapChain = nullptr;
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11RenderTargetView* renderTargetView = nullptr;
	ID3D11BlendState* blendState = nullptr;
	ID3D11DepthStencilView* depthStencilView = nullptr;
	ID3D11Texture2D* depthStencilBuffer = nullptr;
	ID3D11DepthStencilState* depthStencilState = nullptr;
	ID3D11RasterizerState* rasterizerState = nullptr;

	// Stores the projection matrix
	Matrix projectionMatrix;

	// Stores the ortho matrix
	Matrix orthoMatrix;

	// Used format (32bit)
	const DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Display mode flags
	const UINT flags = DXGI_ENUM_MODES_INTERLACED;

	// Loads the description of the video card in videoCardMemory and videoCardDescription
	bool LoadVideoCardDesc(IDXGIAdapter* adapter);

	/* 
	 * Functions generating various DirectX description objects
	 * They write the structs to the supplied pointers and return whether or not the operation was successful.
	 */

	// Gets the best supported display mode
	bool GetDisplayMode(DXGI_MODE_DESC* displayMode, int screenWidth, int screenHeight, IDXGIOutput* adapterOutput);

	// Gets the Buffer Swapping Settings
	bool GetSwapChainDesc(DXGI_SWAP_CHAIN_DESC* swapChainDesc, HWND hwnd, DXGI_MODE_DESC displayMode, bool fullscreen);

	// Gets the Feature Level (Should be DirectX 11.0)
	bool GetFeatureLevel(D3D_FEATURE_LEVEL* featureLevel);

	// Gets the Description of the depth buffer texture
	bool GetDepthBufferDesc(D3D11_TEXTURE2D_DESC* depthBufferDesc, int screenWidth, int screenHeight);

	// Gets the Description of the depth stencil buffer
	bool GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* depthStencilDesc);

	// Gets the Description of the depth stencil view
	bool GetDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* depthStencilViewDesc);

	// Gets the Description of the rasterizer
	bool GetRasterizerDesc(D3D11_RASTERIZER_DESC* rasterizerDesc);

	// Gets the description of the blend state
	bool GetBlendDesc(D3D11_BLEND_DESC* blendDesc);

	// Viewport Dimensions and other
	bool GetViewport(D3D11_VIEWPORT* viewport, int screenWidth, int screenHeight);
};

