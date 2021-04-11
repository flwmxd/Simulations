

#pragma once
#include "engine/Layer.h"
#include "Skeleton.h"
#include "event/EventHandler.h"
#include "engine/Camera.h"
#include "Sphere.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Floor.h"
#include "Mesh.h"
#include "Sphere.h"
#include <string>
#include <imgui.h>
#include <imfilebrowser.h>
#include "opengl/Light.h"
#include "mass/Cloth.h"




class ModelLayerCloth : public Layer
{
public:
	friend class ImLayer;
	virtual auto init() -> void override;
	virtual auto draw(const Timestep& timestep) -> void override;
	auto loadModel(const std::string& name) -> void;
	auto onImGui() -> void override;
	auto onImGuiEnd() -> void override;
	
private:
	std::unique_ptr<opengl::Shader> shader;
	std::unique_ptr <Camera> camera;
	Floor floor;
	glm::mat4 projection;
	glm::mat4 view;
	glm::vec3 cameraPos;
	EventHandler handler;
	ImGui::FileBrowser dialog;

	bool mouseClicked = false;
	glm::vec2 lastPosition;

	std::vector<std::shared_ptr<Mesh>> meshes;

	std::shared_ptr<Cloth> cloth;
	std::shared_ptr<Sphere> sphere;


	OGLTexture defaultTexture;

	Material material;
	Light light;
	glm::vec3 clothPosition;

}; 