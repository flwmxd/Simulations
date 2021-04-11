
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"
#include <cmath>

Camera::Camera(const glm::mat4& initProjection, const glm::vec3& initPosition /*= { 0, 0, 0 }*/, const glm::vec3& up /*= { 0,1,0 }*/, float yaw /*= YAW*/, float pitch /*= PITCH*/)
	:projection(initProjection),position(initPosition),up(up),yaw(yaw),pitch(pitch)
{
	worldUp = up;
	updateCameraVectors();
}

auto Camera::getViewMatrix() const->glm::mat4
{
	return glm::lookAt(position, position + front,up);
}

auto Camera::updateCameraVectors() -> void
{
	front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	front.y = std::sin(glm::radians(pitch));
	front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	front = glm::normalize(front);
	right = glm::normalize(glm::cross(front, worldUp));  
	up = glm::normalize(glm::cross(right, front));
}

auto Camera::onMouseScrolled(float yOffset) -> void
{
	zoom -= yOffset / 20.f;
	if (zoom <= 1.0f)
		zoom = 1.0f;

	updateProjection();
}

auto Camera::onMouseMove(float x, float y, bool constrainPitch /*= true*/) -> void
{
	x *= sensitivity;
	y *= sensitivity;

	yaw -= x;
	pitch -= y;

	if (constrainPitch)
	{
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
	updateCameraVectors();
}

auto Camera::onKeyDown(KeyCode keyCode) -> void
{
	float velocity = movementSpeed / 100.f;
	if (keyCode == KeyCode::W)
		position += front * velocity;
	if (keyCode == KeyCode::S)
		position -= front * velocity;
	if (keyCode == KeyCode::A)
		position -= right * velocity;
	if (keyCode == KeyCode::D)
		position += right * velocity;
}

auto Camera::updateProjection() -> void
{
	projection = glm::perspective(glm::radians(zoom), (float)app->getWindow().getWidth() / (float)app->getWindow().getHeight(), 0.1f, 10000.0f);
}
