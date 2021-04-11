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
#	include <GL/glew.h>
#	include <GLFW/glfw3.h>
#	include "TextureFormat.h"
#	include "RenderFormat.h"

#define CHECK_GL_ERROR() 																\
																						\
do																						\
{																						\
char errorMessage[128] = "\0";															\
	GLenum error = glGetError();														\
	if (error != GL_NO_ERROR){															\
	sprintf(errorMessage, "OpenGL error 0x%04x at %s:%i \n",  error, __func__, __LINE__);	std::cout<<errorMessage;\
}																						\
} while (0)


namespace OGL
{
	inline auto convert(const TextureWrap& wrap)
	{
		switch (wrap)
		{
#ifndef PLATFORM_MOBILE
		case TextureWrap::CLAMP:			return GL_CLAMP;
		case TextureWrap::CLAMP_TO_BORDER:	return GL_CLAMP_TO_BORDER;
#endif
		case TextureWrap::CLAMP_TO_EDGE:	return GL_CLAMP_TO_EDGE;
		case TextureWrap::REPEAT:			return GL_REPEAT;
		case TextureWrap::MIRRORED_REPEAT:	return GL_MIRRORED_REPEAT;
		default:
			return 0;
		}
	}

	inline auto convert(const TextureFormat& format)
	{
		switch (format)
		{
		case TextureFormat::RGBA:				return GL_RGBA;
		case TextureFormat::RGB:				return GL_SRGB;
		case TextureFormat::R8:				    return GL_R8;
		case TextureFormat::RG8:				return GL_RG8;
		case TextureFormat::RGB8:				return GL_RGB8;
		case TextureFormat::RGBA8:				return GL_SRGB_ALPHA;//GL_RGBA8;
		case TextureFormat::RGB16:              return GL_RGB16F;
		case TextureFormat::RGBA16:             return GL_RGBA16F;
		case TextureFormat::RGB32:              return GL_RGB32F;
		case TextureFormat::RGBA32:             return GL_RGBA32F;
		case TextureFormat::DEPTH:              return GL_DEPTH24_STENCIL8;
		default:
			return 0;
		}
	}

	inline auto convert(uint32_t bit)
	{
		uint32_t result = 0;
		if (bit & RENDERER_BUFFER_COLOUR)
			result |= GL_COLOR_BUFFER_BIT;
		if (bit & RENDERER_BUFFER_DEPTH)
			result |= GL_DEPTH_BUFFER_BIT;
		if (bit & RENDERER_BUFFER_STENCIL)
			result |= GL_STENCIL_BUFFER_BIT;
		return result;
	}


	inline auto convert(BlendFunction function)
	{
		switch (function)
		{
		case BlendFunction::ZERO:						return GL_ZERO;
		case BlendFunction::ONE:						return GL_ONE;
		case BlendFunction::SOURCE_ALPHA:				return GL_SRC_ALPHA;
		case BlendFunction::DESTINATION_ALPHA:			return GL_DST_ALPHA;
		case BlendFunction::ONE_MINUS_SOURCE_ALPHA:		return GL_ONE_MINUS_SRC_ALPHA;
		default: return 0;
		}
	}

	inline auto convert(StencilType type)
	{
		switch (type)
		{
		case StencilType::EQUAL:	return GL_EQUAL;
		case StencilType::NOTEQUAL:	return GL_NOTEQUAL;
		case StencilType::KEEP:		return GL_KEEP;
		case StencilType::REPLACE:	return GL_REPLACE;
		case StencilType::ZERO:		return GL_ZERO;
		case StencilType::ALWAYS:	return GL_ALWAYS;
		default:
			return 0;
		}
	}


	inline auto convert(DataType dataType)
	{
		switch (dataType)
		{
		case DataType::FLOAT: return GL_FLOAT;
		case DataType::UNSIGNED_INT: return GL_UNSIGNED_INT;
		case DataType::UNSIGNED_BYTE: return GL_UNSIGNED_BYTE;
		}
		return 0;
	}

	inline auto convert(DrawType drawType)
	{
		switch (drawType)
		{
		case DrawType::POINT:    return GL_POINTS;
		case DrawType::LINES:    return GL_LINES;
		case DrawType::TRIANGLE: return GL_TRIANGLES;
		}
		return 0;
	}

	inline auto internalFormat(uint32_t format)
	{
		switch (format)
		{
		case GL_RGBA:				return GL_RGBA;
		case GL_RGB:				return GL_RGB;
		case GL_R8:				    return GL_RED;
		case GL_RG8:				return GL_RG;
		case GL_RGB8:				return GL_RGB;
		case GL_RGBA8:				return GL_RGBA;
		case GL_RGB16:              return GL_RGB;
		case GL_RGBA16:             return GL_RGBA;
		case GL_RGBA16F:            return GL_RGB;
		case GL_RGB32F:             return GL_RGB;
		case GL_RGBA32F:            return GL_RGBA;
		case GL_SRGB:               return GL_RGB;
		case GL_SRGB_ALPHA:         return GL_RGBA;
		case GL_LUMINANCE:			return GL_LUMINANCE;
		case GL_LUMINANCE_ALPHA:	return GL_LUMINANCE_ALPHA;
		default:
			return 0;
		}
	}
};