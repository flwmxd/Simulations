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

#include "Transform.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

Transform::Transform()
{
}

Transform::Transform(const glm::mat4& matrix)
{
	position = getTranslation(matrix);
	rotation = getRotation(matrix);
	scale = getScale(matrix);
	localTransform = matrix;
	worldTransform = matrix;
}

Transform::Transform(const glm::vec3& position)
{
	setLocalPosition(position);
}
Transform::~Transform()
{
}
auto Transform::applyTransform() -> void
{
	position = getTranslation(localTransform);
	rotation = getRotation(localTransform);
	scale = getScale(localTransform);
}

auto Transform::calculateMatrix() -> void
{

	localTransform = glm::translate(glm::mat4(1), position);

	auto rotationMatrix = glm::mat4_cast(rotation);

	localTransform *= rotationMatrix;
	localTransform *= glm::scale(glm::mat4(1), scale);
	dirty = false;
	updated = true;
}

auto Transform::getTranslation(const glm::mat4& m) -> glm::vec3
{
	return { m[3][0], m[3][1], m[3][2] };
}

auto Transform::getRotation(const glm::mat4& m) -> glm::quat
{
	auto scale = getScale(m);

	const auto m11 = m[0][0] / scale[0];
	const auto m21 = m[0][1] / scale[0];
	const auto m31 = m[0][2] / scale[0];

	const auto m12 = m[1][0] / scale[1];
	const auto m22 = m[1][1] / scale[1];
	const auto m32 = m[1][2] / scale[1];

	const auto m13 = m[2][0] / scale[2];
	const auto m23 = m[2][1] / scale[2];
	const auto m33 = m[2][2] / scale[2];

	glm::quat result;

	result[0] = std::sqrt(std::max(0.f, 1.f + m11 - m22 - m33)) / float(2);
	result[1] = std::sqrt(std::max(0.f, 1.f - m11 + m22 - m33)) / float(2);
	result[2] = std::sqrt(std::max(0.f, 1.f - m11 - m22 + m33)) / float(2);
	result[3] = std::sqrt(std::max(0.f, 1.f + m11 + m22 + m33)) / float(2);

	// The problem with using copysign: http://www.euclideanspace.com/maths/geometry/rotations/conversions/matrixToQuaternion/paul.htm
	result[0] = std::copysign(result[0], m32 - m23);
	result[1] = std::copysign(result[1], m13 - m31);
	result[2] = std::copysign(result[2], m21 - m12);

	return glm::normalize(result);
}

auto Transform::getScale(const glm::mat4& m) ->glm::vec3
{
	glm::vec3 scale;
	scale[0] = glm::length(glm::vec3(m[0][0], m[0][1],m[0][2]));
	scale[1] = glm::length(glm::vec3(m[1][0], m[1][1],m[1][2]));
	scale[2] = glm::length(glm::vec3(m[2][0], m[2][1],m[2][2]));
	return scale;
}


