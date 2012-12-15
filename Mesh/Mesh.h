#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>


struct SubMesh
{
  std::vector<unsigned int> indices;
  std::vector<DirectX::XMFLOAT3> vertices;
  std::vector<DirectX::XMFLOAT3> normals;
  std::vector<DirectX::XMFLOAT3> tangents;
  std::vector<DirectX::XMFLOAT2> textureCoordinates;
  std::string name;
};

struct Mesh
{
  std::vector<SubMesh> subMeshes;
  DirectX::XMFLOAT3 minExtents;
  DirectX::XMFLOAT3 maxExtents;
  std::string name;
};
