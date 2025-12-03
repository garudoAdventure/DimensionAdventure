SamplerState g_sampler : register(s0);

Texture2D g_texture : register(t0);

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

cbuffer PixelConstBuffer : register(b1)
{
    bool isVertical;
    float width;
    float height;
    float dummy2;
}

float4 main(VSOutput In) : SV_Target0
{
    float dx = 1.0 / width;
    float dy = 1.0 / height;
    float weight[5] = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
    
    float3 blurColor = g_texture.Sample(g_sampler, In.tex).rgb * weight[0];
    int2 direction = isVertical ? int2(1, 0) : int2(0, 1);
    
    for (int i = 1; i < 5; i++)
    {
        blurColor += g_texture.Sample(g_sampler, In.tex + direction * float2(dx * i, dy * i)).rgb * weight[i];
        blurColor += g_texture.Sample(g_sampler, In.tex - direction * float2(dx * i, dy * i)).rgb * weight[i];
    }
    
    return In.col * float4(blurColor, 1.0f);
}