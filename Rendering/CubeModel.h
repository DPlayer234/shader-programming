#pragma once

#include "Model.h"

class CubeModel : public Model
{
public:
	CubeModel();

protected:
	bool InitializeShader(ID3D11Device* device, HWND hwnd) override;
	bool CreateVertexArray() override;
	bool CreateIndexArray() override;

private:
	void SetVertices(Vertex vertices[4], Float3 direction, Float3 offset1, Float3 offset2);
};

