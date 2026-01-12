SamplerState g_sampler : register(s0);
Texture2D g_texture0 : register(t0);
Texture2D g_texture1 : register(t1);
Texture2D g_texture2 : register(t2);
Texture2D g_texture3 : register(t3);
Texture2D g_texture4 : register(t4);

cbuffer PixelConstBuffer : register(b0)
{
    bool isVertical;
    float width;
    float height;
    int bloomPower;
}

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float4 main(VSOutput In) : SV_Target0
{
    float3 col[5] =
    {
        g_texture0.Sample(g_sampler, In.tex).rgb,
        g_texture1.Sample(g_sampler, In.tex).rgb,
        g_texture2.Sample(g_sampler, In.tex).rgb,
        g_texture3.Sample(g_sampler, In.tex).rgb,
        g_texture4.Sample(g_sampler, In.tex).rgb
    };
    
    float3 outColor = { 0.0f, 0.0f, 0.0f };
    for (int i = 0; i < bloomPower; i++)
    {
        outColor += col[i];
    }
    
    return float4(outColor, 1.0);
}