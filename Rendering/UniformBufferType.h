#pragma once

#include "typedefs.h"

struct UniformBufferType
{
	Matrix World;
	Matrix View;
	Matrix Projection;
	Float4 LightPos;

	Float4 DiffuseAlbedo;
	Float4 SpecularAlbedo;
	float SpecularPower;
};
