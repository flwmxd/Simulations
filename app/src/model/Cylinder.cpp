

#include "Cylinder.h"
#include "opengl/Buffer.h"
#include "opengl/OGL.h"
#include <cmath>
#include <glm/gtc/type_ptr.hpp>
#ifndef M_PI
#define M_PI 3.14159265f
#endif

auto Cylinder::init() -> void
{
	Primitive::init();

	auto resolution = 30;
	auto buffer = std::make_shared<VertexBuffer>(0);
	
	buffer->setLayout({
			{ ShaderDataType::Float3, "aPos" },
			{ ShaderDataType::Float3, "aNormal"},
		});
	constexpr double pi = M_PI;
	
	std::vector<float> vertices;

	for (auto i = 0; i < resolution; ++i)
	{
		const double theta_1 = 2.0 * static_cast<double>(i + 0) * pi / static_cast<double>(resolution);
		const double theta_2 = 2.0 * static_cast<double>(i + 1) * pi / static_cast<double>(resolution);
		const double cos_1 = std::cos(theta_1);
		const double cos_2 = std::cos(theta_2);
		const double sin_1 = std::sin(theta_1);
		const double sin_2 = std::sin(theta_2);

		vertices.emplace_back(cos_1); vertices.emplace_back(sin_1); vertices.emplace_back(0);
		vertices.emplace_back(cos_1); vertices.emplace_back(sin_1); vertices.emplace_back(0);

		vertices.emplace_back(cos_2); vertices.emplace_back(sin_2); vertices.emplace_back(0);
		vertices.emplace_back(cos_2); vertices.emplace_back(sin_2); vertices.emplace_back(0);

		vertices.emplace_back(cos_2); vertices.emplace_back(sin_2); vertices.emplace_back(1);
		vertices.emplace_back(cos_2); vertices.emplace_back(sin_2); vertices.emplace_back(0);

		vertices.emplace_back(cos_1); vertices.emplace_back(sin_1); vertices.emplace_back(1);
		vertices.emplace_back(cos_1); vertices.emplace_back(sin_1); vertices.emplace_back(0);
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

auto Cylinder::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) -> void
{
	shader->useProgram();
	shader->setUniformMatrix4f("model", glm::value_ptr(m));
	shader->setUniformMatrix4f("view", glm::value_ptr(v));
	shader->setUniformMatrix4f("projection", glm::value_ptr(p));
	vertexArray->bind();
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	vertexArray->unbind();
}


auto Cylinder::draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, const Eigen::Vector3f& p1, const Eigen::Vector3f & p2, const glm::vec4& color) -> void
{
	const Eigen::Vector3f t = p2 - p1;
	const Eigen::Vector3f t_0 = Eigen::Vector3f::UnitZ();
	const float        c = t.dot(t_0) / t.norm();
	const float        q = std::acos(c);
	const Eigen::Vector3f ax = t_0.cross(t).normalized();
	const float          h = t.norm();
	const Eigen::Matrix4f rot = [&]()
	{
		return std::isnan(q) ? Eigen::Matrix4f::Identity() : Eigen::Affine3f(Eigen::AngleAxisf(q, ax)).matrix();
	}();
	
	glm::mat4 model = glm::mat4(1);
	model *= glm::translate(glm::mat4(1), { p1.x(),p1.y(),p1.z() });
	model *= glm::mat4(
		rot.data()[0], rot.data()[1], rot.data()[2], rot.data()[3],
		rot.data()[4], rot.data()[5], rot.data()[6], rot.data()[7],
		rot.data()[8], rot.data()[9], rot.data()[10], rot.data()[11],
		rot.data()[12], rot.data()[13], rot.data()[14], rot.data()[15]
	);
	model *= glm::scale(glm::mat4(1), { 0.025,0.025 ,h });

	model = m * model;

	transform = model;

	shader->useProgram();
	shader->setUniformMatrix4f("model", glm::value_ptr(model));
	shader->setUniformMatrix4f("view", glm::value_ptr(v));
	shader->setUniformMatrix4f("projection", glm::value_ptr(p));
	shader->setUniform4f("color", glm::value_ptr(color));
	draw(shader, 0.025f, h);
}

auto Cylinder::draw(opengl::Shader* shader, double radius, double height, int32_t resolution /*= 30*/) -> void
{

	vertexArray->bind();
	glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	vertexArray->unbind();
}

auto Cylinder::resetTransform() -> void
{
	transform = glm::mat4(1);
}

