#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;


struct Camera
{
  Camera(float fieldOfViewDegrees = 60.0f, float aspectRatio = 16.0f / 9.0f,
    float nearClip = 0.1f, float farClip = 1000.0f);

  void setProperties(float fieldOfViewDegrees, float aspectRatio, float nearClip, float farClip);


  DirectX::XMMATRIX projection;
  DirectX::BoundingFrustum frustum;
};


void setCameraConstantBuffer(ID3D11Buffer& constantBuffer, ID3D11DeviceContext& deviceContext);

void updateCameraConstantBuffer(const Camera& camera, ID3D11Buffer& constantBuffer,
                                ID3D11DeviceContext& deviceContext);
