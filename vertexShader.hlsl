cbuffer Mat : register(b0) 
{
    float4x4 world;
    float4x4 view;
    float4x4 projection;
}

cbuffer Bone : register(b1)
{
    float4x4 boneMatrix[50];
}

cbuffer Light : register(b2)
{
    bool lightEnable;
    float3 lightDirection;
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
    int4 boneIdx: BONE_IDX,
    float4 boneWeight : BONE_WEIGHT
) {
    VSOutput VSOut;
    
    VSOut.pos = pos;
    
    if (boneWeight[0] != 0.0f)
    {
        float4x4 boneTransform = mul(boneMatrix[boneIdx[0]], boneWeight[0]);
        boneTransform += mul(boneMatrix[boneIdx[1]], boneWeight[1]);
        boneTransform += mul(boneMatrix[boneIdx[2]], boneWeight[2]);
        boneTransform += mul(boneMatrix[boneIdx[3]], boneWeight[3]);
        VSOut.pos = mul(VSOut.pos, boneTransform);
    }
    
    VSOut.pos = mul(VSOut.pos, world);
    VSOut.pos = mul(VSOut.pos, view);
    VSOut.pos = mul(VSOut.pos, projection);
    
    VSOut.col = color;
    
    if (lightEnable)
    {
        float3 nor = mul(float4(normal, 0.0f), world);
        nor = normalize(nor);
        VSOut.col.rgb = saturate(-dot(lightDirection, nor)) * color;
        VSOut.col.a = 1.0f;
        
        VSOut.col.rgb += float3(0.4f, 0.4f, 0.4f);
    }
    
    VSOut.tex = tex;
    
    return VSOut;
}

