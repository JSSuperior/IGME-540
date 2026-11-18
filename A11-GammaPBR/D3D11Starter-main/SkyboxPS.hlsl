TextureCube SkyBoxTexture : register(t0);
SamplerState Sampler : register(s0);

struct VertexToPixel
{
    float4 position : SV_POSITION;
    float3 sampleDir : DIRECTION;
};

float4 main(VertexToPixel input) : SV_TARGET
{
    return SkyBoxTexture.Sample(Sampler, input.sampleDir);
}