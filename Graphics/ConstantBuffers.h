#pragma once

#include <Utility/Assert2.h>

#include <DirectXCollision.h>


/*  Slots used when calling ID3D11DeviceContext::VSSetConstantBuffers, PSSetConstantBuffers, etc.
*/
enum class ConstantBufferSlot
{
  CameraPS = 0,
  CameraVS = 0,
};

struct CameraConstantBuffer
{
  DirectX::XMMATRIX	view;
  DirectX::XMMATRIX	projection;
  DirectX::XMMATRIX	viewProjection;
  DirectX::XMMATRIX	inverseViewProjection;
  DirectX::XMVECTOR	cameraPosition;//TODO: figure out if redundant, can get this from view matrix.
};


template<typename BufferType>
ID3D11Buffer* createConstantBuffer(ID3D11Device& device)
{
  D3D11_BUFFER_DESC bufferDesc = {};
  bufferDesc.Usage = D3D11_USAGE_DEFAULT;
  bufferDesc.ByteWidth = sizeof(BufferType);
  bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  bufferDesc.CPUAccessFlags = 0;

  ID3D11Buffer* constantBuffer = nullptr;
  HRESULT hr = device.CreateBuffer(&bufferDesc, nullptr, &constantBuffer);
  ASSERT2(SUCCEEDED(hr), "Failed to create constant buffer");

  return constantBuffer;
}
