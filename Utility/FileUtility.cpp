#include "FileUtility.h"

#include <fstream>
#include <streambuf>


std::string readWholeFile(const char* fileName)
{
  std::ifstream t(fileName);
  t.seekg(0, std::ios::end);
  //Setting size to a minimum of 1 makes buffer[0] not crash in case of an error or empty file.
  const size_t size = std::max(static_cast<size_t>(t.tellg()), (size_t)1);
  t.seekg(0);

  std::string buffer(size, 0);
  t.read(&buffer[0], size);
  return buffer;
}
