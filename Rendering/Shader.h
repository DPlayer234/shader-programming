#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include "defines.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>

using Float3 = DirectX::XMFLOAT3;
using Vector = DirectX::XMVECTOR;
using Matrix = DirectX::XMMATRIX;

class Shader
{
public:
	Shader();

	bool Render(ID3D11DeviceContext* context, int indexCount, Matrix world, Matrix view, Matrix projection);

	virtual bool Initialize(ID3D11Device* device, HWND hwnd) = 0;
	virtual void Release();

protected:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCWSTR vertexShaderPath, LPCWSTR pixelShaderPath);
	void ReleaseShader();

	void RenderShader(ID3D11DeviceContext* context, int indexCount);
	bool SetShaderParamaters(ID3D11DeviceContext* context, Matrix world, Matrix view, Matrix projection);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR path);

	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

private:
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* layout = nullptr;
	ID3D11Buffer* matrixBuffer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
};

