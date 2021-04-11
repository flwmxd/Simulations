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
#include <string>
#include <vector>
#include <memory>

enum class ShaderDataType
{
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
};


static auto ShaderDataTypeSize(ShaderDataType type) -> uint32_t
{
	switch (type)
	{
	case ShaderDataType::Float:    return 4;
	case ShaderDataType::Float2:   return 4 * 2;
	case ShaderDataType::Float3:   return 4 * 3;
	case ShaderDataType::Float4:   return 4 * 4;
	case ShaderDataType::Mat3:     return 4 * 3 * 3;
	case ShaderDataType::Mat4:     return 4 * 4 * 4;
	case ShaderDataType::Int:      return 4;
	case ShaderDataType::Int2:     return 4 * 2;
	case ShaderDataType::Int3:     return 4 * 3;
	case ShaderDataType::Int4:     return 4 * 4;
	case ShaderDataType::Bool:     return 1;
	}

	return 0;
}

struct BufferElement
{
	std::string name;
	ShaderDataType type;
	uint32_t size;
	size_t offset;
	bool normalized;

	BufferElement() = default;

	BufferElement(ShaderDataType type, const std::string& name,uint32_t offset = 0, bool normalized = false)
		: name(name), type(type), size(ShaderDataTypeSize(type)), offset(offset), normalized(normalized)
	{
	}

	inline auto getComponentCount() const
	{
		switch (type)
		{
		case ShaderDataType::Float:   return 1;
		case ShaderDataType::Float2:  return 2;
		case ShaderDataType::Float3:  return 3;
		case ShaderDataType::Float4:  return 4;
		case ShaderDataType::Mat3:    return 3; // 3* float3
		case ShaderDataType::Mat4:    return 4; // 4* float4
		case ShaderDataType::Int:     return 1;
		case ShaderDataType::Int2:    return 2;
		case ShaderDataType::Int3:    return 3;
		case ShaderDataType::Int4:    return 4;
		case ShaderDataType::Bool:    return 1;
		}

		return 0;
	}
};

class BufferLayout
{
public:
	BufferLayout() {}

	BufferLayout(const std::initializer_list<BufferElement>& initElements)
		: elements(initElements)
	{
		calculateOffsetsAndStride();
	}

	const auto& getElements() const { return elements; }

	inline auto getStride() const { return stride; }
	inline auto begin() { return elements.begin(); }
	inline auto end() { return elements.end(); }
	inline auto begin() const { return elements.begin(); }
	inline auto end() const { return elements.end(); }

private:
	auto calculateOffsetsAndStride() -> void
	{
		size_t offset = 0;
		stride = 0;
		for (auto& element : elements)
		{
			element.offset = offset;
			offset += element.size;
			stride += element.size;
		}
	}
private:
	std::vector<BufferElement> elements;
	uint32_t stride = 0;
};



class VertexBuffer
{
public:
	VertexBuffer(uint32_t size);
	VertexBuffer(float* vertices, uint32_t size);
	~VertexBuffer();
	auto bind() const -> void ;
	auto unbind() const -> void ;
	auto setData(const void* data, uint32_t size) -> void;
	auto updateData(const void* data, uint32_t size) -> void;
	inline auto setLayout(const BufferLayout& l) -> void  { layout = l; };
	inline auto& getLayout() const { return layout; }
	inline auto getSize() const { return size; }
private:
	size_t size = 0;
	BufferLayout layout;
	uint32_t bufferId = 0;
};

class IndexBuffer
{
public:
	IndexBuffer(uint32_t* indices, uint32_t count);
	~IndexBuffer();
	auto bind() const -> void ;
	auto unbind() const -> void ;
	auto setData(const  uint32_t* indices, uint32_t count) -> void;
	inline auto getCount() const -> uint32_t  { return count; };
private:
	uint32_t bufferId = 0;
	uint32_t count = 0;
};