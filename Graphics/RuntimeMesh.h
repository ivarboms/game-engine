#pragma once

#include <DirectXMath.h>
#include <vector>
#include <string>
#include <memory>

struct ID3D11Buffer;
struct ID3D11Device;
struct Mesh;



struct VertexFormat
{
  enum Enum
  {
    VertexPositions = 1 << 0,
    Normals = 1 << 1,
    Tangents = 1 << 2,
    TextureCoordinates = 1 << 3,
  };
};

struct RuntimeMesh
{
  std::vector<ID3D11Buffer*> vertexBuffers;
  std::vector<ID3D11Buffer*> indexBuffers;
  std::vector<unsigned int> indexCounts;
  std::vector<unsigned int> vertexCounts;

  DirectX::XMFLOAT3A minExtents;
  DirectX::XMFLOAT3A maxExtents;

  //The vertex format of all of the vertex buffers.
  VertexFormat::Enum vertexFormat;

  std::string name;


  RuntimeMesh(){}
  RuntimeMesh(RuntimeMesh&& other);
  ~RuntimeMesh();

  unsigned int getTriangleCount() const;

  unsigned int calculateByteStride() const;
};

std::shared_ptr<RuntimeMesh> createFromMesh(const Mesh& mesh, ID3D11Device& device);
