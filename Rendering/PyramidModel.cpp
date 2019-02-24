#include "PyramidModel.h"

#include <math.h>
#include "Float3Math.h"

void PyramidModel::ReleaseArrays()
{
	// Do not delete the arrays; points to static memory.
}

bool PyramidModel::CreateVertexArray()
{
	const static float psiz = 0.5f;
	const static float nsiz = -psiz;

	vertexCount = 16;
	vertexArray = new Vertex[vertexCount];

	Float3 top(0.0f, psiz, 0.0f);
	Float3 point1(psiz, nsiz, psiz);
	Float3 point2(nsiz, nsiz, psiz);
	Float3 point3(nsiz, nsiz, nsiz);
	Float3 point4(psiz, nsiz, nsiz);

	// Defining lengths for the normalized normal vectors
	const float normalOut = 1.0f;
	const float normalUp = 0.5f;
	const float normalLength = sqrt(normalOut * normalOut + normalUp * normalUp);
	const float normalOutN = normalOut / normalLength;
	const float normalUpN = normalUp / normalLength;

	SetTriangleVertices(vertexArray + 0, Float3(0.0f, normalUpN, normalOutN), top, point1, point2); // Z+ Face
	SetTriangleVertices(vertexArray + 3, Float3(-normalOutN, normalUpN, 0.0f), top, point2, point3); // X- Face
	SetTriangleVertices(vertexArray + 6, Float3(0.0f, normalUpN, -normalOutN), top, point3, point4); // Z- Face
	SetTriangleVertices(vertexArray + 9, Float3(normalOutN, normalUpN, 0.0f), top, point4, point1); // X+ Face

	SetGroundVertices(vertexArray + 12, Float3(0.0f, nsiz, 0.0f), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, 0.0f, psiz));

	return true;
}

bool PyramidModel::CreateIndexArray()
{
	indexArray = indices;
	indexCount = sizeof(indices) / sizeof(indices[0]);

	return true;
}

void PyramidModel::SetTriangleVertices(Vertex vertices[3], Float3 normal, Float3 point1, Float3 point2, Float3 point3)
{
	vertices[0].Position = point1;
	vertices[1].Position = point2;
	vertices[2].Position = point3;

	vertices[0].Normal = normal;
	vertices[1].Normal = normal;
	vertices[2].Normal = normal;

	vertices[0].Tex = Float2(0.5f, 0.0f);
	vertices[1].Tex = Float2(0.0f, 1.0f);
	vertices[2].Tex = Float2(1.0f, 1.0f);

	Float4 color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].Color = color;
	vertices[1].Color = color;
	vertices[2].Color = color;
}

void PyramidModel::SetGroundVertices(Vertex vertices[4], Float3 direction, Float3 offset1, Float3 offset2)
{
	vertices[0].Position = add(direction, offset1, offset2);
	vertices[1].Position = add(direction, unm(offset1), offset2);
	vertices[2].Position = add(direction, offset1, unm(offset2));
	vertices[3].Position = add(direction, unm(offset1), unm(offset2));

	vertices[0].Normal = add(direction, mul(vertices[0].Position, 0.1));
	vertices[1].Normal = add(direction, mul(vertices[1].Position, 0.1));
	vertices[2].Normal = add(direction, mul(vertices[2].Position, 0.1));
	vertices[3].Normal = add(direction, mul(vertices[3].Position, 0.1));

	vertices[0].Tex = Float2(1.0f, 0.0f);
	vertices[1].Tex = Float2(0.0f, 0.0f);
	vertices[2].Tex = Float2(1.0f, 1.0f);
	vertices[3].Tex = Float2(0.0f, 1.0f);

	//Float4 color(abs(direction.x), abs(direction.y), abs(direction.z), 1.0f);
	Float4 color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].Color = color;
	vertices[1].Color = color;
	vertices[2].Color = color;
	vertices[3].Color = color;
}

UINT PyramidModel::indices[18] = {
	0, 2, 1,
	3, 5, 4,
	6, 8, 7,
	9, 11, 10,
	12, 13, 14,
	13, 15, 14
};
