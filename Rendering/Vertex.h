#pragma once

#include "typedefs.h"

// Data for a single vertex to be passed to a shader or be used in a model
struct Vertex
{
	// The relative position of the vertex
	Float3 Position;

	// The color associated with the vertex
	Float4 Color;

	// The UV Coordinate to be used by the vertex
	Float2 Tex;

	// The normal vector of the vertex
	Float3 Normal;
};
