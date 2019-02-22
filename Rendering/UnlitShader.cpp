#include "UnlitShader.h"

bool UnlitShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result; 
	
	result = InitializeShader(device, hwnd, L"unlit.vs", L"unlit.ps");
	if (!result) return false; 
	
	return true;
}
