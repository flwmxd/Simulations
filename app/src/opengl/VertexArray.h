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

#include <memory>
#include "Buffer.h"


class VertexArray
{
public:
	VertexArray();
	virtual ~VertexArray();

	auto bind() const -> void;
	auto unbind() const  -> void;
	auto addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void;
	auto setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) -> void;
	auto clearVertexBuffer() -> void;
	inline auto& getVertexBuffers() const { return vertexBuffers; };
	inline auto& getIndexBuffer() const { return indexBuffer; }
private:
	uint32_t vao = 0;
	uint32_t vertexBufferIndex = 0;
	std::vector<std::shared_ptr<VertexBuffer>> vertexBuffers;
	std::shared_ptr<IndexBuffer> indexBuffer;
};

