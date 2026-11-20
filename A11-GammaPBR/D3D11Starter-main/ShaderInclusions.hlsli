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

// Normal Distribution Function, Trowbridge-Reitz
float D_GGX(float3 n, float3 h, float roughness)
{
    // Constant values
    const float PI = 3.14159f;
    const float MINIMUM_ROUGHNESS = 0.0000001f;
    
    // Precalculated values to be plugged in
    float NdotH = saturate(dot(n, h));
    float NdotH2 = NdotH * NdotH;
    float a = roughness * roughness;
    float a2 = max(a * a, MINIMUM_ROUGHNESS);
    
    float denomToSquare = NdotH2 * (a2 - 1) + 1;
    return a2 / (PI * denomToSquare * denomToSquare);
};

// Fresnel Function, Schlick's Approximation
float3 F_Schlick(float3 v, float3 h, float3 f0)
{
    float vdotH = saturate(dot(v, h));
    return f0 + (1 - f0) * pow(1 - vdotH, 5);
};

// Geometric Shadowing, Schlick-GGX
float G_SchlickGGX(float3 n, float3 v, float roughness)
{
    float k = pow(roughness + 1, 2) / 8.0f;
    float NdotV = saturate(dot(n, v));
    return 1 / (NdotV * (1 - k) + k);
};

// Full PBR calculation
float3 MicrofacetBRDF(float3 n, float3 l, float3 v, float roughness, float3 f0)
{
    float3 h = normalize(v + l);
    
    // Performing all 3 parts of BRDF
    float D = D_GGX(n, h, roughness);
    float3 F = F_Schlick(v, h, f0);
    float G = G_SchlickGGX(n, v, roughness) * G_SchlickGGX(n, l, roughness);
    
    // Combining them together in the final formula
    return (D * F * G) / 4;
};

// Energy Conservation calculation
float3 DiffuseEnergyConserve(float3 diffuse, float3 F, float metalness)
{
    return diffuse * (1 - F) * (1 - metalness);
}

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
    return saturate(dot(_normal, _directionToLight));
};

// Specular term
//float3 calcSpecularTerm(float3 _normal, float3 _directionToLight, float3 _cameraPosition, float3 _worldPosition)
//{
//    float3 refl = reflect(normalize(-_directionToLight), _normal);
//    float RdotV = saturate(dot(refl, normalize(_cameraPosition - _worldPosition)));
//    return pow(RdotV, 128);
//};

// Directional light calculation
float4 directionalLight(float4 _surfaceColor, float3 _normal, Light _light, float3 _cameraPosition, float3 _worldPosition, float _roughness, float3 _specularColor, float _metalness)
{
    float3 directionToLight = -normalize(_light.Direction);
    float3 directionToCamera = normalize(_cameraPosition - _worldPosition);
    
    // Calculate diffuse term and specular term
    float3 diffuseTerm = calcDiffuseTerm(_normal, directionToLight);
    float3 specularTerm = MicrofacetBRDF(_normal, directionToLight, directionToCamera, _roughness, _specularColor);
    
    specularTerm *= saturate(dot(_normal, directionToLight));
    
    // Calculating diffuse with energy conservation
    float3 F = F_Schlick(directionToCamera, normalize(directionToLight + directionToCamera), _specularColor);
    float3 balancedDiff = DiffuseEnergyConserve(diffuseTerm, F, _metalness);
    
    // Add terms together and multiply by light settings + surface color
    return float4((balancedDiff * _surfaceColor.xyz + specularTerm) * _light.Color * _light.Intensity, 1);
};

// Point light calculation
float4 pointLight(float4 _surfaceColor, float3 _normal, Light _light, float3 _cameraPosition, float3 _worldPosition, float _roughness, float3 _specularColor, float _metalness)
{
    float3 directionToLight = (_light.Position - _worldPosition);
    float3 directionToCamera = normalize(_cameraPosition - _worldPosition);
    
    // Calculate attenation, diffuse term and specular term
    float attenuation = calcAttenuation(_light, _worldPosition);
    float3 diffuseTerm = calcDiffuseTerm(_normal, directionToLight);
    float3 specularTerm = MicrofacetBRDF(_normal, directionToLight, directionToCamera, _roughness, _specularColor);
    
    specularTerm *= saturate(dot(_normal, directionToLight));
    
    // Calculating diffuse with energy conservation
    float3 F = F_Schlick(directionToCamera, normalize(directionToLight + directionToCamera), _specularColor);
    float3 balancedDiff = DiffuseEnergyConserve(diffuseTerm, F, _metalness);
    
    // Add terms together and multiply by light settings + surface color + attenuation
    return float4((balancedDiff * _surfaceColor.xyz + specularTerm) * _light.Color * _light.Intensity * attenuation, 1);
};

// Spot light calculation
float4 spotLight(float4 _surfaceColor, float3 _normal, Light _light, float3 _cameraPosition, float3 _worldPosition, float _roughness, float3 _specularColor, float _metalness)
{
    // Calculate cos angle between pixel and light direction
    float pixelAngle = saturate(dot(-normalize(_light.Position - _worldPosition), normalize(_light.Direction)));
    
    // Calculate light falloff range
    float cosOuter = cos(_light.SpotOuterAngle);
    float cosInner = cos(_light.SpotInnerAngle);
    float falloffRange = cosOuter - cosInner;
    
    // Calculate/Apply linear fall off to light
    float spotTerm = saturate((cosOuter - pixelAngle) / falloffRange);
    return pointLight(_surfaceColor, _normal, _light, _cameraPosition, _worldPosition, _roughness, _specularColor, _metalness) * spotTerm;
}
#endif