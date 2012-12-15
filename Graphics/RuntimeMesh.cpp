#include "stdafx.h"
#include "RuntimeMesh.h"
#include "Mesh.h"
#include "D3DUtility.h"
#include "Assert2.h"

#include <d3d11.h>

#include <memory>
#include <boost/format.hpp>

using namespace DirectX;


RuntimeMesh::~RuntimeMesh()
{
  for (unsigned int i = 0; i < vertexBuffers.size(); ++i)
  {
  	vertexBuffers[i]->Release();
  }

  for (unsigned int i = 0; i < indexBuffers.size(); ++i)
  {
  	indexBuffers[i]->Release();
  }
}

RuntimeMesh::RuntimeMesh(RuntimeMesh&& other)
  : vertexBuffers(std::move(other.vertexBuffers))
  , indexBuffers(std::move(other.indexBuffers))
  , indexCounts(std::move(other.indexCounts))
  , vertexCounts(std::move(other.vertexCounts))
  , minExtents(other.minExtents)
  , maxExtents(other.maxExtents)
  , vertexFormat(other.vertexFormat)
  , name(std::move(other.name))
{
}

unsigned int RuntimeMesh::getTriangleCount() const
{
  unsigned int totalTriangles = 0;
  for (unsigned int i = 0; i < vertexCounts.size(); ++i)
  {
    totalTriangles += vertexCounts[i];
  }
  return totalTriangles;
}

unsigned int RuntimeMesh::calculateByteStride() const
{
  unsigned int stride = 0;
  stride += (vertexFormat & VertexFormat::VertexPositions) ? 12 : 0;
  stride += (vertexFormat & VertexFormat::Normals) ? 12 : 0;
  stride += (vertexFormat & VertexFormat::Tangents) ? 12 : 0;
  stride += (vertexFormat & VertexFormat::TextureCoordinates) ? 8 : 0;
  return stride;
}


template<typename T>
unsigned int byteSize(const std::vector<T>& vec)
{
  return sizeof(T) * vec.size();
}

unsigned int calculateByteSize(const SubMesh& subMesh)
{
  unsigned int byteWidth = 0;
  byteWidth += byteSize(subMesh.vertices);
  byteWidth += byteSize(subMesh.normals);
  byteWidth += byteSize(subMesh.tangents);
  byteWidth += byteSize(subMesh.textureCoordinates);
  return byteWidth;
}

/*  Copies source into destination (modified by headOffset), and modifies headOffset so that
    destination+headOffset points to just after the memory which was copied to destination.
*/
template<typename T>
void copyToBufferAndMoveHead(unsigned char* destination, unsigned int& headOffset, const T& source)
{
  memcpy(destination + headOffset, &source, sizeof(T));
  headOffset += sizeof(T);
}

/*  Combines the buffers in the mesh into a single buffer.
    If the mesh has vertex positions, normals and texture coordinates, the memory layout in the
    buffer will be (vertex, normal, texture coordinate) repeated (vertex count) times.
*/
std::vector<unsigned char> combineBuffers(const SubMesh& subMesh)
{
  const unsigned int combinedBufferByteSize = calculateByteSize(subMesh);
  //std::unique_ptr<unsigned char*> combinedBuffers((unsigned char*)malloc(combinedBufferByteSize));
  std::vector<unsigned char> combinedBuffers2(combinedBufferByteSize);

  const unsigned int vertexCount = subMesh.vertices.size();
  const bool hasNormals = !subMesh.normals.empty();
  const bool hasTangents = !subMesh.tangents.empty();
  const bool hasTextureCoordinates = !subMesh.textureCoordinates.empty();

  ASSERT2(vertexCount == subMesh.normals.size() || !hasNormals, "Invalid mesh; all vertex/normal/tanget"
    " etc buffers need to have the same size per submesh.");
  ASSERT2(vertexCount == subMesh.tangents.size() || !hasTangents, "Invalid mesh; all vertex/normal/tanget"
    " etc buffers need to have the same size per submesh.");
  ASSERT2(vertexCount == subMesh.textureCoordinates.size() || !hasTextureCoordinates,
    "Invalid mesh; all vertex/normal/tanget etc buffers need to have the same size per submesh.");

  unsigned int headOffset = 0;
  for (unsigned int i = 0; i < vertexCount; ++i)
  {
    copyToBufferAndMoveHead(combinedBuffers2.data(), headOffset, subMesh.vertices[i]);

    //copyToBufferAndMoveHead(*combinedBuffers, headOffset, subMesh.vertices[i]);
    if (hasNormals)
    {
      copyToBufferAndMoveHead(combinedBuffers2.data(), headOffset, subMesh.normals[i]);
    }
    if (hasTangents)
    {
      copyToBufferAndMoveHead(combinedBuffers2.data(), headOffset, subMesh.tangents[i]);
    }
    if (hasTextureCoordinates)
    {
      copyToBufferAndMoveHead(combinedBuffers2.data(), headOffset, subMesh.textureCoordinates[i]);
    }
  }

  //If head is not exactly the same as the pre-calculated buffer size, there's a bug somewhere.
  ASSERT2(headOffset == combinedBufferByteSize, "Failed to calculate correct amount of copied bytes.");
  return combinedBuffers2;
}

