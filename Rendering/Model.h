#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "defines.h"
#include "Vertex.h"
#include "Shader.h"

using Vector = DirectX::XMVECTOR;
using Matrix = DirectX::XMMATRIX;
using Float3 = DirectX::XMFLOAT3;
using Float4x4 = DirectX::XMFLOAT4X4;

class Model
{
public:
	Model();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Release();

	virtual void Update(float deltaTime);
	bool Render(ID3D11DeviceContext* context, Matrix view, Matrix projection);

	UINT GetIndexCount()
	{
		return indexCount;
	};

	void SetPosition(Vector position);
	void SetRotation(Vector rotation);
	void SetScale(Vector scale);

	Matrix GetWorldMatrix();

protected:
	bool InitializeVertexBuffer(ID3D11Device* device);
	bool InitializeIndexBuffer(ID3D11Device* device);

	virtual bool InitializeShader(ID3D11Device* device, HWND hwnd) = 0;
	virtual bool CreateVertexArray() = 0;
	virtual bool CreateIndexArray() = 0;

	Shader* shader = nullptr;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	int vertexCount = 0;
	int indexCount = 0;

	UINT* indexArray = nullptr;
	Vertex* vertexArray = nullptr;

	Float3 position = Float3(0.f, 0.f, 0.f);
	Float3 rotation = Float3(0.f, 0.f, 0.f);
	Float3 scale = Float3(1.f, 1.f, 1.f);
	Float4x4 worldMatrix;
};

