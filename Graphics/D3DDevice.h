#pragma once

#include <windows.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct Window;


struct D3D11Device
{
  ID3D11Device* device;
  ID3D11DeviceContext* deviceContext;
  IDXGISwapChain* swapChain;
  ID3D11RenderTargetView* renderTargetView;

  ID3D11Texture2D* depthTexture;
  ID3D11DepthStencilView* depthStencilView;

  ~D3D11Device();
};

HRESULT initDevice(const Window& window, unsigned int windowWidth, unsigned int windowHeight,
                   D3D11Device& device);
