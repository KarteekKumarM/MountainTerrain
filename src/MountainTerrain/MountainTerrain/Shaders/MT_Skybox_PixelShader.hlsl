#include "MT_Skybox_ShaderHeader.hlsli"

//Texture2DArray g_textures;
Texture2D g_textures[6];
SamplerState g_Sampler;

float4 main(PS_INPUT input) : SV_Target
{
	float4 final_color = { 1, 1, 1, 1 };
	for (int i = 0; i < 6; i++)
	{
		int faceIndex = int(input.TextureCoord.z);
		if (i == faceIndex)
		{
			final_color = g_textures[i].Sample(g_Sampler, input.TextureCoord);
		}
	}
	return final_color;
}