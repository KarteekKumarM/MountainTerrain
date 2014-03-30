#include "Header.hlsli"

cbuffer LightBuffer
{
	float4 g_light_ambientColor;
	float4 g_light_diffuseColor;
	float3 g_light_direction;
	float g_light_enabled;
};

Texture2D g_textures[3];
SamplerState g_Sampler;

float4 main( PS_INPUT input ) : SV_Target
{
	const float3 upDir = { 0.0f, 1.0f, 0.0f };

	// TO DO : Fix, so texture does not stretch with height
	float2 texCoOrd = { input.TextureCoordinates.x, input.TextureCoordinates.z };

	// calculate texture color
	float4 textureColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	float slopeFactor = abs(dot(input.Normal, upDir));
	textureColor = (slopeFactor*g_textures[0].Sample(g_Sampler, texCoOrd)) + ((1 - slopeFactor)*g_textures[1].Sample(g_Sampler, texCoOrd));
	if (input.TextureCoordinates.y <= 1.3)
	{
		textureColor = g_textures[2].Sample(g_Sampler, texCoOrd);
	}
	else if (input.TextureCoordinates.y <= 1.5)
	{
		textureColor = (slopeFactor*g_textures[2].Sample(g_Sampler, texCoOrd)) + ((1 - slopeFactor)*g_textures[1].Sample(g_Sampler, texCoOrd));
	}

	float4 final_color;

	if (g_light_enabled == 1.0)
	{
		// ambient
		final_color = g_light_ambientColor;

		// diffuse
		float lightIntensity = dot(input.Normal, g_light_direction);
		if (lightIntensity > 0.0f) {
			final_color += g_light_diffuseColor * textureColor * lightIntensity;
		}
	}
	else
	{
		final_color = textureColor;
	}


	return saturate(final_color);
}