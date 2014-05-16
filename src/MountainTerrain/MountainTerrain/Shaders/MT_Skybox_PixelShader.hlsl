#include "MT_Skybox_ShaderHeader.hlsli"

float4 main(PS_INPUT input) : SV_Target
{
	float4 final_color = { 1, 1, 1, 1 };
	return final_color;
}