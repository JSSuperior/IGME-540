#include "ShaderInclusions.hlsli"
#include "Lights.h"

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

Texture2D SurfaceTexture : register(t0);
SamplerState BasicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
    float4 surfaceColor = SurfaceTexture.Sample(BasicSampler, (input.uv * scale + offset)) * colorTint * ambientColor;
    
    input.normal = normalize(input.normal);
    //return float4(input.normal, 1);
	//return surfaceColor;
    return float4(light.Color, 1);
    //return float4(cameraPos, 1);
    //return float4(input.uv, 0, 1);
    //return float4(input.normal, 1);

}