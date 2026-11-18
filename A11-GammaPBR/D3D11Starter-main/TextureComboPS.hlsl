#include "ShaderInclusions.hlsli"

cbuffer DataFromCPU : register(b0)
{
    Light lights[5];
    float4 colorTint;
    float4 ambientColor;
    float2 totalTime;
    float2 scale;
    float2 offset;
    float2 padding;
    float3 cameraPos;
}

Texture2D SurfaceTexture : register(t0);
Texture2D AdditionalTexture : register(t1);
SamplerState BasicSampler : register(s0);

float4 main(VertexToPixel input) : SV_TARGET
{
    float4 surfaceColor = SurfaceTexture.Sample(BasicSampler, (input.uv * scale + offset)) * colorTint;
    float4 additionalColor = AdditionalTexture.Sample(BasicSampler, (input.uv * scale + offset)) * colorTint;
    return surfaceColor + additionalColor;
}