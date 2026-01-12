SamplerState g_sampler : register(s0);
Texture2D g_texture : register(t0);

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float4 main(VSOutput In) : SV_Target0
{
    float4 col = g_texture.Sample(g_sampler, In.tex) * In.col;
    float luminance = 0.299 * col.r + 0.587 * col.g + 0.114 * col.b;
    clip(luminance - 0.4);
    return col;
}