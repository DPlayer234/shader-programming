#pragma once

#include "Model.h"

class PlaneModel : public Model
{
protected:
	bool CreateVertexArray() override;
	bool CreateIndexArray() override;

private:
	const int MIN_VERTEX = -5;
	const int MAX_VERTEX = 5;

	void SetVertices(Vertex vertices[4], Float3 minLocation);
	void SetIndices(UINT indices[6], UINT firstIndex);
};

