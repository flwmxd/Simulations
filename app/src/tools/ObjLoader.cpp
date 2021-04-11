
#include "ObjLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <stdexcept>
#include "model/Mesh.h"
#include "opengl/Vertex.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


static std::vector<std::shared_ptr<OGLTexture>> loadedTextures;

auto ObjLoader::loadObj(const std::string& path) -> std::vector<std::shared_ptr<Mesh>>
{
	std::string resolvedPath = path;
	tinyobj::attrib_t attrib;
	std::string error;
	std::string warn;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	auto directory = resolvedPath.substr(0, resolvedPath.find_last_of('/'));
	std::string name = directory.substr(directory.find_last_of('/') + 1);

	bool ok = tinyobj::LoadObj(
		&attrib, &shapes, &materials,&warn, &error, (resolvedPath).c_str(), (directory + "/").c_str());


	bool singleMesh = shapes.size() == 1;

	std::vector<std::shared_ptr<Mesh>> meshes;

	for (const auto& shape : shapes)
	{
		uint32_t vertexCount = 0;
		const uint32_t numIndices = static_cast<uint32_t>(shape.mesh.indices.size());
		const uint32_t numVertices = numIndices; 

		std::unordered_map<Vertex, uint32_t> uniqueVertices;
		auto mesh = std::make_shared<Mesh>();

		for (auto i = 0; i < shape.mesh.indices.size(); i++)
		{
			auto& index = shape.mesh.indices[i];
			Vertex vertex;

			if (!attrib.texcoords.empty())
			{
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };
			}
			else
			{
				vertex.texCoord = {};

			}
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2] };

			if (!attrib.normals.empty())
			{
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2] };
			}
			else
			{
				vertex.normal = {};
			}

			glm::vec4 color = { 1,1,1,1 };

			if (shape.mesh.material_ids[0] >= 0)
			{
				tinyobj::material_t* mp = &materials[shape.mesh.material_ids[0]];
				color = { mp->diffuse[0], mp->diffuse[1], mp->diffuse[2], 1.0 };
			}

			vertex.color = color;
			

			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertexCount);
				mesh->getVertices().emplace_back(vertex);
			}

			mesh->getIndices().emplace_back(uniqueVertices[vertex]);
			vertexCount++;
		}
		auto vb = std::make_shared<VertexBuffer>(0);

		vb->setLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float3, "aNormal"},
			{ ShaderDataType::Float2, "aTexCoord"},
			{ ShaderDataType::Float4, "aColor"},
			});



		auto& vertices = mesh->getVertices();
		auto& indices = mesh->getIndices();
		for (auto i = 0; i < numIndices; i += 3)
		{
			const int a = indices[i];
			const int b = indices[i + 1];
			const int c = indices[i + 2];

			const auto _normal = glm::cross((vertices[b].pos - vertices[a].pos), (vertices[c].pos - vertices[a].pos));

			vertices[a].normal += _normal;
			vertices[b].normal += _normal;
			vertices[c].normal += _normal;
		}

		for (auto & v : vertices)
		{
			v.normal = glm::normalize(v.normal);
		}

		vb->setData(vertices.data(), sizeof(Vertex) * vertices.size());

		mesh->getVertexArray()->addVertexBuffer(vb);
		mesh->getVertexArray()->setIndexBuffer(std::make_shared<IndexBuffer>(indices.data(), indices.size()));

		meshes.emplace_back(mesh);

	}
	return meshes;
}


auto ObjLoader::loadObj(const std::string& path, Mesh* mesh) -> void
{
	std::string resolvedPath = path;
	tinyobj::attrib_t attrib;
	std::string error;
	std::string warn;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	auto directory = resolvedPath.substr(0, resolvedPath.find_last_of('/'));
	std::string name = directory.substr(directory.find_last_of('/') + 1);

	bool ok = tinyobj::LoadObj(
		&attrib, &shapes, &materials, &warn, &error, (resolvedPath).c_str(), (directory + "/").c_str());

	bool singleMesh = shapes.size() == 1;

	for (const auto& shape : shapes)
	{
		uint32_t vertexCount = 0;
		const uint32_t numIndices = static_cast<uint32_t>(shape.mesh.indices.size());
		const uint32_t numVertices = numIndices;
		std::unordered_map<Vertex, uint32_t> uniqueVertices;

		for (auto i = 0; i < shape.mesh.indices.size(); i++)
		{
			auto& index = shape.mesh.indices[i];
			Vertex vertex;

			if (!attrib.texcoords.empty())
			{
				vertex.texCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1] };
			}
			else
			{
				vertex.texCoord = {};

			}
			vertex.pos = {
				attrib.vertices[3 * index.vertex_index + 0],
				attrib.vertices[3 * index.vertex_index + 1],
				attrib.vertices[3 * index.vertex_index + 2] };

			if (!attrib.normals.empty())
			{
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2] };
			}
			else
			{
				vertex.normal = {};
			}

			glm::vec4 color = { 1,1,1,1 };

			if (shape.mesh.material_ids[0] >= 0)
			{
				tinyobj::material_t* mp = &materials[shape.mesh.material_ids[0]];
				color = { mp->diffuse[0], mp->diffuse[1], mp->diffuse[2], 1.0 };
			}

			vertex.color = color;

			mesh->getBoundingBox().merge(vertex);
			if (uniqueVertices.count(vertex) == 0)
			{
				uniqueVertices[vertex] = static_cast<uint32_t>(vertexCount);
				mesh->getVertices().emplace_back(vertex);
				vertexCount++;
			}
			mesh->getIndices().emplace_back(uniqueVertices[vertex]);
		}
		auto vb = std::make_shared<VertexBuffer>(0);

		vb->setLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float3, "aNormal"},
			{ ShaderDataType::Float2, "aTexCoord"},
			{ ShaderDataType::Float4, "aColor"},
			});



		auto& vertices = mesh->getVertices();
		auto& indices = mesh->getIndices();
		for (auto i = 0; i < indices.size(); i += 3)
		{
			const int a = indices[i];
			const int b = indices[i + 1];
			const int c = indices[i + 2];

			const auto _normal = glm::cross((vertices[b].pos - vertices[a].pos), (vertices[c].pos - vertices[a].pos));

			vertices[a].normal += _normal;
			vertices[b].normal += _normal;
			vertices[c].normal += _normal;
		}

		for (auto i = 0; i < vertices.size(); i++)
		{
			vertices[i].normal = glm::normalize(vertices[i].normal);
		}

		vb->setData(vertices.data(), sizeof(Vertex) * vertices.size());

		mesh->getVertexArray()->addVertexBuffer(vb);
		mesh->getVertexArray()->setIndexBuffer(std::make_shared<IndexBuffer>(indices.data(), indices.size()));
	}
}

auto ObjLoader::loadMaterialTextures(const std::string& typeName, std::vector<std::shared_ptr<OGLTexture>>& textures, const std::string& name, const std::string& directory, TextureParameters format) -> std::shared_ptr<OGLTexture>
{
	for (auto j = 0; j < textures.size(); j++)
	{
		if (std::strcmp(textures[j]->getFilePath().c_str(), (directory + "/" + name).c_str()) == 0)
		{
			return textures[j];
		}
	}

	TextureLoadOptions options(false, true);
	auto texture = std::make_shared<OGLTexture>(typeName, directory + "/" + name, format, options);
	textures.emplace_back(texture); // Store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
	return texture;
}

