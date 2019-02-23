#pragma once

#include "Model.h"

class CubeModel : public Model
{
public:
	CubeModel();

protected:
	void ReleaseArrays() override;
	bool CreateVertexArray() override;
	bool CreateIndexArray() override;

private:
	static UINT indices[36];
	void SetVertices(Vertex vertices[4], Float3 direction, Float3 offset1, Float3 offset2);
};
