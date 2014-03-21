#include "Header.hlsli"

Texture2D g_Texture;
SamplerState g_Sampler;


float4 main( PS_INPUT input ) : SV_Target
{
	// hard-coding here for now
	float4 ambientColor = { 0.05f, 0.05f, 0.05f, 1.0f };
	float3 lightDir = { -0.03f, -0.03f, 0.75f };

	float2 texCoOrd = { 0.0f, 0.0f };

	// TO DO : Fix, so texture does not stretch with height
	texCoOrd[0] = (input.TextureCoordinates.x);
	texCoOrd[1] = (input.TextureCoordinates.z);

	float4 textureColor = g_Texture.Sample(g_Sampler, texCoOrd);

	// ambient
	float4 color  = ambientColor;

	// diffuse
	lightDir = normalize(-lightDir);
	float lightIntensity = dot(input.Normal, lightDir);
	if(lightIntensity > 0.0f ) {
		color += textureColor * lightIntensity;
	}

    return saturate(color);
}