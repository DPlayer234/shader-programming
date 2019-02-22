#pragma once
#include "Shader.h"

class UnlitShader : public Shader
{
public:
	bool Initialize(ID3D11Device* device, HWND hwnd) override;
};
