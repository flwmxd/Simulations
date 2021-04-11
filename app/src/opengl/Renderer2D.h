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
#include <cstdint>
#include <glm/glm.hpp>

class Renderer2D 
{
public:
	static auto init() -> void;
	static auto shutdown() -> void;
	static auto beginScene(const glm::mat4 & view, const glm::mat4 & projection) -> void;
	static auto endScene() -> void;
	static auto flush() -> void;

	// Primitives
	static auto drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) -> void;
	static auto drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) -> void;
	static auto drawQuad(const glm::mat4& transform, const glm::vec4& color) -> void;
	static auto drawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color) -> void;
	static auto drawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color) -> void;

	// Stats
	struct Statistics
	{
		uint32_t drawCalls = 0;
		uint32_t quadCount = 0;

		inline auto getTotalVertexCount() const  { return quadCount * 4; }
		inline auto getTotalIndexCount()  const  { return quadCount * 6; }
	};
	static auto resetStats() -> void;
	static auto getStats() -> const Statistics& ;
private:
	static auto flushAndReset() -> void;
};
