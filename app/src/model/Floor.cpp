

#include "Floor.h"
#include "opengl/Buffer.h"
#include "opengl/OGL.h"
#include <cmath>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>
#include "opengl/Vertex.h"

#ifndef M_PI
#define M_PI 3.14159265f
#endif

auto Floor::init() -> void
{
	Primitive::init();

	auto buffer = std::make_shared<VertexBuffer>(0);
	
	buffer->setLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float3, "aNormal"},
	});
	constexpr double pi = M_PI;
	
	std::vector<float> vertices;

	double   size = 200.0;
	int32_t  resolution = 20;
	for (int i = 0; i < resolution; ++i)
	{
		const double x_m = -0.5 * size + (i + 0) * size / static_cast<double>(resolution);
		const double x_p = -0.5 * size + (i + 1) * size / static_cast<double>(resolution);

		for (int j = 0; j < resolution; ++j)
		{
			const double z_m = -0.5 * size + (j + 0) * size / static_cast<double>(resolution);
			const double z_p = -0.5 * size + (j + 1) * size / static_cast<double>(resolution);

		
			vertices.emplace_back(x_p); vertices.emplace_back(0.0); vertices.emplace_back(z_p);
			vertices.emplace_back(x_p); vertices.emplace_back(0.0); vertices.emplace_back(z_p);

			vertices.emplace_back(x_m); vertices.emplace_back(0.0); vertices.emplace_back(z_p);
			vertices.emplace_back(x_m); vertices.emplace_back(0.0); vertices.emplace_back(z_p);

			vertices.emplace_back(x_m); vertices.emplace_back(0.0); vertices.emplace_back(z_m);
			vertices.emplace_back(x_m); vertices.emplace_back(0.0); vertices.emplace_back(z_m);

			vertices.emplace_back(x_p); vertices.emplace_back(0.0); vertices.emplace_back(z_m);
			vertices.emplace_back(x_p); vertices.emplace_back(0.0); vertices.emplace_back(z_m);
		}
	}


	constexpr uint32_t MAX_QUADS = 20000;

	constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;

	uint32_t* quadIndices = new uint32_t[MAX_INDICES];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < MAX_INDICES; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 2;
		quadIndices[i + 4] = offset + 3;
		quadIndices[i + 5] = offset + 0;

		offset += 4;
	}

	auto quadIB = std::make_shared<IndexBuffer>(quadIndices, MAX_INDICES);
	vertexArray->setIndexBuffer(quadIB);

	buffer->setData(vertices.data(), vertices.size() * sizeof(float));
	vertexArray->addVertexBuffer(buffer);

}

auto Floor::initWthCloth() -> void
{
	Primitive::init();

	auto buffer = std::make_shared<VertexBuffer>(0);

	buffer->setLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float3, "aNormal"},
			{ ShaderDataType::Float2, "aTexCoord"},
			{ ShaderDataType::Float4, "aColor"},
	});

	constexpr double pi = M_PI;

	

	std::vector<Vertex> vertices;


	double   size = 200.0;
	int32_t  resolution = 20;
	bool flag = false;
	for (int i = 0; i < resolution; ++i)
	{
		const double x_m = -0.5 * size + (i + 0) * size / static_cast<double>(resolution);
		const double x_p = -0.5 * size + (i + 1) * size / static_cast<double>(resolution);
		flag = !flag;
		for (int j = 0; j < resolution; ++j)
		{
			const double z_m = -0.5 * size + (j + 0) * size / static_cast<double>(resolution);
			const double z_p = -0.5 * size + (j + 1) * size / static_cast<double>(resolution);

			Vertex v;
			v.pos = { x_p,0,z_p };
			if (flag)
			{
				v.color = { 0.8,0.8, 0.8, 1.0 };
			}
			else 
			{
				v.color = { 0.7,0.7, 0.7, 1.0 };
			}
			vertices.emplace_back(v);
			v.pos = { x_m ,0,z_p };
			vertices.emplace_back(v);
			v.pos = { x_m ,0,z_m };
			vertices.emplace_back(v);
			v.pos = { x_p ,0,z_m };
			vertices.emplace_back(v);

			flag = !flag;
	
		}
	}




	constexpr uint32_t MAX_QUADS = 20000;

	constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;

	uint32_t* quadIndices = new uint32_t[MAX_INDICES];

	uint32_t offset = 0;
	for (uint32_t i = 0; i < MAX_INDICES; i += 6)
	{
		quadIndices[i + 0] = offset + 0;
		quadIndices[i + 1] = offset + 1;
		quadIndices[i + 2] = offset + 2;

		quadIndices[i + 3] = offset + 0;
		quadIndices[i + 4] = offset + 2;
		quadIndices[i + 5] = offset + 3;

		offset += 4;
	}

	for (auto i = 0; i < 3000; i += 3)
	{
		const int a = quadIndices[i];
		const int b = quadIndices[i + 1];
		const int c = quadIndices[i + 2];

		if (a < vertices.size() && b < vertices.size() && c< vertices.size()) 
		{
			const auto _normal = glm::cross((vertices[b].pos - vertices[a].pos), (vertices[c].pos - vertices[a].pos));

			vertices[a].normal += _normal;
			vertices[b].normal += _normal;
			vertices[c].normal += _normal;
		}
		
	}

	for (auto i = 0; i < vertices.size(); i++)
	{
		vertices[i].normal = glm::normalize(vertices[i].normal);
	}

	auto quadIB = std::make_shared<IndexBuffer>(quadIndices, MAX_INDICES);
	vertexArray->setIndexBuffer(quadIB);

	buffer->setData(vertices.data(), vertices.size() * sizeof(Vertex));
	vertexArray->addVertexBuffer(buffer);

	material.ambient = { 0.2f, 0.9f, 0.0f };
	material.diffuse = { 0.2f, 0.9f, 0.0f };
	material.specular = { 0.2f, 0.9f, 0.0f };
	material.shininess = 32.0f;
}

auto Floor::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void
{
	if (active) 
	{
		shader->useProgram();
		shader->setUniformValue("lightEnable", false);
		shader->setUniform3f("material.ambient", glm::value_ptr(material.ambient));
		shader->setUniform3f("material.diffuse", glm::value_ptr(material.diffuse));
		shader->setUniform3f("material.specular", glm::value_ptr(material.specular)); // specular lighting doesn't have full effect on this object's material
		shader->setUniformValue("material.shininess", material.shininess);
		shader->setUniformMatrix4f("model", glm::value_ptr(m));
		shader->setUniformMatrix4f("view", glm::value_ptr(v));
		shader->setUniformMatrix4f("projection", glm::value_ptr(p));
		shader->setUniform4f("color", glm::value_ptr(glm::vec4{ 0.9, 0.9, 0.9 ,1.0 }));
		vertexArray->bind();
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
		vertexArray->unbind();
		shader->setUniformValue("lightEnable", true);
	}
}

auto Floor::onImGui() -> void
{
	ImGui::Checkbox("active floor", &active);
}
