#include "stdafx.h"
#include <Graphics/D3DDevice.h>
#include <Graphics/D3DUtility.h>
#include <Utility/Assert2.h>

#include <Utility/Window.h>

#include <d3d11.h>


void createDepthStencil(ID3D11Device& device, unsigned int windowWidth, unsigned int windowHeight,
                        ID3D11Texture2D*& depthTexture, ID3D11DepthStencilView*& depthStencilView)
{
  D3D11_TEXTURE2D_DESC depthTextureDesc = {};
  depthTextureDesc.Width = windowWidth;
  depthTextureDesc.Height = windowHeight;
  depthTextureDesc.MipLevels = 1;
  depthTextureDesc.ArraySize = 1;
  depthTextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
  depthTextureDesc.SampleDesc.Count = 1;
  depthTextureDesc.SampleDesc.Quality = 0;
  depthTextureDesc.Usage = D3D11_USAGE_DEFAULT;
  depthTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthTextureDesc.CPUAccessFlags = 0;
  depthTextureDesc.MiscFlags = 0;
  HRESULT hr = device.CreateTexture2D(&depthTextureDesc, nullptr, &depthTexture);
  ASSERT2(SUCCEEDED(hr), "Failed to create depth texture.");

  D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
  dsvDesc.Format = depthTextureDesc.Format;
  dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
  dsvDesc.Texture2D.MipSlice = 0;
  hr = device.CreateDepthStencilView(depthTexture, &dsvDesc, &depthStencilView);
  ASSERT2(SUCCEEDED(hr), "Failed to create depth stencil view.");
}

HRESULT initDevice(const Window& window, unsigned int windowWidth, unsigned int windowHeight,
                   D3D11Device& deviceOut)
{
  HRESULT hr = S_OK;

  unsigned int createDeviceFlags = 0;
#ifdef _DEBUG
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  DXGI_SWAP_CHAIN_DESC sd = { 0 };
  sd.BufferCount = 1;
  sd.BufferDesc.Width = windowWidth;
  sd.BufferDesc.Height = windowHeight;
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
  sd.BufferDesc.RefreshRate.Numerator = 60;
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  sd.OutputWindow = window.hWnd;
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = true;

  D3D_FEATURE_LEVEL featureLevels[] = 
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
  hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags,
    featureLevels, 3, D3D11_SDK_VERSION, &sd, &deviceOut.swapChain, &deviceOut.device, &featureLevel, &deviceOut.deviceContext);
  ASSERT2(featureLevel == D3D_FEATURE_LEVEL_11_0, "Failed to initialize Direct3D with feature level 11.");

  if (FAILED(hr))
  {
    return hr;
  }

  // Create a render target view
  ID3D11Texture2D* backBuffer = nullptr;
  hr = deviceOut.swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
  if (FAILED(hr))
  {
    return hr;
  }
  setDebugName(*backBuffer, "Back Buffer");

  hr = deviceOut.device->CreateRenderTargetView(backBuffer, nullptr, &deviceOut.renderTargetView);
  backBuffer->Release();
  if (FAILED(hr))
  {
    return hr;
  }

  createDepthStencil(*deviceOut.device, windowWidth, windowHeight, deviceOut.depthTexture, deviceOut.depthStencilView);

  deviceOut.deviceContext->OMSetRenderTargets(1, &deviceOut.renderTargetView, deviceOut.depthStencilView);

  // Setup the viewport
  D3D11_VIEWPORT vp;
  vp.Width = (float)windowWidth;
  vp.Height = (float)windowHeight;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;
  deviceOut.deviceContext->RSSetViewports(1, &vp);

  IDXGIDevice* dxgiDevice;
  hr = deviceOut.device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);

  IDXGIAdapter* dxgiAdapter;
  hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);

  IDXGIFactory* dxgiFactory;
  dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);

  //Disable DXGI's listening for Alt-Enter.
  dxgiFactory->MakeWindowAssociation(window.hWnd, DXGI_MWA_NO_ALT_ENTER);

  setDebugName(*deviceOut.device, "D3D11 Device");
  setDebugName(*deviceOut.deviceContext, "Immediate Context");
  setDebugName(*deviceOut.renderTargetView, "RTV Back Buffer");
  setDebugName(*deviceOut.swapChain, "Swap Chain");
  setDebugName(*deviceOut.depthTexture, "Depth Texture");
  setDebugName(*deviceOut.depthStencilView, "Depth Stencil View");
  setDebugName(*dxgiDevice, "DXGI Device");
  setDebugName(*dxgiAdapter, "DXGI Adapter");
  setDebugName(*dxgiFactory, "DXGI Factory");

  dxgiFactory->Release();
  dxgiAdapter->Release();
  dxgiDevice->Release();

  return S_OK;
}

D3D11Device::~D3D11Device()
{
  if (deviceContext != nullptr)
  {
    deviceContext->ClearState();
  }
  if (renderTargetView != nullptr)
  {
    renderTargetView->Release();
  }
  if (depthStencilView != nullptr)
  {
    depthStencilView->Release();
  }
  if (depthTexture != nullptr)
  {
    depthTexture->Release();
  }
  if (swapChain != nullptr)
  {
    swapChain->Release();
  }
  if (deviceContext != nullptr)
  {
    deviceContext->Release();
  }
  if (device != nullptr)
  {
    device->Release();
  }
}
