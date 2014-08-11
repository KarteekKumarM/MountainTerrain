#include "MT_Terrain_ShaderHeader.hlsli"

cbuffer LightBuffer
{
	float4 g_light_ambientColor;
	float4 g_light_diffuseColor;
	float3 g_light_direction;
	bool g_light_enabled;
	bool g_texture_enabled;
	
	// extra unused variables - size must be mulitple of 16
	bool extra, extraa, extraaa;
};

Texture2D g_textures[3];	// {grass, dirt, water}
SamplerState g_Sampler;

float4 triplanarblending( float3 blending, int textureindex, float4 position )
{
	float scale = 1;

	float2 textCoord_xz = { position.x, position.z };
	float4 material_color_xz = g_textures[textureindex].Sample(g_Sampler, textCoord_xz  * scale);

	float2 texCoOrd_yz = { position.y, position.z };
	float4 material_color_yz = g_textures[textureindex].Sample(g_Sampler, texCoOrd_yz * scale);

	float2 texCoOrd_xy = { position.x, position.y };
	float4 material_color_xy = g_textures[textureindex].Sample(g_Sampler, texCoOrd_xy * scale);

	float4 material_color = (material_color_xz * blending.y) + (material_color_yz * blending.x) + (material_color_xy * blending.z);

	return material_color;
}

float4 main( PS_INPUT input ) : SV_Target
{
	const float3 upDir = { 0.0f, 1.0f, 0.0f };

	float4 material_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ------------------------------------------------------------------------------ <TRI-PLANAR TEXTURE MAPPING>
	float3 blending = abs(input.Normal);
	blending = normalize(max(blending, 0.00001));
	float b = blending.x + blending.y + blending.z;
	blending = blending / b;
	if (g_texture_enabled)
	{
		// calculate texture color
		float grassWaterLerpStart = 1.3;
		float grassWaterLerpEnd = 1.9;
		if (input.LocalPosition.y <= grassWaterLerpStart)
		{
			material_color = triplanarblending(blending, 2, input.LocalPosition);
		}
		else if (input.LocalPosition.y <= grassWaterLerpEnd)
		{
			// lerp water to grass
			float maxDiff = grassWaterLerpEnd - grassWaterLerpStart;
			float actualDiff = grassWaterLerpEnd - input.LocalPosition.y;
			float frac = actualDiff / maxDiff;
			material_color = (frac * triplanarblending(blending, 2, input.LocalPosition)) + ((1 - frac) * triplanarblending(blending, 0, input.LocalPosition));
		}
		else
		{
			float slopeFactor = abs(dot(input.Normal, upDir));
			material_color = (slopeFactor*triplanarblending(blending, 0, input.LocalPosition)) + ((1 - slopeFactor)*triplanarblending(blending, 1, input.LocalPosition));
		}
	}
	// ----------------------------------------------------------------------------- </TRI-PLANAR TEXTURE MAPPING>

	float4 final_color = material_color;
	// ------------------------------------------------------------------------------ <LIGHTING>
	if (g_light_enabled)
	{
		// ambient
		final_color = g_light_ambientColor;

		// diffuse
		float lightIntensity = dot(input.Normal, g_light_direction);
		if (lightIntensity > 0.0f) 
		{
			final_color += g_light_diffuseColor * material_color * lightIntensity;
		}
	}
	// ----------------------------------------------------------------------------- </LIGHTING>

	return saturate(final_color);
}