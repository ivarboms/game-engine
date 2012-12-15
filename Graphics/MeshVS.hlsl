
#include "ConstantBuffer.hlsli"

struct Output
{
  float4 positionOut : SV_POSITION;
  float4 normalOut : NORMAL;
};

Output main(float3 pos : POSITION, float3 normal : NORMAL)// : SV_POSITION
//float4 main(float3 pos : POSITION, float3 normal : NORMAL) : SV_POSITION
{
  Output output;
  output.positionOut = mul(float4(pos, 1.0f), viewProjection);
  //output.positionOut = float4(pos, 1.0f);

  output.normalOut = float4(normal, 0.0f);

  //return output.positionOut;
  return output;




  pos.z += 0.8f;
//	return pos;
}