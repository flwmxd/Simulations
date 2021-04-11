
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
#include "OGLTexture.h"
#include "OGL.h"
#include <assert.h>
#include <stb_image.h>
#include "filesystem/Image.h"
#include "filesystem/ImageLoader.h"
#include <iostream>


OGLTexture::OGLTexture(uint32_t width, uint32_t height, const void* data, const TextureParameters & parameters, const TextureLoadOptions & loadOptions)
	:width(width),height(height),parameters(parameters),loadOptions(loadOptions)
{
	filePath = "";
	name = "";
	initWithData(data);
}

OGLTexture::OGLTexture(const std::string& initName, const std::string& path, const TextureParameters &parameters, const TextureLoadOptions&  loadOptions)
	:parameters(parameters), loadOptions(loadOptions), filePath(path)
{
	name = initName;
	initWithData(nullptr);
}

OGLTexture::OGLTexture()
{

}

OGLTexture::~OGLTexture()
{
	glDeleteTextures(1, &textureId);
}

auto OGLTexture::setData(void* data, uint32_t size) -> void
{
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,OGL::convert(parameters.format), GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	CHECK_GL_ERROR();
}

auto OGLTexture::bind(uint32_t slot) const -> void 
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, textureId);
	CHECK_GL_ERROR();
}


auto OGLTexture::unbind(uint32_t slot /*= 0*/) const -> void
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR();	
}


auto OGLTexture::loadTexture(const void* data)   -> void
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameters.minFilter == TextureFilter::LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameters.magFilter == TextureFilter::LINEAR ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, OGL::convert(parameters.wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, OGL::convert(parameters.wrap));
	auto format = OGL::convert(parameters.format);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, OGL::internalFormat(format), isHDR ? GL_FLOAT : GL_UNSIGNED_BYTE, data ? data : NULL);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}

auto OGLTexture::initWithData(const void* data) -> void
{
	const uint8_t * pixels = nullptr;
	std::unique_ptr<Image> image;
	if (data != nullptr)
	{
		pixels = reinterpret_cast<const uint8_t*>(data);
	}
	else
	{
		if (filePath != "")
		{
			image = ImageLoader::loadAsset(filePath,loadOptions.generateMipMaps);
			pixels = image->getData();
			parameters.format = image->getPixelFormat();
			width = image->getWidth();
			height = image->getHeight();
		}
	}
	loadTexture(pixels);
}
