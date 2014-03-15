cbuffer ConstantBuffer : register( b0 )
{
    matrix World;
    matrix View;
    matrix Projection;
}

struct VS_INPUT
{
	float4 Position: POSITION; 
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
};