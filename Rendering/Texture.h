#pragma once

#include "defines.h"
#include <d3d11.h>
#include <stdio.h>

// Represents a loaded texture.
// Only supports loading images in 32bit Targa format.
class Texture
{
public:
	// Initializes the texture by loading the texture
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* fileName);

	// Releases all memory associated with this object
	void Release();

	// Gets the shader resource view of the texture
	ID3D11ShaderResourceView* GetResourceView();

private:
	// The data of the targa file
	unsigned char* targaData = nullptr;

	// The 2D texture with the loaded data
	ID3D11Texture2D* texture = nullptr;

	// The shader resource view of the texture
	ID3D11ShaderResourceView* textureView = nullptr;

	// Loads the targa file and writes the width and height to the parameter pointers
	bool LoadTarga(const char* fileName, int* width, int* height);

	// A struct representing a targa header
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width; // The width of the texture
		unsigned short height; // The height of the texture
		unsigned char bpp; // The bits-per-pixel of the texture. Needs to be 32
		unsigned char data2;
	};
};

