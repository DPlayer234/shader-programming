#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

using Float2 = DirectX::XMFLOAT2;
using Float3 = DirectX::XMFLOAT3;
using Float4 = DirectX::XMFLOAT4;

struct Vertex
{
	Float3 Position;
	Float4 Color;
	Float3 Normal;
};
