struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
};

float4 main(VSOutput In) : SV_Target0
{
    return In.col;
}