#include "PlaneModel.h"

bool PlaneModel::CreateVertexArray()
{
	vertexCount = (MAX_VERTEX - MIN_VERTEX) * (MAX_VERTEX - MIN_VERTEX) * 4;
	vertexArray = new Vertex[vertexCount];

	int x, z;
	unsigned int index = 0;

	for (x = MIN_VERTEX; x < MAX_VERTEX; x++)
	{
		for (z = MIN_VERTEX; z < MAX_VERTEX; z++)
		{
			SetVertices(vertexArray + index, Float3(x, 0.0f, z));
			index += 4;
		}
	}

	return true;
}

bool PlaneModel::CreateIndexArray()
{
	indexCount = (MAX_VERTEX - MIN_VERTEX) * (MAX_VERTEX - MIN_VERTEX) * 6;
	indexArray = new UINT[indexCount];

	unsigned int index = 0;

	for (unsigned int i = 0; i < indexCount; i += 6)
	{
		SetIndices(indexArray + i, index);
		index += 4;
	}

	return true;
}

void PlaneModel::SetVertices(Vertex vertices[4], Float3 minLocation)
{
	vertices[0].Position = minLocation;
	vertices[1].Position = Float3(minLocation.x + 1.0f, minLocation.y, minLocation.z);
	vertices[2].Position = Float3(minLocation.x, minLocation.y, minLocation.z + 1.0f);
	vertices[3].Position = Float3(minLocation.x + 1.0f, minLocation.y, minLocation.z + 1.0f);

	Float3 normal(0.0f, 1.0f, 0.0f);

	vertices[0].Normal = normal;
	vertices[1].Normal = normal;
	vertices[2].Normal = normal;
	vertices[3].Normal = normal;

	vertices[0].Tex = Float2(0.0f, 0.0f);
	vertices[1].Tex = Float2(1.0f, 0.0f);
	vertices[2].Tex = Float2(0.0f, 1.0f);
	vertices[3].Tex = Float2(1.0f, 1.0f);

	Float4 color(1.0f, 1.0f, 1.0f, 1.0f);

	vertices[0].Color = color;
	vertices[1].Color = color;
	vertices[2].Color = color;
	vertices[3].Color = color;
}

void PlaneModel::SetIndices(UINT indices[6], UINT firstIndex)
{
	indices[0] = firstIndex + 0;
	indices[1] = firstIndex + 2;
	indices[2] = firstIndex + 1;
	indices[3] = firstIndex + 1;
	indices[4] = firstIndex + 2;
	indices[5] = firstIndex + 3;
}
