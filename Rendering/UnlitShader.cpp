#include "UnlitShader.h"

bool UnlitShader::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result; 
	
	result = InitializeShader(device, hwnd, L"Unlit.vs", L"Unlit.ps");
	if (!result) return false; 
	
	return true;
}
