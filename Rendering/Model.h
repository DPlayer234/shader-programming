#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "defines.h"
#include "typedefs.h"
#include "Vertex.h"
#include "Shader.h"

// A renderable model.
// Requires setting a shader.
class Model
{
public:
	// Initializes the model
	bool Initialize(ID3D11Device* device, HWND hwnd);

	// Releases all memory associated with the object
	void Release();

	// Renders the model with the given view and projection matrix as well as light point position.
	// This will fail if no shader is set.
	bool Render(ID3D11DeviceContext* context, const Matrix& view, const Matrix& projection, const Float3& lightPos);

	// Gets the count of indices for the model
	UINT GetIndexCount();

	// Sets the position of the model in world space
	void SetPosition(Float3 position);

	// Sets the rotation of the model in world space
	void SetRotation(Float3 rotation);

	// Sets the scale of the model in world space
	void SetScale(Float3 scale);

	// Returns the world space position of the model
	Float3 GetPosition();

	// Returns the world space rotation of the model
	Float3 GetRotation();

	// Returns the world space scale of the model
	Float3 GetScale();

	// Sets the shader the model is using.
	// Setting it NULL prevents rendering.
	void SetShader(Shader* shader);

	// Returns the pointer to the shader the model is using.
	Shader* GetShader();

	// Gets the world/model transformation matrix
	Matrix GetWorldMatrix();

protected:
	// Initializes the vertex buffer of the model
	bool InitializeVertexBuffer(ID3D11Device* device);

	// Initializes the index buffer of the model
	bool InitializeIndexBuffer(ID3D11Device* device);

	// Releases the vertex and index array of the model.
	// delete[]s both by default.
	virtual void ReleaseArrays();

	// Creates the vertex array of the model.
	virtual bool CreateVertexArray() = 0;

	// Creates the index array of the model.
	virtual bool CreateIndexArray() = 0;

	// Stores the pointer to the shader.
	Shader* shader = nullptr;

	// The associated vertex buffer
	ID3D11Buffer* vertexBuffer = nullptr;

	// The associated index buffer
	ID3D11Buffer* indexBuffer = nullptr;

	// The amount of vertices this model has
	int vertexCount = 0;

	// The amount of indices this model has
	int indexCount = 0;

	// The pointer to the vertex array
	Vertex* vertexArray = nullptr;

	// The pointer to the index array
	UINT* indexArray = nullptr;

	// Stores the position of the model in world space
	Float3 position = Float3(0.0f, 0.0f, 0.0f);

	// Stores the rotation of the model in world space
	Float3 rotation = Float3(0.0f, 0.0f, 0.0f);

	// Stores the scale of the model in world space
	Float3 scale = Float3(1.0f, 1.0f, 1.0f);
};
