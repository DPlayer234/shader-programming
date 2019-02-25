#include "MateShader.h"

bool MateShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"Mate.vs", L"Mate.ps");
	if (!result) return false;

	return true;
}

void MateShader::SetSubShaderParameters(UniformBufferType*& uniforms)
{
	uniforms->DiffuseAlbedo = DiffuseAlbedo;
}
