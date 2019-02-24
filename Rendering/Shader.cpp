#include "Shader.h"

void Shader::Release()
{
	ReleaseShader();
}

bool Shader::Render(ID3D11DeviceContext* context, int indexCount, const UniformBufferType& uniforms)
{
	bool result;

	result = SetShaderParamaters(context, uniforms);
	if (!result) return false;

	RenderShader(context, indexCount);

	return true;
}

void Shader::SetTexture(ID3D11ShaderResourceView* textureView)
{
	this->textureView = textureView;
}

ID3D11ShaderResourceView* Shader::GetTexture()
{
	return textureView;
}

bool Shader::InitializeShader(ID3D11Device* device, HWND hwnd, LPCWSTR vertexShaderPath, LPCWSTR pixelShaderPath)
{
	Shader::InitializeStatic();

	HRESULT result;

	ID3D10Blob* errorMessage = nullptr;
	ID3D10Blob* vertexShaderBuffer = nullptr;
	ID3D10Blob* pixelShaderBuffer = nullptr;

#pragma region Load and Compile Shaders

	result = D3DCompileFromFile(
		vertexShaderPath, nullptr, nullptr,
		"main", "vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		OutputShaderErrorMessage(errorMessage, hwnd, vertexShaderPath);
		return false;
	}

	result = D3DCompileFromFile(
		pixelShaderPath, nullptr, nullptr,
		"main", "ps_5_0",
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

#pragma endregion

	result = device->CreateInputLayout(
		polygonLayout,
		layoutElementCount,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&layout);

	if (FAILED(result)) return false;

	RELEASE_N(vertexShaderBuffer);
	RELEASE_N(pixelShaderBuffer);

	result = device->CreateBuffer(&uniformBufferDesc, nullptr, &uniformBuffer);
	if (FAILED(result)) return false;

	result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result)) return false;

	return true;
}

void Shader::ReleaseShader()
{
	RELEASE_N(samplerState);
	RELEASE_N(uniformBuffer);
	RELEASE_N(layout);
	RELEASE_N(pixelShader);
	RELEASE_N(vertexShader);
}

void Shader::RenderShader(ID3D11DeviceContext* context, int indexCount)
{
	context->IASetInputLayout(layout);

	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);

	context->DrawIndexed(indexCount, 0, 0);
}

bool Shader::SetShaderParamaters(ID3D11DeviceContext* context, const UniformBufferType& uniforms)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	UniformBufferType* dataPtr;

	// Allow writing to buffer via CPU
	result = context->Map(uniformBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	dataPtr = (UniformBufferType*)mappedResource.pData;

	dataPtr->World = DirectX::XMMatrixTranspose(uniforms.World);
	dataPtr->View = DirectX::XMMatrixTranspose(uniforms.View);
	dataPtr->Projection = DirectX::XMMatrixTranspose(uniforms.Projection);
	dataPtr->LightPos = uniforms.LightPos;

	SetSubShaderParameters(dataPtr);

	context->Unmap(uniformBuffer, 0);
	context->VSSetConstantBuffers(0, 1, &uniformBuffer);
	context->PSSetConstantBuffers(0, 1, &uniformBuffer);

	if (textureView)
	{
		context->PSSetShaderResources(0, 1, &textureView);
	}

	return true;
}

void Shader::SetSubShaderParameters(UniformBufferType*& uniforms)
{
	// Nothing by default
}

void Shader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR shaderPath)
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

bool Shader::initializedStatic = false;
D3D11_INPUT_ELEMENT_DESC Shader::polygonLayout[4];
unsigned int Shader::layoutElementCount = 0;
D3D11_BUFFER_DESC Shader::uniformBufferDesc;
D3D11_SAMPLER_DESC Shader::samplerDesc;

void Shader::InitializeStatic()
{
	if (initializedStatic) return;

	// Define the layout for per vertex data
	ZeroMemory(&polygonLayout, sizeof(polygonLayout));

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

	polygonLayout[2].SemanticName = "TEXCOORD";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "NORMAL";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;

	layoutElementCount = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	// Define Uniform Buffer
	ZeroMemory(&uniformBufferDesc, sizeof(uniformBufferDesc));

	uniformBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	uniformBufferDesc.ByteWidth = sizeof(UniformBufferType);
	uniformBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	uniformBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	uniformBufferDesc.MiscFlags = 0;
	uniformBufferDesc.StructureByteStride = 0;

	// Sampler State
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.1f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	initializedStatic = false;
}

