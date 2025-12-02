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
    float4 color = g_texture.Sample(g_sampler, In.tex) * In.col;
    float luminance = dot(float3(0.299f, 0.587f, 0.114f), color.rgb);
    color = luminance > 0.6 ? color : float4(0.0f, 0.0f, 0.0f, 0.0f);
    return color;
}