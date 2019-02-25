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
#define MIN_DIFFUSE_ALBEDO 0.15f

// Source: https://gamedev.stackexchange.com/questions/32681/random-number-hlsl
float2 rand_2_10(in float2 uv)
{
    float noiseX = (frac(sin(dot(uv, float2(12.9898,78.233) * 2.0)) * 4375.85453));
    float noiseY = sqrt(1 - noiseX * noiseX);
    return float2(noiseX, noiseY);
}

// Interpolated values passed by the vertex shader
struct PixelInputType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD0;
	float3 V : V;
	float3 N : N;
	float3 L : L;
};

// Pixel shader entry point
float4 main(PixelInputType input) : SV_TARGET
{
	// Calculate Model-View Matrix
	matrix mv = mul(world, view);

	// Random
	float2 r = rand_2_10(input.tex) * 0.8;

	// Normalize inputs
	float3 V = normalize(input.V);
	float3 N = normalize(input.N);
	float3 L = normalize(input.L);

	N += normalize(mul(float3(r.x, 1.0f, r.y), (float3x3)mv)) * 0.8;

	// Compute the half-way vector for better performance than reflect()
	float3 H = normalize(L + V);

	float4 diffuse = max(dot(N, L), MIN_DIFFUSE_ALBEDO) * diffuseAlbedo;
	float4 specular = pow(max(dot(N, H), 0.0f), specularPower) * specularAlbedo;
	
	return stexture.Sample(sampleType, input.tex) * input.color * float4((diffuse + specular).xyz, max(1.0f, diffuse.a + specular.a));
}
