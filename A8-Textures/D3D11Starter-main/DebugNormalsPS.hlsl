cbuffer DataFromCPU : register(b0)
{
    float4 colorTint;
    float2 totalTime;
    float2 scale;
    float2 offset;
}

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
    float4 screenPosition : SV_POSITION;
    float2 uv : TEXTCOORD;
    float3 normal : NORMAL;
	//float4 color			: COLOR;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    return float4(input.normal, 1);
}