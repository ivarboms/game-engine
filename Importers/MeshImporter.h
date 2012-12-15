#pragma once

#pragma comment(lib, "Importers.lib")

#include <string>

struct Mesh;


Mesh importMesh(const std::string& fileName);
