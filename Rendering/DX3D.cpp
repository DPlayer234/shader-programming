#include "DX3D.h"
#include "defines.h"

DX3D::DX3D()
{
}

void DX3D::BeginScene(float r, float g, float b, float a)
{
	float color[4] = { r, g, b, a };

	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void DX3D::EndScene()
{
	if (vsyncEnabled)
	{
		swapChain->Present(1, 0);
	}
	else
	{
		swapChain->Present(0, 0);
	}
}

ID3D11Device* DX3D::GetDevice()
{
	return device;
}

ID3D11DeviceContext* DX3D::GetDeviceContext()
{
	return deviceContext;
}

void DX3D::GetProjectionMatrix(Matrix* outputMatrix)
{
	*outputMatrix = projectionMatrix;
}

void DX3D::GetOrthoMatrix(Matrix* outputMatrix)
{
	*outputMatrix = orthoMatrix;
}

void DX3D::GetVideoCardInfo(char** cardName, int* memory)
{
	// *cardName = new char[128];
	// strcpy_s(cardName, 128, videoCardDescription);

	*cardName = videoCardDescription;
	*memory = videoCardMemory;
}

bool DX3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear)
{
	HRESULT result;

	float fieldOfView, screenAspect;
	vsyncEnabled = vsync;

#pragma region Init Variables
	// Factory for creation of several DXGI Objects
	IDXGIFactory* factory;

	// The used graphics adapter
	IDXGIAdapter* adapter;

	// The adapter's output, for example a monitor
	IDXGIOutput* adapterOutput;
	unsigned int numModes, stringLength;

	// Display-Settings (Array)
	DXGI_MODE_DESC* displayModeList;

	// Display Settings to use
	DXGI_MODE_DESC displayMode;

	// Adapter-Settings
	DXGI_ADAPTER_DESC adapterDesc;

	// Buffer Swapping Settings
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	// Description of the feature level
	D3D_FEATURE_LEVEL featureLevel;

	// Back-Buffer as 2D-Texture
	ID3D11Texture2D* backBuffer;

	// Description of the depth buffer texture
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	// Description of the depth stencil buffer
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	// Description of the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	// Description of the rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;

	// Viewport Dimensions and other
	D3D11_VIEWPORT viewport;

	// Used format (32bit)
	const DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Display mode flags
	const UINT flags = DXGI_ENUM_MODES_INTERLACED;
#pragma endregion

#pragma region Device and SwapChain
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result)) return false;

	// Return the first Graphics Adapter
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result)) return false;

	// Returns the first Monitor-Settings
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result)) return false;

	// Gets the amount of display modes
	result = adapterOutput->GetDisplayModeList(format, flags, &numModes, NULL);

	if (FAILED(result)) return false;

	// Initialize the Display Mode List
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// Fills our Display Mode List
	result = adapterOutput->GetDisplayModeList(format, flags, &numModes, displayModeList);

	if (FAILED(result)) return false;

	for (int i = 0; i < numModes; i++)
	{
		auto mode = displayModeList[i];
		
		if (mode.Width == screenWidth && mode.Height == screenHeight)
		{
			// Save selected mode
			displayMode = mode;
		}
	}

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	size_t converted;
	errno_t error = wcstombs_s(&converted, videoCardDescription, adapterDesc.Description, sizeof(videoCardDescription));
	videoCardMemory = (int)adapterDesc.DedicatedVideoMemory / (1024 * 1024);

	if (error) return false;

	// TODO: Potential memory leaks on error
	// Display mode list no longer needed
	delete[] displayModeList;
	displayModeList = nullptr;

	RELEASE_N(adapterOutput);
	RELEASE_N(adapter);
	RELEASE_N(factory);

	// Swap Chain
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	// Assign cached display mode
	swapChainDesc.BufferDesc = displayMode;

	// If enabled, uses the display mode's refresh rate
	if (!vsyncEnabled)
	{
		// Back Buffer updated as fast as possible
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set correct usage
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = hwnd;

	// Disable multi-sampling
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Microsoft Recommendatation:
	// IDXGISwapChain::SetFullscreenState()

	swapChainDesc.Windowed = !fullscreen;

	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.0
	featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Actually create the device and swap chain
	result = D3D11CreateDeviceAndSwapChain(
		nullptr, D3D_DRIVER_TYPE_HARDWARE,
		NULL, 0, &featureLevel, 1, D3D11_SDK_VERSION,
		&swapChainDesc, &swapChain,
		&device, nullptr, &deviceContext);

	if (FAILED(result)) return false;
	
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(result)) return false;

	result = device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	if (FAILED(result)) return false;

	backBuffer->Release();
	backBuffer = nullptr;
#pragma endregion

#pragma region Depth Buffer
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;

	// 24 bits for depth, 8 bits for Stencil
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;

	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
	if (FAILED(result)) return false;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthStencilDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

	// Setting depth stencil operations for the front face
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Setting depth stencil operations for the back face
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result)) return false;

	deviceContext->OMSetDepthStencilState(depthStencilState, 1); // D3D11_DEFAULT_STENCIL_REFERENCE);

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	
	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result)) return false;

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);
#pragma endregion

#pragma region Rasterizer
	ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));

	rasterizerDesc.AntialiasedLineEnable = false;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0.0f;
	rasterizerDesc.DepthClipEnable = true;
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.FrontCounterClockwise = false;
	rasterizerDesc.MultisampleEnable = false;
	rasterizerDesc.ScissorEnable = false;
	rasterizerDesc.SlopeScaledDepthBias = 0.0f;

	result = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if (FAILED(result)) return false;

	deviceContext->RSSetState(rasterizerState);
#pragma endregion

#pragma region Viewport and Matrices
	viewport.Width = screenWidth;
	viewport.Height = screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	deviceContext->RSSetViewports(1, &viewport);

	fieldOfView = (float)DirectX::XM_PI / 2.0f;
	screenAspect = (float)screenWidth / (float)screenHeight;

	projectionMatrix = DirectX::XMMatrixPerspectiveLH(fieldOfView, screenAspect, screenNear, screenDepth);
	orthoMatrix = DirectX::XMMatrixOrthographicLH(screenWidth, screenHeight, screenNear, screenDepth);
#pragma endregion

	return true;
}

void DX3D::Release()
{
	if (swapChain)
	{
		swapChain->SetFullscreenState(false, nullptr);
	}

	RELEASE_N(rasterizerState);
	RELEASE_N(depthStencilState);
	RELEASE_N(depthStencilBuffer);
	RELEASE_N(depthStencilView);
	RELEASE_N(renderTargetView);
	RELEASE_N(deviceContext);
	RELEASE_N(device);
	RELEASE_N(swapChain);
}