Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

cbuffer PixelTime : register(b2)
{
    float time;
    float3 dummy;
};

float noise(float seed)
{
    float num = 543.2543f * sin(dot(float2(seed, seed), float2(3525.46f, -54.3415f)));
    return num - floor(num);
}

float4 main(VSOutput In) : SV_Target0
{
    float blockSize = 50.0f;
    float shakePower = 0.03f;
    float shakeSpeed = 0.1f;
    float shakeRate = 0.2f;
    float shakeColorRate = 0.02f;
    float shiftEnable = float(noise(trunc(time * shakeSpeed)) < shakeRate);
    
    float2 uvTex = In.tex;
    uvTex.x += (noise((trunc(uvTex.y * blockSize) / blockSize) + time) - 0.5f) * shakePower * shiftEnable;
    
    float4 color = g_texture.Sample(g_sampler, uvTex);
    color.r = lerp(color.r, g_texture.Sample(g_sampler, uvTex + float2(shakeColorRate, 0.0f)).r, shiftEnable);    
    color.b = lerp(color.b, g_texture.Sample(g_sampler, uvTex + float2(-shakeColorRate, 0.0f)).b, shiftEnable);
    
    return color;
}