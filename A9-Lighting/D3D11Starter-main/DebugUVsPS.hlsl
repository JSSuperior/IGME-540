#include "ShaderInclusions.hlsli"

cbuffer DataFromCPU : register(b0)
{
    Light light;
    float4 colorTint;
    float4 ambientColor;
    float2 totalTime;
    float2 scale;
    float2 offset;
    float2 padding;
    float3 cameraPos;
}

float4 main(VertexToPixel input) : SV_TARGET
{
    return float4(input.uv, 0, 1);
    //return float4(input.normal, 1);
}