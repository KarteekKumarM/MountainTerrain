struct VS_INPUT
{
	float4 Position: POSITION;
	float3 TextureCoord: TEX_COORD;
};

struct PS_INPUT
{
	float4 Position: SV_POSITION;
	float3 TextureCoord: TEX_COORD;
};