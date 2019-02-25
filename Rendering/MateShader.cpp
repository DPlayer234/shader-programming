#include "MateShader.h"

bool MateShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"Mate.vs.hlsl", L"Mate.ps.hlsl");
	if (!result) return false;

	return true;
}

void MateShader::SetSubShaderParameters(UniformBufferType*& uniforms)
{
	uniforms->DiffuseAlbedo = DiffuseAlbedo;
}
