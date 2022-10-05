#include "./Global/PSGlobal.hlsli"

Texture2D g_Texture : register(t0);

struct PS_INPUT
{
    float4 Pos      : SV_Position;
    float4 Color    : COLOR0;
    float2 UV       : TEXCOORD0;
};


float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = g_Texture.Sample(g_LinearWrapState, input.UV);
    color *= input.Color;

    return color;
}