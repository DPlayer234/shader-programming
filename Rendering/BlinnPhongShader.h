#pragma once

#include "Shader.h"

class BlinnPhongShader : public Shader
{
public:
	bool Initialize(ID3D11Device* device, HWND hwnd) override;

	Float4 DiffuseAlbedo = Float4(1.0f, 0.0f, 1.0f, 1.0f);
	Float4 SpecularAlbedo = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	float SpecularPower = 1.0f;

protected:
	void SetSubShaderParameters(UniformBufferType*& uniforms) override;
};