void createBuffers(const SubMesh& subMesh, ID3D11Device& device, ID3D11Buffer*& vertexBuffer,
                   ID3D11Buffer*& indexBuffer)
{
  const std::vector<unsigned char> combinedBuffers(combineBuffers(subMesh));

  D3D11_BUFFER_DESC bufferDescription = { 0 };
  bufferDescription.Usage = D3D11_USAGE_DEFAULT;
  bufferDescription.ByteWidth = combinedBuffers.size();
  bufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;

  D3D11_SUBRESOURCE_DATA initData = { 0 };
  initData.pSysMem = combinedBuffers.data();

  HRESULT hr = device.CreateBuffer(&bufferDescription, &initData, &vertexBuffer);
  ASSERT2(SUCCEEDED(hr), "Failed to create vertex buffer");

  //Create index buffer.
  bufferDescription.ByteWidth = sizeof(unsigned int) * subMesh.indices.size();
  bufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
  initData.pSysMem = subMesh.indices.data();

  hr = device.CreateBuffer(&bufferDescription, &initData, &indexBuffer);
  ASSERT2(SUCCEEDED(hr), "Failed to create index buffer.");
}

VertexFormat::Enum calculateVertexFormat(const SubMesh& subMesh)
{
  unsigned int format = 0;
  if (!subMesh.vertices.empty())
  {
    format |= VertexFormat::VertexPositions;
  }
  if (!subMesh.normals.empty())
  {
    format |= VertexFormat::Normals;
  }
  if (!subMesh.tangents.empty())
  {
    format |= VertexFormat::Tangents;
  }
  if (!subMesh.textureCoordinates.empty())
  {
    format |= VertexFormat::TextureCoordinates;
  }
  return (VertexFormat::Enum)format;
}

void verifyMesh(const Mesh& mesh)
{
  ASSERT2(!mesh.subMeshes.empty(), "Mesh has no submeshes.");

  //Verify that all submeshes have identical formats.
  const VertexFormat::Enum subMeshVertexFormat = calculateVertexFormat(mesh.subMeshes[0]);
  for (unsigned int i = 1; i < mesh.subMeshes.size(); ++i)
  {
    const SubMesh& subMesh = mesh.subMeshes[i];
    ASSERT2(calculateVertexFormat(subMesh) == subMeshVertexFormat,
      "The vertex format does not match the content of the mesh.");
  }
}

std::shared_ptr<RuntimeMesh> createFromMesh(const Mesh& mesh, ID3D11Device& device)
{
  verifyMesh(mesh);

  std::shared_ptr<RuntimeMesh> runtimeMesh(std::make_shared<RuntimeMesh>());
  runtimeMesh->minExtents = XMFLOAT3A(mesh.minExtents.x, mesh.minExtents.y, mesh.minExtents.z);
  runtimeMesh->maxExtents = XMFLOAT3A(mesh.maxExtents.x, mesh.maxExtents.y, mesh.maxExtents.z);

  runtimeMesh->name = mesh.name;

  runtimeMesh->vertexBuffers.reserve(mesh.subMeshes.size());
  runtimeMesh->indexBuffers.reserve(mesh.subMeshes.size());

  for (unsigned int i = 0; i < mesh.subMeshes.size(); ++i)
  {
    const SubMesh& subMesh = mesh.subMeshes[i];
    ID3D11Buffer* vertexBuffer = nullptr;
    ID3D11Buffer* indexBuffer = nullptr;
    createBuffers(subMesh, device, vertexBuffer, indexBuffer);
    runtimeMesh->vertexBuffers.push_back(vertexBuffer);
    runtimeMesh->indexBuffers.push_back(indexBuffer);
    runtimeMesh->indexCounts.push_back(subMesh.indices.size());
    runtimeMesh->vertexCounts.push_back(subMesh.vertices.size());

    setDebugName(*vertexBuffer, "VB " + mesh.name + " : " + std::to_string(i));
    setDebugName(*indexBuffer, "IB " + mesh.name + " : " + std::to_string(i));
  }

  runtimeMesh->vertexFormat = calculateVertexFormat(mesh.subMeshes[0]);

  return runtimeMesh;
}
