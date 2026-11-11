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
Texture2D NormalTexture : register(t1);
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
    // normalizing input normals/tangents
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    
    // Unpacking normal
    float3 unpackedNormal = normalize(NormalTexture.Sample(BasicSampler, (input.uv * scale + offset)).rgb * 2.0f - 1.0f);
    
    // Creating matrix
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent - dot(input.tangent, N) * N);
    float3 B = cross(T, N);
    float3x3 TBN = float3x3(T, B, N);
    
    input.normal = mul(unpackedNormal, TBN);
    
    // Sampling texture
    float4 surfaceColor = SurfaceTexture.Sample(BasicSampler, (input.uv * scale + offset));
    
    // applying ambient light and color tint to surface color
    surfaceColor = surfaceColor * colorTint * ambientColor;
    
    // Adding together all lights in the scene
    // need to make sure to update this value when changed elsewhere
    for (int i = 0; i < 5; i++)
    {
        switch (lights[i].Type)
        {
            case LIGHT_TYPE_DIRECTIONAL:
                surfaceColor += directionalLight(surfaceColor, input.normal, lights[i], cameraPos, input.worldPosition);
                break;
            case LIGHT_TYPE_POINT:
                surfaceColor += pointLight(surfaceColor, input.normal, lights[i], cameraPos, input.worldPosition);
                break;
            case LIGHT_TYPE_SPOT:
                surfaceColor += spotLight(surfaceColor, input.normal, lights[i], cameraPos, input.worldPosition);
                break;
        }
        
    }
    
    // Return final surface color with all applied lights
    return surfaceColor;
}