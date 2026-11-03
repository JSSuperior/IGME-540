cbuffer DataFromCPU : register(b0)
{
    float4 colorTint;
    float2 totalTime;
    float2 scale;
    float2 offset;
}

Texture2D SurfaceTexture : register(t0);
Texture2D AdditionalTexture : register(t1);
SamplerState BasicSampler : register(s0);

struct VertexToPixel
{
    float4 screenPosition : SV_POSITION;
    float2 uv : TEXTCOORD;
    float3 normal : NORMAL;
	//float4 color			: COLOR;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    float4 surfaceColor = SurfaceTexture.Sample(BasicSampler, (input.uv * scale + offset)) * colorTint;
    float4 additionalColor = AdditionalTexture.Sample(BasicSampler, (input.uv * scale + offset)) * colorTint;
    return surfaceColor + additionalColor;
}