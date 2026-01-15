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
    float2 uv = In.tex - float2(0.5, 0.5);
    float distanceFromCenter = length(uv);
    float angle = distanceFromCenter * 4 * 3.1415;
    float c = cos(angle);
    float s = sin(angle);

    uv = mul(uv, float2x2(c, -s, s, c)) + float2(0.5, 0.5);

    float4 texColor = g_texture.Sample(g_sampler, uv);

    return texColor * In.col + 0.1f;
}