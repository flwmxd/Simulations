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

	enum class TextureWrap
	{
		NONE,
		REPEAT,
		CLAMP,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class TextureFilter
	{
		NONE,
		LINEAR,
		NEAREST
	};

	enum class TextureFormat
	{
		NONE,
		R8,
		RG8,
		RGB8,
		RGBA8,
		RGB16,
		RGBA16,
		RGB32,
		RGBA32,
		RGB,
		RGBA,
		DEPTH,
		STENCIL,
		DEPTH_STENCIL
	};

	enum class TextureType
	{
		COLOUR,
		DEPTH,
		DEPTHARRAY,
		CUBE,
		OTHER
	};

	struct TextureParameters
	{
		TextureFormat format;
		TextureFilter minFilter;
		TextureFilter magFilter;
		TextureWrap wrap;

		TextureParameters()
		{
			format = TextureFormat::RGBA;
			minFilter = TextureFilter::LINEAR;
			magFilter = TextureFilter::LINEAR;
			wrap = TextureWrap::CLAMP_TO_EDGE;
		}

		TextureParameters(TextureFormat format, TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap)
			: format(format)
			, minFilter(minFilter)
			, magFilter(magFilter)
			, wrap(wrap)
		{
		}

		TextureParameters(TextureFilter minFilter, TextureFilter magFilter)
			: format(TextureFormat::RGBA)
			, minFilter(minFilter)
			, magFilter(magFilter)
			, wrap(TextureWrap::CLAMP)
		{
		}

		TextureParameters(TextureFilter minFilter, TextureFilter magFilter, TextureWrap wrap)
			: format(TextureFormat::RGBA)
			, minFilter(minFilter)
			, magFilter(magFilter)
			, wrap(wrap)
		{
		}

		TextureParameters(TextureWrap wrap)
			: format(TextureFormat::RGBA)
			, minFilter(TextureFilter::LINEAR)
			, magFilter(TextureFilter::LINEAR)
			, wrap(wrap)
		{
		}

		TextureParameters(TextureFormat format)
			: format(format)
			, minFilter(TextureFilter::LINEAR)
			, magFilter(TextureFilter::LINEAR)
			, wrap(TextureWrap::CLAMP)
		{
		}
	};

	struct TextureLoadOptions
	{
		bool flipX;
		bool flipY;
		bool generateMipMaps;

		TextureLoadOptions()
		{
			flipX = false;
			flipY = false;
			generateMipMaps = true;
		}

		TextureLoadOptions(bool flipX, bool flipY, bool genMips = true)
			: flipX(flipX)
			, flipY(flipY)
			, generateMipMaps(genMips)
		{
		}
	};
