#include "CubeModel.h"

#include <math.h>
#include "Float3Math.h"

void CubeModel::ReleaseArrays()
{
	DELETE_A(vertexArray);
	// Do not delete index array; points to static memory.
}

bool CubeModel::CreateVertexArray()
{
	const static float psiz = 0.5f;
	const static float nsiz = -psiz;

	const static float UV_OFFSET = 2e-3f;

	const static float uv1L = 0.0f + UV_OFFSET;
	const static float uv1U = 0.5f - UV_OFFSET;
	const static float uv2L = 0.5f + UV_OFFSET;
	const static float uv2U = 1.0f - UV_OFFSET;

	vertexCount = 24;
	vertexArray = new Vertex[vertexCount];

	SetVertices(vertexArray + 0,  Float3(0.0f, psiz, 0.0f), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, 0.0f, psiz), Float2(uv1L, uv1L), Float2(uv1U, uv1U)); // TOP
	SetVertices(vertexArray + 4,  Float3(0.0f, nsiz, 0.0f), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, 0.0f, psiz), Float2(uv2U, uv1L), Float2(uv2L, uv1U)); // BOTTOM
	SetVertices(vertexArray + 8,  Float3(psiz, 0.0f, 0.0f), Float3(0.0f, 0.0f, nsiz), Float3(0.0f, psiz, 0.0f), Float2(uv1U, uv2L), Float2(uv1L, uv2U)); // Other faces
	SetVertices(vertexArray + 12, Float3(nsiz, 0.0f, 0.0f), Float3(0.0f, 0.0f, nsiz), Float3(0.0f, psiz, 0.0f), Float2(uv1L, uv2L), Float2(uv1U, uv2U));
	SetVertices(vertexArray + 16, Float3(0.0f, 0.0f, psiz), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, psiz, 0.0f), Float2(uv2U, uv2L), Float2(uv2L, uv2U));
	SetVertices(vertexArray + 20, Float3(0.0f, 0.0f, nsiz), Float3(psiz, 0.0f, 0.0f), Float3(0.0f, psiz, 0.0f), Float2(uv2L, uv2L), Float2(uv2U, uv2U));

	return true;
}

bool CubeModel::CreateIndexArray()
{
	indexArray = indices;
	indexCount = sizeof(indices) / sizeof(indices[0]);

	return true;
}

void CubeModel::SetVertices(Vertex vertices[4], Float3 direction, Float3 offset1, Float3 offset2, Float2 uvTopLeft, Float2 uvBottomRight)
{
	vertices[0].Position = add(direction, offset1, offset2);
	vertices[1].Position = add(direction, unm(offset1), offset2);
	vertices[2].Position = add(direction, offset1, unm(offset2));
	vertices[3].Position = add(direction, unm(offset1), unm(offset2));

	vertices[0].Normal = add(direction, mul(vertices[0].Position, 0.1f));
	vertices[1].Normal = add(direction, mul(vertices[1].Position, 0.1f));
	vertices[2].Normal = add(direction, mul(vertices[2].Position, 0.1f));
	vertices[3].Normal = add(direction, mul(vertices[3].Position, 0.1f));

	vertices[0].Tex = Float2(uvBottomRight.x, uvTopLeft.y);
	vertices[1].Tex = uvTopLeft;
	vertices[2].Tex = uvBottomRight;
	vertices[3].Tex = Float2(uvTopLeft.x, uvBottomRight.y);

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
