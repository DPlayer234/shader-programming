cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
	float3 lightPos;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD;
	float4 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD;
	float4 color : COLOR;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.color = input.color;

	return output;
}
