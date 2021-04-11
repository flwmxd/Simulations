#include "Mesh.h"
#include <glm/gtc/type_ptr.hpp>
#include "opengl/OGL.h"
#include "filesystem/ImageLoader.h"
#include "tools/ObjLoader.h"
#include <unordered_set>

Mesh::Mesh()
	:Mesh("")
{
}

Mesh::Mesh(const std::string & name, bool load )
	:name(name)
{
	vertexArray = std::make_unique<VertexArray>();
	if (load) 
	{
		ObjLoader::loadObj(name, this);
	}
}


auto Mesh::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void
{
	shader->useProgram();
	shader->setUniformMatrix4f("model", glm::value_ptr(m));
	shader->setUniformMatrix4f("view", glm::value_ptr(v));
	shader->setUniformMatrix4f("projection", glm::value_ptr(p));
	vertexArray->bind();

	if (texture != nullptr) {
		texture->bind();
	}

	uint32_t count = vertexArray->getVertexBuffers().size();
	if (count > 0)
	{
		auto& vb = vertexArray->getVertexBuffers()[0];

		//glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vb->getSize() / sizeof(Vertex)));
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	}

	vertexArray->unbind();
	if (texture != nullptr) {
		texture->unbind();
	}
}

auto Mesh::loadTexture(const std::string& name) -> void
{
	auto image = ImageLoader::loadAsset(name, false);
	texture = std::make_shared<OGLTexture>(image->getWidth(), image->getHeight(), image->getData());
}

auto Mesh::onImGui() -> void
{

}

auto Mesh::generateNormals(uint32_t numVertices, glm::vec3* vertices, uint32_t* indices, uint32_t numIndices) -> glm::vec3*
{
	auto normals = new glm::vec3[numVertices];

	for (auto i = 0; i < numVertices; ++i)
	{
		normals[i] = {};
	}

	if (indices)
	{
		for (auto i = 0; i < numIndices; i += 3)
		{
			const int a = indices[i];
			const int b = indices[i + 1];
			const int c = indices[i + 2];

			const auto _normal = glm::cross((vertices[b] - vertices[a]), (vertices[c] - vertices[a]));

			normals[a] += _normal;
			normals[b] += _normal;
			normals[c] += _normal;
		}
	}
	else
	{
		// It's just a list of triangles, so generate face normals
		for (auto i = 0; i < numVertices; i += 3)
		{
			auto& a = vertices[i];
			auto& b = vertices[i + 1];
			auto& c = vertices[i + 2];

			const auto  _normal = glm::cross(b - a, c - a);

			normals[i] = _normal;
			normals[i + 1] = _normal;
			normals[i + 2] = _normal;
		}
	}

	for (auto i = 0; i < numVertices; ++i)
	{
		normals[i] = glm::normalize(normals[i]);
	}

	return normals;
}
