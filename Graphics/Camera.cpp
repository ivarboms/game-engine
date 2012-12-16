#include "stdafx.h"
#include "Camera.h"
#include "ConstantBuffers.h"

#include <d3d11.h>

using namespace DirectX;


Camera::Camera(float fieldOfViewDegrees /*= 60.0f*/, float aspectRatio /*= 16.0f / 9.0f*/,
               float nearClip /*= 0.1f*/, float farClip /*= 1000.0f*/)
{
  computeFrustum(fieldOfViewDegrees, aspectRatio, nearClip, farClip);
}

void Camera::computeFrustum(float fieldOfViewDegrees, float aspectRatio, float nearClip, float farClip)
{
  const float fieldOfViewRadians = XMConvertToRadians(fieldOfViewDegrees);
  projection = XMMatrixPerspectiveFovLH(fieldOfViewRadians, aspectRatio, nearClip, farClip);
  BoundingFrustum::CreateFromMatrix(frustum, projection);
}

XMMATRIX calculateViewMatrix(FXMVECTOR cameraPosition, FXMVECTOR cameraRotation)
{
  //Compute the camera's world matrix, and then invert it, resulting in the view matrix.
  const XMMATRIX world = XMMatrixAffineTransformation(XMVectorReplicate(1.0f), XMVectorZero(),
    cameraRotation, cameraPosition);
  return XMMatrixInverse(nullptr, world);
}

void setCameraConstantBuffer(ID3D11Buffer& constantBuffer, ID3D11DeviceContext& deviceContext)
{
  ID3D11Buffer* bufferArray[] = { &constantBuffer };
  deviceContext.PSSetConstantBuffers((unsigned int)ConstantBufferSlot::CameraPS, 1, bufferArray);
  deviceContext.VSSetConstantBuffers((unsigned int)ConstantBufferSlot::CameraVS, 1, bufferArray);
}

void updateCameraConstantBuffer(const Camera& camera, ID3D11Buffer& constantBuffer,
                                ID3D11DeviceContext& deviceContext)
{
  const XMVECTOR cameraPosition = XMLoadFloat3(&camera.frustum.Origin);
  const XMVECTOR cameraRotation = XMLoadFloat4(&camera.frustum.Orientation);

  //Fill in the contents of the constant buffer.
  CameraConstantBuffer content;
  content.view = calculateViewMatrix(cameraPosition, cameraRotation);
  content.projection = camera.projection;
  content.viewProjection = content.view * content.projection;
  content.inverseViewProjection = XMMatrixInverse(nullptr, content.viewProjection);
  content.cameraPosition = cameraPosition;

  //The matrices need to be transposed to match the HLSL matrix layout.

  content.view = XMMatrixTranspose(content.view);
  content.projection = XMMatrixTranspose(content.projection);
  content.viewProjection = XMMatrixTranspose(content.viewProjection);
  content.inverseViewProjection = XMMatrixTranspose(content.inverseViewProjection);

  deviceContext.UpdateSubresource(&constantBuffer, 0, nullptr, &content, 0, 0);
}
