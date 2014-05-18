#include "MT_Skybox_ShaderHeader.hlsli"

//Texture2DArray g_textures;
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
	//for (int i = 0; i < 6; i++)
	//{
	//	float faceIndex = input.TextureCoord.z;
	//	if (faceIndex < 0.5)		// 0
	//	{
	//		float4 color = { 1, 0, 0, 1 };
	//		final_color = color;
	//		break;
	//	}
	//	if (faceIndex > 0.5 && faceIndex < 1.5)		//1
	//	{
	//		float4 color = { 0, 1, 0, 1 };
	//		final_color = color;
	//		break;
	//	}
	//	if (faceIndex > 1.5 && faceIndex < 2.5)		//2
	//	{
	//		float4 color = { 0, 0, 1, 1 };
	//		final_color = color;
	//		break;
	//	}
	//	if (faceIndex > 2.5 && faceIndex < 3.5)		//3
	//	{
	//		float4 color = { 1, 1, 0, 1 };
	//		final_color = color;
	//		break;
	//	}
	//	if (faceIndex > 3.5 && faceIndex < 4.5)		//4
	//	{
	//		float4 color = { 0, 1, 1, 1 };
	//		final_color = color;
	//		break;
	//	}
	//	if (faceIndex > 4.5)						//5
	//	{
	//		float4 color = { 1, 0, 1, 1 };
	//		final_color = color;
	//		break;
	//	}
	//}
	return final_color;
}