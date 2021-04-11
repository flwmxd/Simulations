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

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <cmath>
#include <cstddef>
#include <limits>

class Transform
{
public:
	Transform();
	Transform(const glm::mat4& matrix);
	Transform(const glm::vec3& position);
	~Transform();

	inline auto setWorldMatrix(const glm::mat4& mat) { if (dirty) calculateMatrix(); worldTransform = mat * localTransform; }
	inline auto setLocalTransform(const glm::mat4& localMat) { localTransform = localMat; updated = true; applyTransform(); }
	inline auto setLocalPosition(const glm::vec3& localPos) { dirty = true; position = localPos; }
	inline auto setLocalScale(const glm::vec3& localScale) { dirty = true; scale = localScale; }
	inline auto setLocalRotation(const glm::quat & quat) { dirty = true; rotation = quat; }


	auto applyTransform() -> void;
	auto calculateMatrix() -> void;


	inline const auto getWorldPosition() const {  
		return getTranslation(worldTransform);
	}
	inline const auto getWorldRotation() const { 
		return getRotation(worldTransform);
	}

	inline const auto& getWorldMatrix() { if (dirty) { calculateMatrix(); } return worldTransform; }
	inline const auto& getLocalMatrix() { if (dirty) { calculateMatrix(); } return localTransform; }
	inline const auto& getLocalPosition() const { return position; }
	inline const auto& getLocalScale() const { return scale; }
	inline const auto& getLocalRotation() const { return rotation; }

	static auto getTranslation(const glm::mat4&)->glm::vec3;
	static auto getRotation(const glm::mat4&)->glm::quat;
	static auto getScale(const glm::mat4&)->glm::vec3;

private:
	glm::mat4 worldTransform = glm::mat4(1);
	glm::mat4 localTransform = glm::mat4(1);
	glm::quat rotation;
	glm::vec3 position;
	glm::vec3 scale = { 1,1,1 };

	bool dirty = false;
	bool updated = false;
};

