#pragma once

#pragma comment(lib, "d3dcompiler.lib")

#include "defines.h"
#include "typedefs.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <fstream>
#include "UniformBufferType.h"

// Defines a generic shader object usable to render models
class Shader
{
public:
	// Initializes the shader and all its components
	virtual bool Initialize(ID3D11Device* device, HWND hwnd) = 0;

	// Release all memory associated with this object
	virtual void Release();

	// Renders based on the index and vertex buffer set for the InputAssembler (IA)
	bool Render(ID3D11DeviceContext* context, int indexCount, const UniformBufferType& uniforms);

	// Sets the texture sent to the shader. If no texture is set, it will always use the one that was used to render last.
	void SetTexture(ID3D11ShaderResourceView* textureView);

	// Returns the texture used. May be NULL.
	ID3D11ShaderResourceView* GetTexture();

protected:
	// Handles the actual initialization of the shader by loading the shader code from the specified files and compiling it
	bool InitializeShader(ID3D11Device* device, HWND hwnd, LPCWSTR vertexShaderPath, LPCWSTR pixelShaderPath);

	// Releases the memory associated with the shader components
	void ReleaseShader();

	// Renders the shader
	void RenderShader(ID3D11DeviceContext* context, int indexCount);

	// Sets the shader parameters. This is called in Shader::Render() before Shader::RenderShader() is called.
	bool SetShaderParamaters(ID3D11DeviceContext* context, const UniformBufferType& uniforms);

	// Sets additional uniform fields. Called within Shader::SetShaderParameters()
	virtual void SetSubShaderParameters(UniformBufferType*& uniforms);

	// Outputs the shader error message to a file
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCWSTR path);

private:
	/* Direct3D shader components */

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* layout = nullptr;
	ID3D11Buffer* uniformBuffer = nullptr;
	ID3D11SamplerState* samplerState = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;

	// Whether static components were already initialized
	static bool initializedStatic;

	// The input element layout used for all shaders
	static D3D11_INPUT_ELEMENT_DESC polygonLayout[4];

	// The amount of input layout elements
	static unsigned int layoutElementCount;

	// The description for the uniform buffer
	static D3D11_BUFFER_DESC uniformBufferDesc;

	// The description for the sampler
	static D3D11_SAMPLER_DESC samplerDesc;

	// Initializes static shader components. Does not do anything if called repeatedly.
	static void InitializeStatic();
};

// Shortcut to creating shaders which only need different paths to vertex and pixel shader
#define DEFINE_SHADER(CLASS_NAME, vertexShaderPath, pixelShaderPath) \
class CLASS_NAME : public Shader \
{ \
public: \
	bool Initialize(ID3D11Device* device, HWND hwnd) override \
	{ \
		return InitializeShader(device, hwnd, (vertexShaderPath), (pixelShaderPath)); \
	} \
}
