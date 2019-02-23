#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "defines.h"
#include "typedefs.h"
#include "Vertex.h"
#include "Shader.h"

class Model
{
public:
	Model();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Release();

	virtual void Update(float deltaTime);
	bool Render(ID3D11DeviceContext* context, const Matrix& view, const Matrix& projection, const Float3& LightPos);

	UINT GetIndexCount()
	{
		return indexCount;
	};

	void SetPosition(Float3 position);
	void SetRotation(Float3 rotation);
	void SetScale(Float3 scale);

	Float3 GetPosition();
	Float3 GetRotation();
	Float3 GetScale();

	void SetShader(Shader* shader);
	Shader* GetShader();

	Matrix GetWorldMatrix();

protected:
	bool InitializeVertexBuffer(ID3D11Device* device);
	bool InitializeIndexBuffer(ID3D11Device* device);

	virtual void ReleaseArrays();
	virtual bool CreateVertexArray() = 0;
	virtual bool CreateIndexArray() = 0;

	Shader* shader = nullptr;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	int vertexCount = 0;
	int indexCount = 0;

	UINT* indexArray = nullptr;
	Vertex* vertexArray = nullptr;

	Float3 position = Float3(0.0f, 0.0f, 0.0f);
	Float3 rotation = Float3(0.0f, 0.0f, 0.0f);
	Float3 scale = Float3(1.0f, 1.0f, 1.0f);
	Float4x4 worldMatrix;
};

#define MODEL_SHADER(NEW_CLASS_NAME, MODEL_CLASS, SHADER_CLASS) \
class NEW_CLASS_NAME : public MODEL_CLASS \
{ \
protected: \
	bool InitializeShader(ID3D11Device* device, HWND hwnd) override \
	{ \
		bool result; \
		\
		shader = new SHADER_CLASS; \
		if (!shader) return false; \
		\
		result = shader->Initialize(device, hwnd); \
		if (!result) return false; \
		\
		return true; \
	} \
}

