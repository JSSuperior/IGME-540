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

float4 main(VertexToPixel input) : SV_TARGET
{
    return float4(sin(totalTime.r * input.normal.r), cos(totalTime.r * input.normal.g), tan(totalTime.r * input.normal.b), 1);
}