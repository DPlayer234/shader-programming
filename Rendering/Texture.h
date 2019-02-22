#pragma once

#include "defines.h"
#include <d3d11.h>
#include <stdio.h>

class Texture
{
public:
	Texture();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* fileName);
	void Release();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(const char* fileName, int* width, int* height);
	unsigned char* targaData = nullptr;
	ID3D11Texture2D* texture = nullptr;
	ID3D11ShaderResourceView* textureView = nullptr;

	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};
};
