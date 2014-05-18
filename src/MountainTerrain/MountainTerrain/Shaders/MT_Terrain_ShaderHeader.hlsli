
struct VS_INPUT
{
	float4 Position: POSITION; 
	float3 Normal : NORMAL;
};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
	float3 Normal : NORMAL;
	float4 LocalPosition : LOC_POSITION;
};