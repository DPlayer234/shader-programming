cbuffer UniformBuffer
{
	matrix world;
	matrix view;
	matrix projection;
	float4 lightPos;

	float4 diffuseAlbedo;
	float4 specularAlbedo;
	float specularPower;
};

struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 V : V;
	float3 N : N;
	float3 L : L;
};

PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;

	matrix mv = mul(world, view);
	float4 p = mul(input.position, mv);

	output.N = mul(input.normal, (float3x3)mv);
	output.L = mul(lightPos, view) - p.xyz;
	output.V = -p.xyz;

	output.position = mul(p, projection);

	output.color = input.color;
	output.tex = input.tex;

	return output;
}
