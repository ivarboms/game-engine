#include "MeshSerialization.h"

#include <fstream>

#pragma warning(push)
//boost\archive\basic_binary_iprimitive.hpp(181): warning C4244: 'argument' : conversion from
//'std::streamsize' to 'size_t', possible loss of data
#pragma warning(disable:4244)
//boost\archive\basic_binary_oprimitive.hpp(163): warning C4310: cast truncates constant value
#pragma warning(disable:4310)
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#pragma warning(pop)


void saveToFile(const Mesh& mesh, const std::wstring& fileName)
{
  std::ofstream fs(fileName, std::ios::binary);
  boost::archive::binary_oarchive archive(fs);
  archive << mesh;
}

Mesh loadFromFile(const std::wstring& fileName)
{
  std::ifstream fs(fileName, std::ios::binary);
  boost::archive::binary_iarchive archive(fs);
  Mesh mesh;
  archive >> mesh;
  return mesh;
}
