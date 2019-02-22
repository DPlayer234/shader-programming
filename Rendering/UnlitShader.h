#pragma once

#include "Shader.h"

class UnlitShader :
	public Shader
{
public:
	UnlitShader();

	bool Initialize(ID3D11Device* device, HWND hwnd) override;
};

