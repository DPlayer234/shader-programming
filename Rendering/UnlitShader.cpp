#include "UnlitShader.h"

bool UnlitShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result; 
	
	result = InitializeShader(device, hwnd, L"Unlit.vs.hlsl", L"Unlit.ps.hlsl");
	if (!result) return false; 
	
	return true;
}
