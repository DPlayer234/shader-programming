#pragma once

#include "Shader.h"

class GlitterShader : public Shader
{
public:
	Float4 DiffuseAlbedo = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	Float4 SpecularAlbedo = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	float SpecularPower = 1.0f;

	bool Initialize(ID3D11Device* device, HWND hwnd) override;

protected:
	void SetSubShaderParameters(UniformBufferType*& uniforms) override;
};

