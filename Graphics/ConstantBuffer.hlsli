
cbuffer CameraConstantBuffer : register(b0)
{
  float4x4 view;
  float4x4 projection;
  float4x4 viewProjection;
  float4x4 inverseViewProjection;
  float4 cameraPosition;
};
