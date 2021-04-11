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

#include <memory>
#include <stdexcept>
#include "ImageLoader.h"

#define STBI_NO_PSD
#define STBI_NO_HDR
#define STBI_NO_PIC
#define STBI_NO_GIF
#define STBI_NO_PNM
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


auto ImageLoader::loadAsset(const std::string& name, bool mipmaps) -> std::unique_ptr<Image>
{
	int32_t width;
	int32_t height;
	int32_t channels;
	TextureFormat format = TextureFormat::RGBA8;

	auto data = stbi_load(name.c_str(), &width, &height, &channels, STBI_default);

	stbi_set_flip_vertically_on_load(1);
	assert(data);

	if (channels == 4)
	{
		format = TextureFormat::RGBA8;
	}
	else if (channels == 3)
	{
		format = TextureFormat::RGB8;
	}

	return std::make_unique<Image>(format, width, height, data);
}

