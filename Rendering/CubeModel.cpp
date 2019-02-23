#include "CubeModel.h"

#include <math.h>

Float3 add(Float3 a, Float3 b)
{
	return Float3(
		a.x + b.x,
		a.y + b.y,
		a.z + b.z);
}

Float3 add(Float3 a, Float3 b, Float3 c)
{
	return Float3(
		a.x + b.x + c.x,
		a.y + b.y + c.y,
		a.z + b.z + c.z);
}

Float3 unm(Float3 v)
{
	return Float3(-v.x, -v.y, -v.z);
}

Float3 mul(Float3 v, float s)
{
	return Float3(v.x * s, v.y * s, v.z * s);
}

CubeModel::CubeModel()
{
}

void CubeModel::ReleaseArrays()
{
	DELETE_A(vertexArray);
	// Do not delete index array; points to static memory.
}

bool CubeModel::CreateVertexArray()
{
	const static float psiz = 0.5f;
	const static float nsiz = -psiz;

	vertexCount = 24;
	vertexArray = new Vertex[vertexCount];

	SetVertices(vertexArray + 0,  Float3(0.0f, psiz, 0.0f), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, 0.0f, psiz));
	SetVertices(vertexArray + 4,  Float3(0.0f, nsiz, 0.0f), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, 0.0f, psiz));
	SetVertices(vertexArray + 8,  Float3(psiz, 0.0f, 0.0f), Float3(0.0f, psiz, 0.0f), Float3(0.0f, 0.0f, psiz));
	SetVertices(vertexArray + 12, Float3(nsiz, 0.0f, 0.0f), Float3(0.0f, psiz, 0.0f), Float3(0.0f, 0.0f, psiz));
	SetVertices(vertexArray + 16, Float3(0.0f, 0.0f, psiz), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, psiz, 0.0f));
	SetVertices(vertexArray + 20, Float3(0.0f, 0.0f, nsiz), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, psiz, 0.0f));

	return true;
}

bool CubeModel::CreateIndexArray()
{
	indexArray = indices;
	indexCount = sizeof(indices) / sizeof(indices[0]);

	return true;
}

void CubeModel::SetVertices(Vertex vertices[4], Float3 direction, Float3 offset1, Float3 offset2)
{
	vertices[0].Position = add(direction, offset1, offset2);
	vertices[1].Position = add(direction, unm(offset1), offset2);
	vertices[2].Position = add(direction, offset1, unm(offset2));
	vertices[3].Position = add(direction, unm(offset1), unm(offset2));

	vertices[0].Normal = add(direction, mul(vertices[0].Position, 0.1));
	vertices[1].Normal = add(direction, mul(vertices[1].Position, 0.1));
	vertices[2].Normal = add(direction, mul(vertices[2].Position, 0.1));
	vertices[3].Normal = add(direction, mul(vertices[3].Position, 0.1));

	vertices[0].Tex = Float2(1.0f, 1.0f);
	vertices[1].Tex = Float2(0.0f, 1.0f);
	vertices[2].Tex = Float2(1.0f, 0.0f);
	vertices[3].Tex = Float2(0.0f, 0.0f);

	//Float4 color(abs(direction.x), abs(direction.y), abs(direction.z), 1.0f);
	Float4 color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].Color = color;
	vertices[1].Color = color;
	vertices[2].Color = color;
	vertices[3].Color = color;
}

UINT CubeModel::indices[36] = {
	0,  2,  1,
	1,  2,  3,
	4,  5,  6,
	5,  7,  6,
	8,  9, 10,
	9, 11, 10,
	12, 14, 13,
	13, 14, 15,
	16, 17, 18,
	17, 19, 18,
	20, 22, 21,
	21, 22, 23
};
