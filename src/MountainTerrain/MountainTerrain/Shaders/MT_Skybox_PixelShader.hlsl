#include "MT_Skybox_ShaderHeader.hlsli"

Texture2D g_textures[6];
SamplerState g_Sampler;

float4 main(PS_INPUT input) : SV_Target
{
	float4 final_color = { 1, 1, 1, 1 };
	for (int i = 0; i < 6; i++)
	{
		float faceIndex = input.TextureCoord.z;
		float diff = abs(faceIndex - float(i));
		
		if (diff < 0.2)
		{
			final_color = g_textures[i].Sample(g_Sampler, input.TextureCoord);
			break;
		}
	}
	return final_color;
}