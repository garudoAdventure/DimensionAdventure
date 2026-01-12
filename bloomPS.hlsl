SamplerState g_sampler : register(s0);
Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
Texture2D g_texture2 : register(t2);
Texture2D g_texture3 : register(t3);
Texture2D g_texture4 : register(t4);

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float4 main(VSOutput In) : SV_Target0
{
    float3 col0 = g_texture0.Sample(g_sampler, In.tex).rgb;
    float3 col1 = g_texture1.Sample(g_sampler, In.tex).rgb;
    float3 col2 = g_texture2.Sample(g_sampler, In.tex).rgb;
    float3 col3 = g_texture3.Sample(g_sampler, In.tex).rgb;
    float3 col4 = g_texture4.Sample(g_sampler, In.tex).rgb;
    
    return float4(col0 + col1 + col2 + col3 + col4, 1.0);
}