//////////////////////////////////////////////////////////////////////////////
// This file is part of the FFD Course work									//
// Author Prime Zeng														//
//////////////////////////////////////////////////////////////////////////////

#include "Window.h"
#include <iostream>
#include "Application.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

auto Window::init(int32_t width, int32_t height) -> void
{
	this->width = width;
	this->height = height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	 window = glfwCreateWindow(width, height, "FFD", nullptr, nullptr);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glewInit();
	glfwSetWindowUserPointer(window, this);

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	setEventCallback();
}

auto Window::update() -> void
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

auto Window::setEventCallback() -> void
{
	glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int32_t w, int32_t h) {
		glViewport(0, 0, w, h);
		auto window = (Window*)glfwGetWindowUserPointer(win);
		window->setWidthAndHeight(w, h);
	});


	glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
		auto event = std::make_unique<MouseMoveEvent>();
		auto window = (Window*)glfwGetWindowUserPointer(win);
		y = window->getHeight() - y;
		event->position = glm::vec2{ x,y };
		app->getEventDispatcher().postEvent(std::move(event));
	});

	glfwSetScrollCallback(window, [](GLFWwindow* win, double x, double y) {
		app->getEventDispatcher().postEvent(std::make_unique<MouseScrolledEvent>(x, y));
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
		double x, y;
		glfwGetCursorPos(win, &x, &y);
		auto window = (Window*)glfwGetWindowUserPointer(win);
		y = window->getHeight()  - y;

		if (action == GLFW_PRESS) {
			auto event = std::make_unique<MouseClickEvent>();
			event->position = glm::vec2{ x,y };
			event->buttonId = button;
			app->getEventDispatcher().postEvent(std::move(event));
		}
		else
		{
			auto event = std::make_unique<MouseReleaseEvent>();
			event->position = glm::vec2{ x,y };
			event->buttonId = button;
			app->getEventDispatcher().postEvent(std::move(event));
		}
	});

	glfwSetKeyCallback(window, [](GLFWwindow*, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
		switch (action)
		{
		case GLFW_PRESS:
		{
			app->getEventDispatcher().postEvent(std::make_unique <KeyPressedEvent>(static_cast<KeyCode>(key), 0));
			break;
		}
		case GLFW_RELEASE:
		{
			app->getEventDispatcher().postEvent(std::make_unique <KeyReleasedEvent>(static_cast<KeyCode>(key)));
			break;
		}
		case GLFW_REPEAT:
		{
			app->getEventDispatcher().postEvent(std::make_unique <KeyPressedEvent>(static_cast<KeyCode>(key), 1));
			break;
		}
		}
	});

}

auto Window::setWidthAndHeight(int32_t width, int32_t height) -> void
{
	this->width = width;
	this->height = height;
}
