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

VSOutput main(
    float4 pos : POSITION,
    float4 color : COLOR,
    float3 normal : NORMAL,
    float2 tex : TEXCOORD0,

    float4 world0 : WORLD0,
    float4 world1 : WORLD1,
    float4 world2 : WORLD2,
    float4 world3 : WORLD3,
    float4 insCol : INS_COL
)
{
    row_major float4x4 insWorld = float4x4(world0, world1, world2, world3);
    
    VSOutput VSOut;
    
    VSOut.pos = pos;
    
    VSOut.pos = mul(VSOut.pos, insWorld);
    VSOut.pos = mul(VSOut.pos, view);
    VSOut.pos = mul(VSOut.pos, projection);
    
    VSOut.col = insCol;
    
    VSOut.tex = tex;
    
    return VSOut;
}

