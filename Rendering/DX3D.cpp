#include "DX3D.h"
#include "defines.h"

bool DX3D::Initialize(int screenWidth, int screenHeight, bool vsync, HWND hwnd, bool fullscreen, float screenDepth, float screenNear, float fieldOfView)
{
	HRESULT result;
	bool okay;

	float screenAspect;
	vsyncEnabled = vsync;

#pragma region Init Variables
	// Factory for creation of several DXGI Objects
	IDXGIFactory* factory;

	// The used graphics adapter
	IDXGIAdapter* adapter;

	// The adapter's output, for example a monitor
	IDXGIOutput* adapterOutput;

	// Display Settings to use
	DXGI_MODE_DESC displayMode;

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

	// Description of the blend state
	D3D11_BLEND_DESC blendDesc;

	// Description of the rasterizer
	D3D11_RASTERIZER_DESC rasterizerDesc;

	// Viewport Dimensions and other
	D3D11_VIEWPORT viewport;
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

	okay = GetDisplayMode(&displayMode, screenWidth, screenHeight, adapterOutput);
	if (!okay) return false;

	LoadVideoCardDesc(adapter);

	RELEASE_N(adapterOutput);
	RELEASE_N(adapter);
	RELEASE_N(factory);

	// Swap Chain
	okay = GetSwapChainDesc(&swapChainDesc, hwnd, displayMode, fullscreen);
	if (!okay) return false;

	// Set the feature level to DirectX 11.0
	okay = GetFeatureLevel(&featureLevel);
	if (!okay) return false;

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

	RELEASE_N(backBuffer);
#pragma endregion

#pragma region Depth Buffer
	okay = GetDepthBufferDesc(&depthBufferDesc, screenWidth, screenHeight);
	if (!okay) return false;

	result = device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencilBuffer);
	if (FAILED(result)) return false;

	okay = GetDepthStencilDesc(&depthStencilDesc);
	if (!okay) return false;

	device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);
	if (FAILED(result)) return false;

	deviceContext->OMSetDepthStencilState(depthStencilState, 1); // D3D11_DEFAULT_STENCIL_REFERENCE);

	okay = GetDepthStencilViewDesc(&depthStencilViewDesc);
	if (!okay) return false;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if (FAILED(result)) return false;
#pragma endregion

#pragma region Setting Output Merger
	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	okay = GetBlendDesc(&blendDesc);
	if (!okay) return false;

	result = device->CreateBlendState(&blendDesc, &blendState);
	if (FAILED(result)) return false;

	deviceContext->OMSetBlendState(blendState, nullptr, 0xffffffff);
#pragma endregion

#pragma region Rasterizer
	okay = GetRasterizerDesc(&rasterizerDesc);
	if (!okay) return false;

	result = device->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
	if (FAILED(result)) return false;

	deviceContext->RSSetState(rasterizerState);
#pragma endregion

#pragma region Viewport
	okay = GetViewport(&viewport, screenWidth, screenHeight);
	if (!okay) return false;

	deviceContext->RSSetViewports(1, &viewport);

	screenAspect = (float)screenWidth / (float)screenHeight;

	projectionMatrix = DirectX::XMMatrixPerspectiveLH(fieldOfView * screenAspect, screenAspect, screenNear, screenDepth);
	orthoMatrix = DirectX::XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, screenNear, screenDepth);
#pragma endregion

	return true;
}

void DX3D::Release()
{
	if (swapChain)
	{
		// Disable fullscreen mode
		swapChain->SetFullscreenState(false, nullptr);
	}

	// Order potentially important
	RELEASE_N(rasterizerState);
	RELEASE_N(blendState);
	RELEASE_N(depthStencilState);
	RELEASE_N(depthStencilBuffer);
	RELEASE_N(depthStencilView);
	RELEASE_N(renderTargetView);
	RELEASE_N(deviceContext);
	RELEASE_N(device);
	RELEASE_N(swapChain);
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
	*cardName = videoCardDescription;
	*memory = videoCardMemory;
}

