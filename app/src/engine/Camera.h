

#pragma once
#include <glm/glm.hpp>
#include "engine/KeyCodes.h"

constexpr auto YAW = -90.0f;
constexpr auto PITCH = 0.0f;
constexpr auto SPEED = 50.0f;
constexpr auto SENSITIVITY = 0.08f;
constexpr auto ZOOM = 45.0f;

class Camera
{
public:

	Camera(const glm::mat4& initProjection,const glm::vec3& initPosition = { 0, 0, 0 }, const glm::vec3& up = { 0,1,0 }, float yaw = YAW, float pitch = PITCH);
	virtual ~Camera() = default;
	inline const auto& getProjection() const { return projection; }
	inline auto& getPosition() const { return position; }
	auto getViewMatrix() const->glm::mat4;

	auto updateCameraVectors() -> void;
	auto onMouseScrolled(float yOffset) -> void;
	auto onMouseMove(float x, float y, bool constrainPitch = true) -> void;
	auto onKeyDown(KeyCode keyCode) -> void;
protected:
	auto updateProjection() -> void;

	glm::mat4 projection = glm::mat4(1.f);

	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Euler Angles
	float yaw = YAW;
	float pitch = PITCH;

	// Camera options
	float movementSpeed = SPEED;
	float sensitivity = SENSITIVITY;
	float zoom = ZOOM;
};
