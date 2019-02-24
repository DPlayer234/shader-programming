#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include "defines.h"
#include "typedefs.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "UniformBufferType.h"

class Shader
{
public:
	virtual bool Initialize(ID3D11Device* device, HWND hwnd) = 0;
	virtual void Release();

	bool Render(ID3D11DeviceContext* context, int indexCount, const UniformBufferType& uniforms);

	void SetTexture(ID3D11ShaderResourceView* textureView);
	ID3D11ShaderResourceView* GetTexture();

protected:
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCWSTR vertexShaderPath, LPCWSTR pixelShaderPath);
	void ReleaseShader();

	void RenderShader(ID3D11DeviceContext* context, int indexCount);
	bool SetShaderParamaters(ID3D11DeviceContext* context, const UniformBufferType& uniforms);
	virtual void SetSubShaderParameters(UniformBufferType*& uniforms);
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR path);

private:
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* layout = nullptr;
	ID3D11Buffer* uniformBuffer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;

	static bool initializedStatic;
	static D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	static unsigned int layoutElementCount;
	static D3D11_BUFFER_DESC uniformBufferDesc;
	static D3D11_SAMPLER_DESC samplerDesc;

	static void InitializeStatic();
};

#define DEFINE_SHADER(CLASS_NAME, vertexShaderPath, pixelShaderPath) \
class CLASS_NAME : public Shader \
{ \
public: \
	bool Initialize(ID3D11Device* device, HWND hwnd) override \
	{ \
		bool result; \
		\
		result = InitializeShader(device, hwnd, (vertexShaderPath), (pixelShaderPath)); \
		if (!result) return false; \
		\
		return true; \
	} \
}
