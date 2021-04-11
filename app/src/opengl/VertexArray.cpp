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

#include "VertexArray.h"
#include "OGL.h"
#include <iostream>


static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return GL_FLOAT;
	case ShaderDataType::Float2:   return GL_FLOAT;
	case ShaderDataType::Float3:   return GL_FLOAT;
	case ShaderDataType::Float4:   return GL_FLOAT;
	case ShaderDataType::Mat3:     return GL_FLOAT;
	case ShaderDataType::Mat4:     return GL_FLOAT;
	case ShaderDataType::Int:      return GL_INT;
	case ShaderDataType::Int2:     return GL_INT;
	case ShaderDataType::Int3:     return GL_INT;
	case ShaderDataType::Int4:     return GL_INT;
	case ShaderDataType::Bool:     return GL_BOOL;
	}

	return 0;
}

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &vao);
	CHECK_GL_ERROR();
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &vao);
}

auto VertexArray::bind() const -> void
{
	glBindVertexArray(vao);
	CHECK_GL_ERROR();
}

auto VertexArray::unbind() const -> void 
{
	glBindVertexArray(0);
}

//bind vbo[s]
auto VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) -> void 
{
	glBindVertexArray(vao);
	CHECK_GL_ERROR();
	vertexBuffer->bind();
	const auto& layout = vertexBuffer->getLayout();
	for (const auto& element : layout)
	{
		switch (element.type)
		{
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
		case ShaderDataType::Bool:
		{
			glEnableVertexAttribArray(vertexBufferIndex);
			glVertexAttribPointer(vertexBufferIndex,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.offset);
			vertexBufferIndex++;
			break;
		}
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
		{
			uint8_t count = element.getComponentCount();
			for (uint8_t i = 0; i < count; i++)
			{
				glEnableVertexAttribArray(vertexBufferIndex);
				glVertexAttribPointer(vertexBufferIndex,
					count,
					ShaderDataTypeToOpenGLBaseType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*)(sizeof(float) * count * i));
				glVertexAttribDivisor(vertexBufferIndex, 1);
				vertexBufferIndex++;
			}
			break;
		}
		default:
			break;
		}
	}
	vertexBuffers.emplace_back(vertexBuffer);
}

auto VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) -> void 
{
	glBindVertexArray(vao);
	CHECK_GL_ERROR();
	indexBuffer->bind();
	this->indexBuffer = indexBuffer;
}

auto VertexArray::clearVertexBuffer() -> void
{
	vertexBuffers.clear();
}

