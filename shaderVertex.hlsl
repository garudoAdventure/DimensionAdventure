cbuffer Mat : register(b0) 
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
}

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

VSOutput main(float4 pos : POSITION, float4 color : COLOR, float4 normal : NORMAL, float2 tex : TEXCOORD0)
{
    VSOutput VSOut;
    
    VSOut.pos = pos;
    VSOut.pos = mul(VSOut.pos, world);
    VSOut.pos = mul(VSOut.pos, view);
    VSOut.pos = mul(VSOut.pos, projection);
    
    float3 nor = mul(normal, world);
    float3 light = { 0.0f, 0.5f, -1.0f };
    float col;
    nor = normalize(nor);
    light = normalize(light);
    col = saturate(-dot(light, nor));
    col = col + 0.2f;
    
    VSOut.col = float4(col, col, col, 1.0f) * color;
    
    VSOut.col = color;
    
    VSOut.tex = tex;
    
    return VSOut;
}

