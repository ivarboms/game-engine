#include "stdafx.h"
#include <Graphics/Shader.h>
#include <Graphics/D3DUtility.h>
#include <Utility/Assert2.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#include <boost/format.hpp>


std::string toNarrow(const std::wstring& str)
{
  return std::string(str.begin(), str.end());
}


HRESULT compileShader(const std::wstring& fileName, ID3DBlob*& shaderBlobOut,
                      const char* profile, const char* functionName /*= "main"*/)
{
  ID3DBlob* blob = nullptr;
  ID3DBlob* errorBlob = nullptr;
  
  unsigned int shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if _DEBUG
  shaderFlags |= D3DCOMPILE_DEBUG;
  shaderFlags |= D3DCOMPILE_WARNINGS_ARE_ERRORS;
#else
  shaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

  //D3DIncludeHandler includeHandler;
  const HRESULT hr = D3DCompileFromFile(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, functionName, profile,
    shaderFlags, 0, &blob, &errorBlob);

  if (FAILED(hr))
  {
    if (errorBlob != nullptr)
    {
      const char* errorMessage = (const char*)errorBlob->GetBufferPointer();
      std::string fileNameNotWide(fileName.begin(), fileName.end());
      boost::format formatedErrorMessage("Failed to compile shader '%1%': %2%");
      formatedErrorMessage % fileNameNotWide % errorMessage;
      std::string errorMessageCombined(formatedErrorMessage.str());
      
      OutputDebugStringA(errorMessage);
      ASSERT2(false, errorMessageCombined);

      errorBlob->Release();
    }
  }

  shaderBlobOut = blob;
  return hr;
}

std::shared_ptr<VertexShader> createVertexShader(ID3DBlob& vertexShaderBlob, ID3D11Device& device)
{
  std::shared_ptr<VertexShader> vertexShader(std::make_shared<VertexShader>());

  HRESULT hr = device.CreateVertexShader(vertexShaderBlob.GetBufferPointer(), vertexShaderBlob.GetBufferSize(),
    nullptr, &vertexShader->shader);
  ASSERT2(SUCCEEDED(hr), "Failed to create vertex shader.");

  createInputLayoutDescFromVertexShader(vertexShaderBlob, device, vertexShader->inputLayout);

  return vertexShader;
}

/*  This code is taken from
  http://takinginitiative.net/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
*/
void createInputLayoutDescFromVertexShader(ID3DBlob& vertexShaderBlob, ID3D11Device& device,
                                           ID3D11InputLayout*& generatedInputLayout)
{
  ID3D11ShaderReflection* shaderReflection = nullptr;
  HRESULT hr = D3DReflect(vertexShaderBlob.GetBufferPointer(), vertexShaderBlob.GetBufferSize(),
    IID_ID3D11ShaderReflection, (void**)&shaderReflection);
  ASSERT2(SUCCEEDED(hr), "Failed to reflect shader.");

  D3D11_SHADER_DESC shaderDesc;
  hr = shaderReflection->GetDesc(&shaderDesc);
  ASSERT2(SUCCEEDED(hr), "Failed to get shader desc.");

  unsigned int byteOffset = 0;
  std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
  inputLayoutDesc.reserve(shaderDesc.InputParameters);

  //Get input layout from shader desc.
  for (unsigned int i = 0; i < shaderDesc.InputParameters; ++i)
  {
  	D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
    shaderReflection->GetInputParameterDesc(i, &paramDesc);

    D3D11_INPUT_ELEMENT_DESC elementDesc;
    elementDesc.SemanticName = paramDesc.SemanticName;     
    elementDesc.SemanticIndex = paramDesc.SemanticIndex;
    elementDesc.InputSlot = 0;
    elementDesc.AlignedByteOffset = byteOffset; //  D3D11_APPEND_ALIGNED_ELEMENT
    elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    elementDesc.InstanceDataStepRate = 0;

    // determine DXGI format
    if (paramDesc.Mask == 1)
    {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
      byteOffset += 4;
    }
    else if (paramDesc.Mask <= 3)
    {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
      byteOffset += 8;
    }
    else if (paramDesc.Mask <= 7)
    {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
      byteOffset += 12;
    }
    else if (paramDesc.Mask <= 15)
    {
      if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
      else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      byteOffset += 16;
    }

    //save element desc
    inputLayoutDesc.push_back(elementDesc);
  }

  hr = device.CreateInputLayout(inputLayoutDesc.data(), inputLayoutDesc.size(),
    vertexShaderBlob.GetBufferPointer(), vertexShaderBlob.GetBufferSize(), &generatedInputLayout);
  ASSERT2(SUCCEEDED(hr), "Failed to create input layout.");

  //Free allocation shader reflection memory
  shaderReflection->Release();
}

std::shared_ptr<PixelShader> createPixelShader(ID3DBlob& pixelShaderBlob, ID3D11Device& device)
{
  std::shared_ptr<PixelShader> pixelShader(std::make_shared<PixelShader>());

  HRESULT hr = device.CreatePixelShader(pixelShaderBlob.GetBufferPointer(), pixelShaderBlob.GetBufferSize(),
    nullptr, &pixelShader->shader);
  ASSERT2(SUCCEEDED(hr), "Failed to create pixel shader");

  return pixelShader;
}

std::shared_ptr<VertexShader> compileAndCreateVertexShader(const wchar_t* fileName, ID3D11Device& device,
                                                           const char* functionName /*= "main"*/)
{
  ID3DBlob* shaderBlob = nullptr;
  compileShader(fileName, shaderBlob, "vs_5_0", functionName);
  std::shared_ptr<VertexShader> vs(createVertexShader(*shaderBlob, device));
  setDebugName(*vs->shader, "VS " + toNarrow(fileName));
  setDebugName(*vs->inputLayout, "IL " + toNarrow(fileName));
  return vs;
}

std::shared_ptr<PixelShader> compileAndCreatePixelShader(const wchar_t* fileName, ID3D11Device& device,
                                                         const char* functionName /*= "main"*/)
{
  ID3DBlob* shaderBlob = nullptr;
  compileShader(fileName, shaderBlob, "ps_5_0", functionName);
  std::shared_ptr<PixelShader> ps(createPixelShader(*shaderBlob, device));
  setDebugName(*ps->shader, "PS " + toNarrow(fileName));
  return ps;
}
