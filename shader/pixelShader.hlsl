Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float4 main(VSOutput In) : SV_Target0
{
    float4 outCol = In.col * g_texture.Sample(g_sampler, In.tex);
    return outCol;
}