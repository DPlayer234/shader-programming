#include "Model.h"

bool Model::Initialize(ID3D11Device* device, HWND hwnd)
{
	if (!InitializeVertexBuffer(device)) return false;
	if (!InitializeIndexBuffer(device)) return false;

	return true;
}

void Model::Release()
{
	RELEASE_N(vertexBuffer);
	RELEASE_N(indexBuffer);
}

bool Model::Render(ID3D11DeviceContext* context, const Matrix& view, const Matrix& projection, const Float3& lightPos)
{
	if (!shader) return false;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	UniformBufferType uniforms;

	uniforms.World = GetWorldMatrix();
	uniforms.View = view;
	uniforms.Projection = projection;
	uniforms.LightPos = Float4(lightPos.x, lightPos.y, lightPos.z, 1.0f);

	return shader->Render(context, GetIndexCount(), uniforms);
}

UINT Model::GetIndexCount()
{
	return indexCount;
}

void Model::SetPosition(Float3 position)
{
	this->position = position;
}

void Model::SetRotation(Float3 rotation)
{
	this->rotation = rotation;
}

void Model::SetScale(Float3 scale)
{
	this->scale = scale;
}

Float3 Model::GetPosition()
{
	return position;
}

Float3 Model::GetRotation()
{
	return rotation;
}

Float3 Model::GetScale()
{
	return scale;
}

void Model::SetShader(Shader* shader)
{
	this->shader = shader;
}

Shader* Model::GetShader()
{
	return shader;
}

Matrix Model::GetWorldMatrix()
{
	Matrix translationM = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	Matrix rotationM = DirectX::XMMatrixRotationRollPitchYaw(
		DirectX::XMConvertToRadians(rotation.x),
		DirectX::XMConvertToRadians(rotation.y),
		DirectX::XMConvertToRadians(rotation.z));

	Matrix scaleM = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	return scaleM * rotationM * translationM;
}

bool Model::InitializeVertexBuffer(ID3D11Device* device)
{
	if (!CreateVertexArray()) return false;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(Vertex) * vertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = vertexArray;

	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &vertexBuffer);
	if (FAILED(result)) return false;

	return true;
}

bool Model::InitializeIndexBuffer(ID3D11Device* device)
{
	if (!CreateIndexArray()) return false;

	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(UINT) * indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = indexArray;

	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &indexBuffer);
	if (FAILED(result)) return false;

	return true;
}

void Model::ReleaseArrays()
{
	DELETE_A(vertexArray);
	DELETE_A(indexArray);
}
