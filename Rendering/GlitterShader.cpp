#include "GlitterShader.h"

bool GlitterShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"Glitter.vs.hlsl", L"Glitter.ps.hlsl");
	if (!result) return false;

	return true;
}

void GlitterShader::SetSubShaderParameters(UniformBufferType*& uniforms)
{
	uniforms->DiffuseAlbedo = DiffuseAlbedo;
	uniforms->SpecularAlbedo = SpecularAlbedo;
	uniforms->SpecularPower = SpecularPower;
}
