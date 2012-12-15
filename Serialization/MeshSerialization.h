#pragma once

#include <string>

#include <Mesh.h>


void saveToFile(const Mesh& mesh, const std::wstring& fileName);

Mesh loadFromFile(const std::wstring& fileName);

#include <boost/serialization/tracking.hpp>
#include <boost/serialization/vector.hpp>
BOOST_CLASS_TRACKING(Mesh, boost::serialization::track_never);
//BOOST_CLASS_TRACKING(SubMesh, boost::serialization::track_never);
namespace boost
{
  namespace serialization
  {
    template<class Archive>
    void serialize(Archive& ar, DirectX::XMFLOAT2& vec, const unsigned int)
    {
      ar & vec.x;
      ar & vec.y;
    }

    template<class Archive>
    void serialize(Archive& ar, DirectX::XMFLOAT3& vec, const unsigned int)
    {
      ar & vec.x;
      ar & vec.y;
      ar & vec.z;
    }

    template<class Archive>
    void serialize(Archive& ar, Mesh& mesh, const unsigned int)
    {
      ar & mesh.name;
      ar & mesh.subMeshes;
      ar & mesh.minExtents;
      ar & mesh.maxExtents;
    }

    template<class Archive>
    void serialize(Archive& ar, SubMesh& subMesh, const unsigned int file_version)
    {
      boost::serialization::split_free(ar, subMesh, file_version);
    }

    template<class Archive>
    void save(Archive& ar, const SubMesh& subMesh, const unsigned int)
    {
      ar << subMesh.name;
      ar << subMesh.indices;
      ar << subMesh.vertices;
      ar << subMesh.normals;
      ar << subMesh.tangents;
      ar << subMesh.textureCoordinates;
    }

    template<class Archive>
    void load(Archive& ar, SubMesh& subMesh, const unsigned int)
    {
      ar >> subMesh.name;
      ar >> subMesh.indices;
      ar >> subMesh.vertices;
      ar >> subMesh.normals;
      ar >> subMesh.tangents;
      ar >> subMesh.textureCoordinates;
    }
  }
}
