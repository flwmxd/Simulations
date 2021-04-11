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

#include "Renderer2D.h"
#include <memory>
#include "VertexArray.h"
#include "Buffer.h"
#include "OGL.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

constexpr uint32_t MAX_QUADS = 20000;
constexpr uint32_t MAX_VERTICES = MAX_QUADS * 4;
constexpr uint32_t MAX_INDICES = MAX_QUADS * 6;
constexpr uint32_t MAX_TEXTURE_SLOTS = 32; // TODO: RenderCaps

struct QuadVertex
{
	glm::vec3 position;
	glm::vec4 color;
};


struct Renderer2DData
{
	std::shared_ptr<VertexArray> quadVertexArray;
	std::shared_ptr<VertexBuffer> quadVertexBuffer;

	uint32_t quadIndexCount = 0;
	QuadVertex* quadVertexBufferBase = nullptr;
	QuadVertex* quadVertexBufferPtr = nullptr;

	glm::vec4 quadVertexPositions[4];
	Renderer2D::Statistics stats;
};
static Renderer2DData render2dData;

auto Renderer2D::init() -> void
{
	render2dData.quadVertexArray = std::make_shared<VertexArray>();
	render2dData.quadVertexBuffer = std::make_shared<VertexBuffer>(MAX_VERTICES * sizeof(QuadVertex));
	render2dData.quadVertexBuffer->setLayout({
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float4, "a_Color" },
	});

	render2dData.quadVertexArray->addVertexBuffer(render2dData.quadVertexBuffer);

	render2dData.quadVertexBufferBase = new QuadVertex[MAX_VERTICES];

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

	render2dData.quadVertexArray->setIndexBuffer(quadIB);
	delete[] quadIndices;

	int32_t samplers[MAX_TEXTURE_SLOTS];
	for (uint32_t i = 0; i < MAX_TEXTURE_SLOTS; i++)
		samplers[i] = i;

	
	render2dData.quadVertexPositions[0] = glm::vec4{ -0.5f, -0.5f, 0.0f, 1.0f };
	render2dData.quadVertexPositions[1] = glm::vec4{ 0.5f, -0.5f, 0.0f, 1.0f };
	render2dData.quadVertexPositions[2] = glm::vec4{ 0.5f,  0.5f, 0.0f, 1.0f };
	render2dData.quadVertexPositions[3] = glm::vec4{ -0.5f,  0.5f, 0.0f, 1.0f };
}


auto Renderer2D::shutdown() -> void
{
	delete [] render2dData.quadVertexBufferBase;
}



auto Renderer2D::beginScene(const glm::mat4 & view, const glm::mat4 & projection) -> void
{
	render2dData.quadIndexCount = 0;
	render2dData.quadVertexBufferPtr = render2dData.quadVertexBufferBase;
}

auto Renderer2D::endScene()  -> void
{
	uint32_t dataSize = (uint32_t)((uint8_t*)render2dData.quadVertexBufferPtr - (uint8_t*)render2dData.quadVertexBufferBase);
	render2dData.quadVertexBuffer->setData(render2dData.quadVertexBufferBase, dataSize);
	flush();
}
auto Renderer2D::flush() -> void
{
	if (render2dData.quadIndexCount == 0)
		return; // Nothing to draw
	uint32_t count = render2dData.quadIndexCount ? render2dData.quadIndexCount : render2dData.quadVertexArray->getIndexBuffer()->getCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	render2dData.stats.drawCalls++;
}

auto Renderer2D::drawQuad(const glm::vec2 & position, const glm::vec2& size, const glm::vec4& color) -> void
{
	drawQuad(glm::vec3{ position.x, position.y, 0.0f }, size, color);
}

auto Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) -> void
{
	auto transform = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });

	drawQuad(transform, color);
}

auto Renderer2D::drawQuad(const glm::mat4& transform, const glm::vec4& color) -> void
{
	constexpr size_t quadVertexCount = 4;
	const float textureIndex = 0.0f; // White Texture
	const glm::vec2 textureCoords[] = { glm::vec2{ 0.0f, 0.0f }, glm::vec2{ 1.0f, 0.0f }, glm::vec2{ 1.0f, 1.0f }, glm::vec2{ 0.0f, 1.0f } };
	const float tilingFactor = 1.0f;

	if (render2dData.quadIndexCount >= MAX_INDICES)
		flushAndReset();

	for (size_t i = 0; i < quadVertexCount; i++)
	{
		render2dData.quadVertexBufferPtr->position = transform * render2dData.quadVertexPositions[i];
		render2dData.quadVertexBufferPtr->color = color;
		render2dData.quadVertexBufferPtr++;
	}

	render2dData.quadIndexCount += 6;
	render2dData.stats.quadCount++;

}



auto Renderer2D::drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) -> void
{
	drawRotatedQuad(glm::vec3{ position.x, position.y, 0.0f }, size, rotation, color);
}

auto Renderer2D::drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) -> void
{
	auto transform = glm::translate(glm::mat4(1.0f), position)
		* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f })
		* glm::scale(glm::mat4(1.0f), glm::vec3{ size.x, size.y, 1.0f });
	

	drawQuad(transform, color);
}



auto Renderer2D::resetStats() -> void
{
	memset(&render2dData.stats, 0, sizeof(Statistics));
}

auto Renderer2D::getStats() -> const Statistics&
{
	return render2dData.stats;
}

auto Renderer2D::flushAndReset() -> void
{
	endScene();
	render2dData.quadIndexCount = 0;
	render2dData.quadVertexBufferPtr = render2dData.quadVertexBufferBase;
}

