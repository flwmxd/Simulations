//////////////////////////////////////////////////////////////////////////////
// This file is part of the PharaohStroy MMORPG client                      //
// Copyright ?2020-2022 Prime Zeng                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////


#include "BatchCommand.h"
#include "OGL.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

constexpr uint32_t MAX_QUADS = 20000;
constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;

BatchCommand::BatchCommand(Type type)
	:type(type)
{
	vertexArray.bind();

	vertexBuffer = std::make_shared<VertexBuffer>(MAX_VERTICES * sizeof(QuadVertex));
	vertexBuffer->setLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" }
	});
	vertexArray.addVertexBuffer(vertexBuffer);
	if (type == Type::TRIANGLES) {
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

		indexBuffer = std::make_shared<IndexBuffer>(quadIndices, MAX_INDICES);
		vertexArray.setIndexBuffer(indexBuffer);
		delete[] quadIndices;

		quadVertexPositions[0] = glm::vec4{ -0.5f, -0.5f, 0.0f, 1.0f };
		quadVertexPositions[1] = glm::vec4{ 0.5f, -0.5f, 0.0f, 1.0f };
		quadVertexPositions[2] = glm::vec4{ 0.5f,  0.5f, 0.0f, 1.0f };
		quadVertexPositions[3] = glm::vec4{ -0.5f,  0.5f, 0.0f, 1.0f };
	}

	CHECK_GL_ERROR();
}

auto BatchCommand::addVertex(const glm::vec3& position, const glm::vec4& color) -> void
{
	vertics.emplace_back(QuadVertex{ position, color });
}

auto BatchCommand::flush(opengl::Shader& shader) -> void
{
	shader.useProgram();
	vertexArray.bind();
	vertexBuffer->setData(vertics.data(), sizeof(QuadVertex) * vertics.size());

	switch (type)
	{
	case BatchCommand::Type::POINTS:
		glDrawArrays(GL_POINTS, 0, vertics.size());
		break;
	case BatchCommand::Type::LINES:
		glDrawArrays(GL_LINES, 0, vertics.size());
		break;
	case BatchCommand::Type::TRIANGLES:
		uint32_t count = quadIndexCount != 0 ? quadIndexCount : vertexArray.getIndexBuffer()->getCount();
		glDrawElements(GL_TRIANGLES, vertics.size(), GL_UNSIGNED_INT, nullptr);
		break;
	}
	vertexArray.unbind();
	vertics.clear();
	quadIndexCount = 0;
	CHECK_GL_ERROR();
}

auto BatchCommand::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, opengl::Shader& shader) -> void
{
	drawQuad(glm::vec3{ position.x, position.y, 0.0f }, size, color,shader);
}

auto BatchCommand::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, opengl::Shader& shader) -> void
{
	auto transform = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });

	drawQuad(transform, color, shader);
}

auto BatchCommand::drawQuad(const glm::mat4& transform, const glm::vec4& color, opengl::Shader& shader) -> void
{
	constexpr size_t quadVertexCount = 4;
	for (size_t i = 0; i < quadVertexCount; i++)
	{
		addVertex(transform * quadVertexPositions[i],color);
	}
}



auto BatchCommand::drawPolygon(const std::vector<glm::vec2>& vertices, const glm::mat4& transform, const glm::vec4& color, opengl::Shader& shader) -> void
{
	for (auto & v : vertices)
	{
		addVertex(transform * glm::vec4{v,1,1}, color);
	}
}

auto BatchCommand::drawPolygon(const std::vector<glm::vec2>& vertices, const std::vector<uint32_t>& index, const glm::mat4& transform, const glm::vec4& color, opengl::Shader& shader) -> void
{
	for (auto& v : vertices)
	{
		addVertex(transform * glm::vec4{ v,1,1 }, color);
	}
	indexBuffer->setData(index.data(), index.size());
}

auto BatchCommand::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, opengl::Shader& shader) -> void
{
}

auto BatchCommand::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, opengl::Shader& shader) -> void
{
}
