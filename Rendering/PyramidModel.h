#pragma once

#include "Model.h"

class PyramidModel : public Model
{
protected:
	void ReleaseArrays() override;
	bool CreateVertexArray() override;
	bool CreateIndexArray() override;

private:
	static UINT indices[18];
	void SetTriangleVertices(Vertex vertices[3], Float3 normal, Float3 point1, Float3 point2, Float3 point3);
	void SetGroundVertices(Vertex vertices[4], Float3 direction, Float3 offset1, Float3 offset2);
};
