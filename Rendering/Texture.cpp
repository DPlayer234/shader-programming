#include "Texture.h"

bool Texture::Initialize(ID3D11Device* device, ID3D11DeviceContext* context, const char* fileName)
{
	bool result;
	int width, height;
	D3D11_TEXTURE2D_DESC textureDesc;
	HRESULT hResult;
	unsigned int rowPitch;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	result = LoadTarga(fileName, &width, &height);
	if (!result) return false;

#pragma region Create DirectX Texture
	ZeroMemory(&textureDesc, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hResult = device->CreateTexture2D(&textureDesc, nullptr, &texture);
	if (FAILED(hResult)) return false;
#pragma endregion

	rowPitch = (width * 4) * sizeof(unsigned char);
	context->UpdateSubresource(texture, 0, nullptr, targaData, rowPitch, 0);

#pragma region Create DirectX Subresource
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hResult = device->CreateShaderResourceView(texture, &srvDesc, &textureView);
	if (FAILED(hResult)) return false;
#pragma endregion

	context->GenerateMips(textureView);

	DELETE_A(targaData);

	return true;
}

void Texture::Release()
{
	RELEASE_N(textureView);
	RELEASE_N(texture);
	DELETE_A(targaData);
}

ID3D11ShaderResourceView* Texture::GetResourceView()
{
	return textureView;
}

bool Texture::LoadTarga(const char* fileName, int* width, int* height)
{
	errno_t error;
	int bpp, imageSize;
	FILE* filePtr;
	size_t count;
	TargaHeader targaFileHeader;
	unsigned char* targaImage;
	
	error = fopen_s(&filePtr, fileName, "rb");
	if (error) return false;

	count = fread(&targaFileHeader, sizeof(TargaHeader), 1, filePtr);
	if (count != 1) return false;

	*width = targaFileHeader.width;
	*height = targaFileHeader.height;
	bpp = targaFileHeader.bpp;
	if (bpp != 32) return false;

	int pelSize = (bpp / 8);

	imageSize = (*width) * (*height) * pelSize;
	targaImage = new unsigned char[imageSize];
	if (!targaImage) return false;

	count = fread(targaImage, 1, imageSize, filePtr);
	if (count != imageSize) return false;

	error = fclose(filePtr);
	if (error) return false;

	targaData = new unsigned char[imageSize];
	if (!targaData) return false;

	int index, i, j, k;
	index = 0;
	k = imageSize - (*width * pelSize);

	for (j = 0; j < *height; j++)
	{
		for (i = 0; i < *width; i++)
		{
			targaData[index + 0] = targaImage[k + 2]; // Red
			targaData[index + 1] = targaImage[k + 1]; // Green
			targaData[index + 2] = targaImage[k + 0]; // Blue
			targaData[index + 3] = targaImage[k + 3]; // Alpha

			k += pelSize;
			index += 4;
		}

		k -= (*width * pelSize * 2);
	}

	DELETE_A(targaImage);

	return true;
}
