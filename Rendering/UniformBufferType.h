#pragma once

#include "typedefs.h"

struct UniformBufferType
{
	Matrix World;
	Matrix View;
	Matrix Projection;
	Float3 LightPos;

	Float4 DiffuseAlbedo;
	Float4 SpecularAlbedo;
	float SpecularPower;
};