bool DX3D::GetDisplayMode(DXGI_MODE_DESC* displayMode, int screenWidth, int screenHeight, IDXGIOutput* adapterOutput)
{
	bool result;

	DXGI_MODE_DESC* displayModeList;
	unsigned int numModes;

	// Gets the amount of display modes
	result = adapterOutput->GetDisplayModeList(format, flags, &numModes, NULL);

	if (FAILED(result)) return false;

	// Initialize the Display Mode List
	displayModeList = new DXGI_MODE_DESC[numModes];
	if (!displayModeList) return false;

	// Fills our Display Mode List
	result = adapterOutput->GetDisplayModeList(format, flags, &numModes, displayModeList);

	if (FAILED(result)) return false;

	// Find best supported display mode
	int minAreaDifference = INT_MAX;
	int targetArea = screenWidth * screenHeight;

	int area, areaDifference;
	DXGI_MODE_DESC mode;

	for (unsigned int i = 0; i < numModes; i++)
	{
		mode = displayModeList[i];

		area = mode.Width * mode.Height;
		areaDifference = abs(targetArea - area);

		if (mode.Width == screenWidth && mode.Height == screenHeight || areaDifference < minAreaDifference)
		{
			// Save selected mode
			*displayMode = mode;
			minAreaDifference = areaDifference;
		}
	}

	// Display mode list no longer needed
	delete[] displayModeList;
	displayModeList = nullptr;

	return true;
}

bool DX3D::LoadVideoCardDesc(IDXGIAdapter* adapter)
{
	HRESULT result;

	DXGI_ADAPTER_DESC adapterDesc;

	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result)) return false;

	//Copy description from adapter into member variable with correct format
	size_t converted;
	errno_t error = wcstombs_s(&converted, videoCardDescription, adapterDesc.Description, sizeof(videoCardDescription));
	videoCardMemory = (int)adapterDesc.DedicatedVideoMemory / (1024 * 1024);

	if (error) return false;

	return true;
}

bool DX3D::GetSwapChainDesc(DXGI_SWAP_CHAIN_DESC* _swapChainDesc, HWND hwnd, DXGI_MODE_DESC displayMode, bool fullscreen)
{
	DXGI_SWAP_CHAIN_DESC& swapChainDesc = *_swapChainDesc;
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

	return true;
}

bool DX3D::GetFeatureLevel(D3D_FEATURE_LEVEL* featureLevel)
{
	*featureLevel = D3D_FEATURE_LEVEL_11_0;
	return true;
}

bool DX3D::GetDepthBufferDesc(D3D11_TEXTURE2D_DESC* _depthBufferDesc, int screenWidth, int screenHeight)
{
	D3D11_TEXTURE2D_DESC& depthBufferDesc = *_depthBufferDesc;
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

	return true;
}

bool DX3D::GetDepthStencilDesc(D3D11_DEPTH_STENCIL_DESC* _depthStencilDesc)
{
	D3D11_DEPTH_STENCIL_DESC& depthStencilDesc = *_depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
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

	return true;
}

bool DX3D::GetDepthStencilViewDesc(D3D11_DEPTH_STENCIL_VIEW_DESC* _depthStencilViewDesc)
{
	D3D11_DEPTH_STENCIL_VIEW_DESC& depthStencilViewDesc = *_depthStencilViewDesc;

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	return true;
}

bool DX3D::GetRasterizerDesc(D3D11_RASTERIZER_DESC* _rasterizerDesc)
{
	D3D11_RASTERIZER_DESC& rasterizerDesc = *_rasterizerDesc;

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

	return true;
}

bool DX3D::GetBlendDesc(D3D11_BLEND_DESC* _blendDesc)
{
	D3D11_BLEND_DESC& blendDesc = *_blendDesc;

	// Allow transparent objects
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;

	auto& renderTargetBlendDesc = blendDesc.RenderTarget[0];

	renderTargetBlendDesc.BlendEnable = true;

	renderTargetBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	renderTargetBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;

	renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
	renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;

	renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	return true;
}

bool DX3D::GetViewport(D3D11_VIEWPORT* _viewport, int screenWidth, int screenHeight)
{
	D3D11_VIEWPORT& viewport = *_viewport;

	ZeroMemory(&viewport, sizeof(viewport));

	viewport.Width = (FLOAT)screenWidth;
	viewport.Height = (FLOAT)screenHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	return true;
}
