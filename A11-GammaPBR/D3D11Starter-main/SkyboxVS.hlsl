// might need to change register down the line, keep an eye on
cbuffer DataFromCPU : register(b0) {
    float4x4 viewMatrix;
    float4x4 projMatrix;
}

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

struct VertexToPixel {
    float4 position : SV_POSITION;
    float3 sampleDir : DIRECTION;
};

VertexToPixel main( VertexShaderInput input )
{
    // Creating output returnable
    VertexToPixel output;
    
    // Creating view matrix and zeroing out translation
    float4x4 viewNoTranslation = viewMatrix;
    viewNoTranslation._14 = 0;
    viewNoTranslation._24 = 0;
    viewNoTranslation._34 = 0;
    
    // Setting output position
    matrix vp = mul(projMatrix, viewNoTranslation);
    output.position = mul(vp, float4(input.localPosition, 1.0f));
    output.position.z = output.position.w;
    
    // Setting output sample direction
    output.sampleDir = input.localPosition;
    
	return output;
}