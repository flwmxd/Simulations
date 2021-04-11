
#pragma once

#include <string>
#include <vector>
#include "model/Mesh.h"
#include "opengl/OGLTexture.h"

namespace ObjLoader
{
	auto loadObj(const std::string& name)->std::vector<std::shared_ptr<Mesh>>;
	auto loadObj(const std::string& name, Mesh* mesh)-> void;
	auto loadMaterialTextures(const std::string& typeName, std::vector<std::shared_ptr<OGLTexture>> & textures, const std::string& name, const std::string& directory, TextureParameters format) ->std::shared_ptr<OGLTexture>;
};

