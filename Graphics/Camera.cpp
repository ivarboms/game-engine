#include "stdafx.h"
#include "Camera.h"
#include "ConstantBuffers.h"

#include <d3d11.h>

using namespace DirectX;


Camera::Camera(float fieldOfViewDegrees /*= 60.0f*/, float aspectRatio /*= 16.0f / 9.0f*/,
               float nearClip /*= 0.1f*/, float farClip /*= 1000.0f*/)
{
  setProperties(fieldOfViewDegrees, aspectRatio, nearClip, farClip);
}

void Camera::setProperties(float fieldOfViewDegrees, float aspectRatio, float nearClip, float farClip)
{
  const float fieldOfViewRadians = XMConvertToRadians(fieldOfViewDegrees);
  projection = XMMatrixPerspectiveFovLH(fieldOfViewRadians, aspectRatio, nearClip, farClip);
  BoundingFrustum::CreateFromMatrix(frustum, projection);
}

XMMATRIX calculateViewMatrix(FXMVECTOR cameraPosition, FXMVECTOR cameraRotation)
{
  //Calculate the camera's up and forward axes.
  //g_XMIdentityR1 is up in world space, g_XMIdentityR2 is forward.
  const XMVECTOR upCameraSpace = XMVector3Rotate(g_XMIdentityR1, cameraRotation);
  const XMVECTOR forwardCameraSpace = XMVector3Rotate(g_XMIdentityR2, cameraRotation);

  //The look-at target is just in front of the camera in camera space.
  const XMVECTOR lookAtTarget = cameraPosition + forwardCameraSpace;

  //return XMMatrixLookAtLH(lookAtTarget, cameraPosition, upCameraSpace);
  //return XMMatrixLookAtLH(cameraPosition, lookAtTarget, upCameraSpace);
  XMMATRIX m = XMMatrixAffineTransformation(XMVectorReplicate(1.0f), cameraPosition, cameraRotation, XMVectorNegate(cameraPosition));

  //printf("X: %.2f, Y: %.2f, Z: %.2f, W: %.2f\n", cameraRotation.m128_f32[0], cameraRotation.m128_f32[1], cameraRotation.m128_f32[2], cameraRotation.m128_f32[3]);
  return m;
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
