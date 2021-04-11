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

#include <string>
#include <memory>
#include "TextureFormat.h"
#include "RenderFormat.h"



class Image;

class OGLTexture
{
public:
	OGLTexture();
	OGLTexture(uint32_t width, uint32_t height, const void* data, const TextureParameters & parameters = {}, const TextureLoadOptions& loadOptions = {});
	OGLTexture(const std::string& name, const std::string& path, const TextureParameters& parameters = {}, const TextureLoadOptions& loadOptions = {});
	virtual ~OGLTexture();

	inline auto getWidth() const ->uint32_t  { return width; }

	inline auto getHeight() const->uint32_t  { return height; }

	inline auto operator==(const OGLTexture & other) const -> bool  { return textureId == ((OGLTexture&)other).textureId; };

	auto setData(void* data, uint32_t size) -> void ;

	auto bind(uint32_t slot = 0) const -> void ;
	auto unbind(uint32_t slot = 0) const -> void ;
	auto getHandle() const -> void*  { return (void*)&textureId; }

	auto loadTexture(const void* data)  -> void;

	inline auto& getFilePath() const { return filePath; }

private:

	auto initWithData(const void* data) -> void;
	std::string name;
	uint32_t width = 0;
	uint32_t height = 0;
	uint32_t textureId = 0;
	std::string filePath;
	TextureParameters parameters;
	TextureLoadOptions loadOptions;
	bool isHDR = false;
};
