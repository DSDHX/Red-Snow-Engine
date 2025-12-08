cbuffer TransformBuffer : register(b0)
{
    matrix World;
    matrix WVP;
    matrix LightViewProj;
    float3 ViewPosition;
    float Padding0;
}

cbuffer MaterialBuffer : register(b1)
{
    float3 LightDirection;
    float Padding1;
    float4 LightAmbient;
    float4 LightDiffuse;
    float4 LightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
    float4 MaterialEmissive;
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float MaterialShininess;
    float3 Padding2;
}

cbuffer HalftoneParams : register(b3)
{
    float2 ViewportSize;
    float DotDensity;
    float Power;
    float4 ShadowColor;
    float4 LightColor;
}

Texture2D DiffuseMap : register(t0);
Texture2D ShadowMap : register(t1);

SamplerState TextureSampler : register(s0);
SamplerState ShadowSampler : register(s1);

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position : SV_Position;
    float3 WorldPos : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
    float4 LightNDCPos : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    output.Position = mul(float4(input.Position, 1.0f), WVP);
    output.WorldPos = mul(float4(input.Position, 1.0f), World).xyz;
    output.Normal = mul(input.Normal, (float3x3) World);
    output.TexCoord = input.TexCoord;
    
    output.LightNDCPos = mul(float4(input.Position, 1.0f), mul(World, LightViewProj));
    
    return output;
}

float GetShadow(float4 lightPos)
{
    float2 shadowUV = 0.5f * lightPos.xy / lightPos.w + float2(0.5f, 0.5f);
    shadowUV.y = 1.0f - shadowUV.y;
    
    float currentDepth = lightPos.z / lightPos.w;
    
    if (currentDepth > 1.0f)
    {
        return 1.0f;
    }
    
    float storedDepth = ShadowMap.Sample(ShadowSampler, shadowUV).r;
    
    float bias = 0.001f;
    
    return (currentDepth - bias > storedDepth) ? 0.0f : 1.0f;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 N = normalize(input.Normal);
    float3 L = normalize(-LightDirection);
    
    float NdotL = max(dot(N, L), 0.0f);
    float shadow = GetShadow(input.LightNDCPos);
    
    float intensity = NdotL * shadow;
    
    intensity = saturate(intensity + 0.1f);
    
    float2 st = input.Position.xy;
    
    float2x2 rotation = float2x2(0.707, -0.707, 0.707, 0.707);
    float2 rotatedST = mul(st, rotation);
    
    float2 nearest = 2.0f * frac(rotatedST * DotDensity / ViewportSize.y) - 1.0f;
    float dist = length(nearest);
    
    float radius = sqrt(1.0f - intensity);
    float3 halftoneColor = (dist > radius) ? LightColor.rgb : ShadowColor.rgb;
    
    float4 textureColor = DiffuseMap.Sample(TextureSampler, input.TexCoord);
    
    return float4(halftoneColor * textureColor.rgb, 1.0f);
}