
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
#include <vector>
#include "opengl/TextureFormat.h"


class Image final
{
public:
	Image() = default;

	Image(TextureFormat initPixelFormat,
		  uint32_t width,uint32_t height,
		  uint8_t * initData):
		pixelFormat(initPixelFormat), width(width), height(height), data(initData)
	{
	}

	~Image() {
		free(data);
	}

	inline auto getPixelFormat() const noexcept { return pixelFormat; }
	inline auto& getWidth() const noexcept { return width; }
	inline auto& getHeight() const noexcept { return height; }
	inline auto& getData() const noexcept { return data; }

private:
	TextureFormat pixelFormat = TextureFormat::RGBA8;
	uint32_t width = 0;
	uint32_t height = 0;
	uint8_t* data = nullptr;
};

