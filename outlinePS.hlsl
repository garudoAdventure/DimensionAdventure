Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

bool hasTexture : register(b0);

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float4 main(VSOutput In) : SV_Target0
{
    return float4(1.0f, 0.0f, 0.0f, 1.0f);
}