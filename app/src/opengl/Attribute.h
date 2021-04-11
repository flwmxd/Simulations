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
namespace opengl
{
    
    enum class AttributeType
	{
        TYPE_INT8,
		TYPE_INT16,
        TYPE_INT32,
        TYPE_UINT8,
		TYPE_UINT16,
        TYPE_UINT32,
		TYPE_FLOAT,
	};

	struct Attribute
	{
		std::string name;
		int32_t size;
        AttributeType type;
		int32_t offset;
		int32_t pos;
	};
};