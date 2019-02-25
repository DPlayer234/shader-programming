#include "BlinnPhongShader.h"

bool BlinnPhongShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(device, hwnd, L"BlinnPhong.vs", L"BlinnPhong.ps");
	if (!result) return false;

	return true;
}

void BlinnPhongShader::SetSubShaderParameters(UniformBufferType*& uniforms)
{
	uniforms->DiffuseAlbedo = DiffuseAlbedo;
	uniforms->SpecularAlbedo = SpecularAlbedo;
	uniforms->SpecularPower = SpecularPower;
}
