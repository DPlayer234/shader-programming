cbuffer UniformBuffer
{
	matrix world;
	matrix view;
	matrix projection;
	float3 lightPos;

	float4 diffuseAlbedo;
	float4 specularAlbedo;
	float specularPower;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float4 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float4 worldPos : POSITION;
	float3 normal : NORMAL;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;

	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);
	
	output.color = input.color;
	output.normal = normalize(mul(input.normal, (float3x3)world));

	return output;
}
