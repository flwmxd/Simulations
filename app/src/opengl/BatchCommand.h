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
#pragma once

#include <vector>
#include "VertexArray.h"
#include "Shader.h"
#include <glm/glm.hpp>

class BatchCommand 
{
public:
	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
	};

	enum class Type
	{
		POINTS,
		LINES,
		TRIANGLES
	};

	BatchCommand(Type type);
	auto addVertex(const glm::vec3& position,const glm::vec4 & color) -> void;
	auto flush(opengl::Shader & shader) -> void;

	auto drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,opengl::Shader & shader) -> void;
	auto drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, opengl::Shader& shader) -> void;
	auto drawQuad(const glm::mat4& transform, const glm::vec4& color, opengl::Shader& shader) -> void;

	auto drawPolygon(const std::vector<glm::vec2>& vertices, const glm::mat4& transform, const glm::vec4& color, opengl::Shader& shader) -> void;
	auto drawPolygon(const std::vector<glm::vec2>& vertices, const std::vector<uint32_t>& index, const glm::mat4& transform, const glm::vec4& color, opengl::Shader& shader) -> void;

	auto drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color, opengl::Shader& shader) -> void;
	auto drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, opengl::Shader& shader) -> void;

	inline auto getType() const { return type; }

private:
	std::vector<QuadVertex> vertics;
	Type type;
	VertexArray vertexArray;
	std::shared_ptr<VertexBuffer> vertexBuffer;
	std::shared_ptr< IndexBuffer> indexBuffer;
	glm::vec4 quadVertexPositions[4];
	uint32_t quadIndexCount = 0;
};