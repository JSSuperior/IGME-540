// Used the demo for help when I got stuck
// https://github.com/vixorien/ggp-demos/blob/main/GGP/09%20-%20Basic%20Lighting/Lighting.hlsli

#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__

#define LIGHT_TYPE_DIRECTIONAL	0
#define LIGHT_TYPE_POINT		1
#define LIGHT_TYPE_SPOT			2

struct Light
{
    int Type;
    float3 Direction;
    float Range;
    float3 Position;
    float Intensity;
    float3 Color;
    float SpotInnerAngle;
    float SpotOuterAngle;
    float2 Padding;
};
    
// Struct representing a single vertex worth of data
// - This should match the vertex definition in our C++ code
// - By "match", I mean the size, order and number of members
// - The name of the struct itself is unimportant, but should be descriptive
// - Each variable must have a semantic, which defines its usage
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float3 localPosition : POSITION; // XYZ position
	//float4 color			: COLOR;        // RGBA color
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float4 screenPosition : SV_POSITION; // XYZW position (System Value Position)
    float2 uv : TEXTCOORD;
    float3 normal : NORMAL;
    float3 worldPosition : POSITION;
    float3 tangent : TANGENT;
    //float3 tangent : TANGENT;
};

// Helper functions
//Attenuate
float calcAttenuation(Light _light, float3 _worldPosition)
{
    float dist = distance(_light.Position, _worldPosition);
    float att = saturate(1.0f - (dist * dist / (_light.Range * _light.Range)));
    return att * att;
};

// Difuse term
float3 calcDiffuseTerm(float3 _normal, float3 _directionToLight)
{
    return saturate(dot(_normal, normalize(_directionToLight)));
};

// Specular term
float3 calcSpecularTerm(float3 _normal, float3 _directionToLight, float3 _cameraPosition, float3 _worldPosition)
{
    float3 refl = reflect(normalize(-_directionToLight), _normal);
    float RdotV = saturate(dot(refl, normalize(_cameraPosition - _worldPosition)));
    return pow(RdotV, 128);
};

// Directional light calculation
float4 directionalLight(float4 _surfaceColor, float3 _normal, Light _light, float3 _cameraPosition, float3 _worldPosition)
{
    // Calculate diffuse term and specular term
    float3 diffuseTerm = calcDiffuseTerm(_normal, -normalize(_light.Direction));
    float3 specularTerm = calcSpecularTerm(_normal, -normalize(_light.Direction), _cameraPosition, _worldPosition);
    
    // Cutting specular if diffuse contribution is 0
    specularTerm *= any(diffuseTerm);
    
    // Add terms together and multiply by light settings + surface color
    return ((diffuseTerm * _surfaceColor.xyz + specularTerm) * _light.Color * _light.Intensity).xyzz;
};

// Point light calculation
float4 pointLight(float4 _surfaceColor, float3 _normal, Light _light, float3 _cameraPosition, float3 _worldPosition)
{
    // Calculate attenation, diffuse term and specular term
    float attenuation = calcAttenuation(_light, _worldPosition);
    float3 diffuseTerm = calcDiffuseTerm(_normal, (_light.Position - _worldPosition));
    float3 specularTerm = calcSpecularTerm(_normal, (_light.Position - _worldPosition), _cameraPosition, _worldPosition);
    
    // Cutting specular if diffuse contribution is 0
    specularTerm *= any(diffuseTerm);
    
    // Add terms together and multiply by light settings + surface color + attenuation
    return ((diffuseTerm * _surfaceColor.xyz + specularTerm) * _light.Color * _light.Intensity * attenuation).xyzz;
};

// Spot light calculation
float4 spotLight(float4 _surfaceColor, float3 _normal, Light _light, float3 _cameraPosition, float3 _worldPosition)
{
    // Calculate cos angle between pixel and light direction
    float pixelAngle = saturate(dot(-normalize(_light.Position - _worldPosition), normalize(_light.Direction)));
    
    // Calculate light falloff range
    float cosOuter = cos(_light.SpotOuterAngle);
    float cosInner = cos(_light.SpotInnerAngle);
    float falloffRange = cosOuter - cosInner;
    
    // Calculate/Apply linear fall off to light
    float spotTerm = saturate((cosOuter - pixelAngle) / falloffRange);
    return pointLight(_surfaceColor, _normal, _light, _cameraPosition, _worldPosition) * spotTerm;
}
#endif