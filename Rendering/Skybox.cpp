#include "Skybox.h"

void Skybox::InitializeSkybox()
{
	SetScale(Float3(skyboxBoundSize, skyboxBoundSize, skyboxBoundSize));
}

bool Skybox::CreateIndexArray()
{
	indexArray = indices;
	indexCount = sizeof(indices) / sizeof(indices[0]);

	return true;
}

UINT Skybox::indices[36] = {
	 0,  1,  2,
	 1,  3,  2,
	 4,  6,  5,
	 5,  6,  7,
	 8, 10,  9,
	 9, 10, 11,
	12, 13, 14,
	13, 15, 14,
	16, 18, 17,
	17, 18, 19,
	20, 21, 22,
	21, 23, 22
};
