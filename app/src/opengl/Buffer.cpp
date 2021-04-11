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
#include "Buffer.h"
#include "OGL.h"
#include "Shader.h"
#include <iostream>

VertexBuffer::VertexBuffer(uint32_t size)
	:VertexBuffer(nullptr, size)
{

}

VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	:size(size)
{
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
	CHECK_GL_ERROR();
}
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &bufferId);
}

auto VertexBuffer::bind() const -> void
{
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	CHECK_GL_ERROR();
}

auto VertexBuffer::unbind() const -> void
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto VertexBuffer::setData(const void* data, uint32_t size) -> void
{
	this->size = size;
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	//glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	CHECK_GL_ERROR();
}

auto VertexBuffer::updateData(const void* data, uint32_t size) -> void
{
	this->size = size;
	glBindBuffer(GL_ARRAY_BUFFER, bufferId);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	CHECK_GL_ERROR();
}

IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t initCount)
	:count(initCount)
{
	glGenBuffers(1, &bufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	CHECK_GL_ERROR();
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &bufferId);
}

auto IndexBuffer::bind() const -> void
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	CHECK_GL_ERROR();
}

auto IndexBuffer::unbind() const -> void
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

auto IndexBuffer::setData(const uint32_t* indices, uint32_t count) -> void
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
}
