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

// The texture sent to the shader and the sample state used to sample it
Texture2D stexture;
SamplerState sampleType;

// Minimum Albedo lighting
#define MIN_DIFFUSE_ALBEDO 0.25f

// Interpolated values passed by the vertex shader
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 N : N;
	float3 L : L;
};

// Pixel shader entry point
float4 main(PixelInputType input) : SV_TARGET
{
	// Normalize inputs
	float3 N = normalize(input.N);
	float3 L = normalize(input.L);

	float4 diffuse = max(dot(N, L), MIN_DIFFUSE_ALBEDO) * diffuseAlbedo;

	return stexture.Sample(sampleType, input.tex) * input.color * float4(diffuse.xyz, max(1.0f, diffuse.a));
}
