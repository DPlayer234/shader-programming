#pragma once

#include "typedefs.h"

// Represents the uniform buffer type used by Shaders
struct UniformBufferType
{
	// The world/model transformation
	Matrix World;

	// The view matrix
	Matrix View;

	// The projection matrix
	Matrix Projection;

	// The position of the light point
	Float4 LightPos;

	// The diffuse albedo color
	Float4 DiffuseAlbedo;

	// The specular albedo color
	Float4 SpecularAlbedo;

	// The power of the specular point
	float SpecularPower;
};
