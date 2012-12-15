#include "StdAfx.h"

#include "MeshImporter.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <boost/format.hpp>

using namespace DirectX;

#pragma comment(lib, "assimp.lib")
#pragma comment(lib, "zlib.lib")


void calculateAABB(Mesh& mesh)
{
  XMVECTOR min = XMVectorReplicate(FLT_MAX);
  XMVECTOR max = XMVectorReplicate(-FLT_MAX);

  for (unsigned int i = 0; i < mesh.subMeshes.size(); ++i)
  {
  	for (unsigned int j = 0; j < mesh.subMeshes[i].vertices.size(); ++j)
  	{
      XMVECTOR vertex = XMLoadFloat3(&mesh.subMeshes[i].vertices[j]);
      min = XMVectorMin(vertex, min);
      max = XMVectorMax(vertex, max);
  	}
  }

  XMStoreFloat3(&mesh.minExtents, min);
  XMStoreFloat3(&mesh.maxExtents, max);
}

void copyMeshData(const aiMesh& source, SubMesh& desination)
{
  desination.name = source.mName.C_Str();

  //Copy vertex data into
  const unsigned int numVertices = source.mNumVertices;
  const unsigned int numNormal = source.HasNormals() ? source.mNumVertices : 0;
  const unsigned int numTangents = source.HasTangentsAndBitangents() ? source.mNumVertices : 0;
  const unsigned int numTextureCoordinates = source.HasTextureCoords(0) ? source.mNumVertices : 0;

  desination.vertices.resize(numVertices);
  desination.normals.resize(numNormal);
  desination.tangents.resize(numTangents);
  desination.textureCoordinates.resize(numTextureCoordinates);

  memcpy(desination.vertices.data(), source.mVertices, sizeof(XMFLOAT3) * numVertices);
  memcpy(desination.normals.data(), source.mNormals, sizeof(XMFLOAT3) * numNormal);
  memcpy(desination.tangents.data(), source.mTangents, sizeof(XMFLOAT3) * numTangents);
  memcpy(desination.textureCoordinates.data(), source.mTextureCoords, sizeof(XMFLOAT2) * numTextureCoordinates);

  for (unsigned int i = 0; i < source.mNumFaces; ++i)
  {
    const aiFace& face = source.mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; ++j)
    {
      desination.indices.push_back(face.mIndices[j]);
    }
  }
}

Mesh importMesh(const std::string& fileName)
{
  Assimp::Importer importer;
  unsigned int postProcessFlags = aiProcess_ConvertToLeftHanded
    | aiProcessPreset_TargetRealtime_MaxQuality;
  //Remove split large meshes flag, set by aiProcessPreset_TargetRealtime_MaxQuality.
  //postProcessFlags &= ~aiProcess_SplitLargeMeshes;

  const aiScene* scene = importer.ReadFile(fileName, postProcessFlags);
  if (scene == nullptr)
  {
    std::string error((boost::format("Failed to load file '%1%': %2%") % fileName % importer.GetErrorString()).str());
    printf("%s", error.c_str());
    return Mesh();
  }

  Mesh mesh;
  mesh.name = fileName;
  mesh.subMeshes.resize(scene->mNumMeshes);
  for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
  {
  	aiMesh& importedMesh = *scene->mMeshes[i];
    SubMesh& subMesh = mesh.subMeshes[i];
    
    copyMeshData(importedMesh, subMesh);
  }

  calculateAABB(mesh);

  return mesh;
}

