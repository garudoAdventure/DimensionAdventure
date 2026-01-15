cbuffer PixelTime : register(b1)
{
    float time;
    float3 dummy;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float grad(in float2 pos)
{
    float2 p = float2(dot(pos, float2(127.1, 311.7)), dot(pos, float2(269.5, 183.3)));
    return -1.0 + 2.0 * frac(sin(p) * (43758.5453123));
}
    
float perlinNoise(in float2 pos) {
    float2 floorPos = floor(pos);
    float2 fracPos = frac(pos);
    
    float val00 = dot(grad(floorPos + float2(0.0f, 0.0f)), fracPos - float2(0.0f, 0.0f));
    float val01 = dot(grad(floorPos + float2(0.0f, 1.0f)), fracPos - float2(0.0f, 1.0f));
    float val10 = dot(grad(floorPos + float2(1.0f, 0.0f)), fracPos - float2(1.0f, 0.0f));
    float val11 = dot(grad(floorPos + float2(1.0f, 1.0f)), fracPos - float2(1.0f, 1.0f));
    
    fracPos = smoothstep(0.0, 1.0, fracPos);
    
    return lerp(lerp(val00, val01, fracPos.y), lerp(val10, val11, fracPos.y), fracPos.x);
}

float4 main(VSOutput In) : SV_Target0
{
    float col = perlinNoise(float2(In.tex.x * 2 + time, 0.0f));
    return float4(col, col, col, 1.0f);
}