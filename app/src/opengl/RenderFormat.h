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


enum RendererBufferType
{
	RENDERER_BUFFER_NONE = 0,
	RENDERER_BUFFER_COLOUR = 1,
	RENDERER_BUFFER_DEPTH = 2,
	RENDERER_BUFFER_STENCIL = 4
};

enum class DrawType
{
	POINT,
	TRIANGLE,
	LINES
};

enum class StencilType
{
	EQUAL,
	NOTEQUAL,
	KEEP,
	REPLACE,
	ZERO,
	ALWAYS
};

enum class PixelPackType
{
	PACK,
	UNPACK
};

enum class BlendFunction
{
	NONE,
	ZERO,
	ONE,
	SOURCE_ALPHA,
	DESTINATION_ALPHA,
	ONE_MINUS_SOURCE_ALPHA
};

enum class BlendEquation
{
	NONE,
	ADD,
	SUBTRACT
};

enum class RenderMode
{
	FILL,
	WIREFRAME
};

enum class DataType
{
	FLOAT,
	UNSIGNED_INT,
	UNSIGNED_BYTE
};
