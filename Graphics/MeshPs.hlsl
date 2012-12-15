
#include "ConstantBuffer.hlsli"
/*
struct Input
{
  float4 position : POSITION;
  float4 normal : NORMAL;
}

*/

float4 main(float4 Pos : SV_POSITION, float4 normal : NORMAL) : SV_TARGET
//float4 main(float4 Pos : SV_POSITION) : SV_TARGET
{
  float3 cam = cameraPosition.xyz;
  float3 lightVector = cam - Pos.xyz;
  lightVector = float3(-0.3f, 0.7f, -0.3f);
  lightVector = normalize(lightVector);

  float nDotL = max(0.0f, dot(normal.xyz, lightVector));

  float4 color = float4(1.0f, 1.0f, 0.0f, 1.0f);
  return nDotL * color;
	return float4(1.0f, 0.0f, 1.0f, 1.0f);
}