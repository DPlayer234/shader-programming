// The CBuffer with the uniforms
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

// Per-Vertex data passed to the vertex shader
struct VertexInputType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
	float3 normal : NORMAL;
};

// Values to pass to the Pixel Shader
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 N : N;
	float3 L : L;
};

// Vertex shader entry point
PixelInputType main(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;

	// Calculate Model-View Matrix
	matrix mv = mul(world, view);

	// Calculate the world-position of the vertex
	float4 p = mul(input.position, mv);

	// Calculate transformed normal vector
	output.N = mul(input.normal, (float3x3)mv);

	// Get the light direction in relation to the camera
	output.L = mul(lightPos, view) - p.xyz;

	// Calculate the screen position of the vertex
	output.position = mul(p, projection);

	// Pass on these values
	output.color = input.color;
	output.tex = input.tex;

	return output;
}
