Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

cbuffer ColorGrading : register(b2)
{
    float3 whiteBalance;
    float saturation;
    float contrast;
    float3 dummy;
};

struct VSOutput
{
    float4 pos : SV_Position;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float3 rgb2lms(float3 c)
{
    float3x3 LIN_2_LMS = float3x3(
        0.390405, 0.549941, 0.00892632,
        0.0708416, 0.963172, 0.00135775,
        0.0231082, 0.128021, 0.936245
    );
    return mul(LIN_2_LMS, c);
}

float3 lms2rgb(float3 c)
{
    float3x3 LMS_2_LIN = float3x3(
        2.85847, -1.62879, -0.0248910,
        -0.210182, 1.15820, 0.000324281,
        -0.0418120, -0.118169, 1.06867
    );
    return mul(LMS_2_LIN, c);
}

float4 main(VSOutput In) : SV_Target0
{
    float4 color = g_texture.Sample(g_sampler, In.tex);
    
    // White balance
    float3 lms = rgb2lms(color.rgb);
    lms *= whiteBalance;
    color.rgb = lms2rgb(lms);
    
    // Contrast
    color.rgb = (color.rgb - 0.5) * contrast + 0.5;
    
    // Saturation
    float luminance = dot(color.rgb, float3(0.2126, 0.7152, 0.0722));
    color.rgb = lerp(luminance.xxx, color.rgb, saturation);
    
    return float4(color.rgb, 1.0);
}