#pragma once

#include <string>
#include <memory>

#include <D3Dcommon.h>
#include <d3d11.h>


struct VertexShader
{
  VertexShader()
    : shader(nullptr)
    , inputLayout(nullptr)
  {
  }

  ~VertexShader()
  {
    if (shader != nullptr)
    {
      shader->Release();
    }
    if (inputLayout != nullptr)
    {
      inputLayout->Release();
    }
  }

  ID3D11VertexShader* shader;
  ID3D11InputLayout* inputLayout;
};

struct PixelShader
{
  PixelShader()
    : shader(nullptr)
  {
  }

  ~PixelShader()
  {
    if (shader != nullptr)
    {
      shader->Release();
    }
  }

  ID3D11PixelShader* shader;
};

HRESULT compileShader(const std::wstring& fileName, ID3DBlob*& shaderBlobOut,
                      const char* profile, const char* functionName = "main");

std::shared_ptr<VertexShader> createVertexShader(ID3DBlob& vertexShaderBlob, ID3D11Device& device);

void createInputLayoutDescFromVertexShader(ID3DBlob& vertexShaderBlob, ID3D11Device& device,
                                           ID3D11InputLayout*& generatedInputLayout);

std::shared_ptr<PixelShader> createPixelShader(ID3DBlob& pixelShaderBlob, ID3D11Device& device);

std::shared_ptr<VertexShader> compileAndCreateVertexShader(const wchar_t* fileName,
                                                           ID3D11Device& device,
                                                           const char* functionName = "main");

std::shared_ptr<PixelShader> compileAndCreatePixelShader(const wchar_t* fileName,
                                                         ID3D11Device& device,
                                                         const char* functionName = "main");