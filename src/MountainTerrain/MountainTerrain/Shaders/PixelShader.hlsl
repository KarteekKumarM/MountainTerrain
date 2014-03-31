#include "Header.hlsli"

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

Texture2D g_textures[3];
SamplerState g_Sampler;

float4 main( PS_INPUT input ) : SV_Target
{
	const float3 upDir = { 0.0f, 1.0f, 0.0f };

	float4 material_color = { 1.0f, 1.0f, 1.0f, 1.0f };
	// ------------------------------------------------------------------------------ <TEXTURING>
	if (g_texture_enabled)
	{
		// TO DO : Fix, so texture does not stretch with height
		float2 texCoOrd = { input.TextureCoordinates.x, input.TextureCoordinates.z };
		// calculate texture color
		float slopeFactor = abs(dot(input.Normal, upDir));
		material_color = (slopeFactor*g_textures[0].Sample(g_Sampler, texCoOrd)) + ((1 - slopeFactor)*g_textures[1].Sample(g_Sampler, texCoOrd));
		if (input.TextureCoordinates.y <= 1.3)
		{
			material_color = g_textures[2].Sample(g_Sampler, texCoOrd);
		}
		else if (input.TextureCoordinates.y <= 1.5)
		{
			material_color = (slopeFactor*g_textures[2].Sample(g_Sampler, texCoOrd)) + ((1 - slopeFactor)*g_textures[1].Sample(g_Sampler, texCoOrd));
		}
	}
	// ----------------------------------------------------------------------------- </TEXTURING>

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