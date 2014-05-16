#include "MT_Skybox_ShaderHeader.hlsli"

cbuffer ConstantBuffer : register(b0)
{
	matrix g_world;
	matrix g_view;
	matrix g_projection;
}

PS_INPUT main(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	output.Position = mul(input.Position, g_world);
	output.Position = mul(output.Position, g_view);
	output.Position = mul(output.Position, g_projection);
	return output;
}