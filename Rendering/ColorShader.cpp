#include "ColorShader.h"

ColorShader::ColorShader()
{
}

bool ColorShader::Render(ID3D11DeviceContext* context, int indexCount, Matrix world, Matrix view, Matrix projection)
{
	bool result;

	result = SetShaderParamaters(context, world, view, projection);
	if (!result) return false;

	RenderShader(context, indexCount);

	return true;
}

bool ColorShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"color.vs", L"color.ps");
	if (!result) return false;

	return true;
}

void ColorShader::Release()
{
	ReleaseShader();
}

bool ColorShader::InitializeShader(ID3D11Device* device, HWND hwnd, LPCWSTR vertexShaderPath, LPCWSTR pixelShaderPath)
{
	HRESULT result;

	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;

	result = D3DCompileFromFile(
		vertexShaderPath, nullptr, nullptr,
		"ColorVertex", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, hwnd, vertexShaderPath);
		return false;
	}

	result = D3DCompileFromFile(
		pixelShaderPath, nullptr, nullptr,
		"ColorPixel", "ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, hwnd, pixelShaderPath);
		return false;
	}

	result = device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		nullptr,
		&vertexShader);

	if (FAILED(result)) return false;

	result = device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		nullptr,
		&pixelShader);

	if (FAILED(result)) return false;

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	result = device->CreateInputLayout(
		polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&layout);

	if (FAILED(result)) return false;

	RELEASE_N(vertexShaderBuffer);
	RELEASE_N(pixelShaderBuffer);

	// Define vertex buffer
	ZeroMemory(&matrixBufferDesc, sizeof(matrixBufferDesc));

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
	if (FAILED(result)) return false;

	return true;
}

void ColorShader::ReleaseShader()
{
	RELEASE_N(matrixBuffer);
	RELEASE_N(layout);
	RELEASE_N(pixelShader);
	RELEASE_N(vertexShader);
}

void ColorShader::RenderShader(ID3D11DeviceContext* context, int indexCount)
{
	context->IASetInputLayout(layout);

	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	context->DrawIndexed(indexCount, 0, 0);
}

bool ColorShader::SetShaderParamaters(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix projection)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	unsigned int bufferNumber;

	world = DirectX::XMMatrixTranspose(world);
	view = DirectX::XMMatrixTranspose(view);
	projection = DirectX::XMMatrixTranspose(projection);

	// Allow writing to buffer via CPU
	result = context->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = world;
	dataPtr->view = view;
	dataPtr->projection = projection;

	context->Unmap(matrixBuffer, 0);
	context->VSSetConstantBuffers(0, 1, &matrixBuffer);

	return true;
}

void ColorShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderPath)
{
	if (!errorMessage)
	{
		MessageBox(hwnd, shaderPath, L"Missing Shader File", MB_OK);
		return;
	}

	LPSTR errors;
	SIZE_T bufferSize, i;

	std::ofstream fout;
	errors = (LPSTR)errorMessage->GetBufferPointer();
	bufferSize = errorMessage->GetBufferSize();

	fout.open("shader-error-log.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << errors[i];
	}

	fout.close();

	RELEASE_N(errorMessage);

	MessageBox(hwnd, shaderPath, L"Check shader-error-log.txt", MB_OK);
}
