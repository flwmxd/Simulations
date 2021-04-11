#include "ModelLayerCloth.h"
#include "Application.h"
#include "tools/ObjLoader.h"

#include <limits>

static bool endsWith(const std::string& str, const std::string& end)
{
	int srclen = str.size();
	int endlen = end.size();
	if (srclen >= endlen)
	{
		string temp = str.substr(srclen - endlen, endlen);
		if (temp == end)
			return true;
	}

	return false;
}

auto ModelLayerCloth::init() -> void 
{
	Layer::init();
	shader = std::make_unique<opengl::Shader>("shader/vertClothShader.glsl","shader/fragClothShader.glsl");
	shader->compile();

	projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.f, 100.0f);
	camera = std::make_unique<Camera>(projection, glm::vec3(0, 0, 3));

	handler.mouseScrollHandler = [&](auto event) {
		if(isActive())
			camera->onMouseScrolled(event->getYOffset());
		return isActive();
	};

	handler.mouseClickHandler = [&](auto event) {
		mouseClicked = true;
		lastPosition = event->position;
		return false;
	};

	handler.mouseRelaseHandler = [&](auto event) {
		mouseClicked = false;
		return false;
	};

	handler.mouseMoveHandler = [&](auto event) {
		if (isActive() && mouseClicked) {
			auto delta = event->position - lastPosition;
			camera->onMouseMove(delta.x, delta.y,false);
			lastPosition = event->position;
		}
		return isActive() && mouseClicked;
	};

	
	handler.keyPressedHandler = [&](auto event) {
		if (isActive()) {
			camera->onKeyDown(event->getKeyCode());
		}
		return false;
	};
	
	app->getEventDispatcher().addEventHandler(&handler);

	floor.initWthCloth();

	dialog.SetTypeFilters({ ".obj",".png" });

	int32_t value = INT_MAX;
	defaultTexture = {1,1,&value};

	light.lightDirection = { 0.0f, 1.f, 0.0f };
	light.lightColor = { 1.f, 1.f, 1.f };
	light.lightDiffuse = { 1.f, 0.7f, 0.7f };
	light.lightSpecular = { 1.0f, 1.0f, 1.0f };


	material.ambient = { 0.2f, 0.9f, 0.0f };
	material.diffuse = { 0.2f, 0.9f, 0.0f };
	material.specular = { 0.2f, 0.9f, 0.0f };
	material.shininess = 32.0f;

	sphere = std::make_shared<Sphere>("models/sphere.obj");
	cloth = std::make_shared<Cloth>("models/large.obj");
	
}

auto ModelLayerCloth::draw(const Timestep& timestep) -> void
{
	shader->useProgram();

	shader->setUniform3f("viewPos", glm::value_ptr(camera->getPosition()));

	shader->setUniform3f("light.position",glm::value_ptr(light.lightDirection));
	shader->setUniform3f("light.color", glm::value_ptr(light.lightColor));

	sphere->draw(shader.get(), glm::mat4(1), camera->getViewMatrix(), camera->getProjection());

	defaultTexture.bind();
	floor.draw(shader.get(),glm::mat4(1), camera->getViewMatrix(), camera->getProjection());
	

	shader->setUniform3f("material.ambient", glm::value_ptr(material.ambient));
	shader->setUniform3f("material.diffuse", glm::value_ptr(material.diffuse));
	shader->setUniform3f("material.specular", glm::value_ptr(material.specular)); // specular lighting doesn't have full effect on this object's material
	shader->setUniformValue("material.shininess", material.shininess);

	for (auto & mesh : meshes)
	{
		mesh->draw(shader.get(), glm::mat4(1), camera->getViewMatrix(), camera->getProjection());
	} 
	cloth->draw(shader.get(), glm::mat4(1), camera->getViewMatrix(), camera->getProjection());

	defaultTexture.unbind();



//===
	if (cloth->play) 
	{
		cloth->addForces();
		cloth->groundCollision(0);
		cloth->collisionWithMesh(sphere.get());

		cloth->calculateNextPosition();
		cloth->clearForces();

		cloth->computeNormals();
		cloth->generateBuffer();
	}

}

auto ModelLayerCloth::loadModel(const std::string& name) -> void
{
	meshes = ObjLoader::loadObj(name);
}

auto ModelLayerCloth::onImGui() -> void
{
/*
	if (ImGui::Button("Load Mesh")) {
		dialog.Open();
	}*/

	floor.onImGui();

	ImGui::DragFloat3("Light Position", &light.lightDirection[0],0.1);
	ImGui::ColorEdit3("Light Color", &light.lightColor[0]);


	ImGui::Separator();
	ImGui::Text("Mesh Material");
	ImGui::ColorEdit3("Mesh ambient", &material.ambient[0]);
	ImGui::ColorEdit3("Mesh diffuse", &material.diffuse[0]);
	ImGui::ColorEdit3("Mesh specular", &material.specular[0]);
	ImGui::DragFloat("Mesh shininess", &material.shininess, 0.01);


	ImGui::Separator();
	
	if (ImGui::Button("reload")) 
	{
		if (cloth != nullptr) {
			cloth = std::make_shared<Cloth>(cloth->getName());
		}
	}

	if (ImGui::Button("load from file"))
	{
		dialog.Open();
	}


	if (ImGui::Button(
		app->getRenderer().isRecording() ? "Stop Record" : "Start Record"
		)) 
	{
		app->getRenderer().captureScreen(!app->getRenderer().isRecording());
	}

	ImGui::Separator();
	sphere->onImGui();
	ImGui::Separator();
	cloth->onImGui();
}

auto ModelLayerCloth::onImGuiEnd() -> void 
{
	dialog.Display();
	if (dialog.HasSelected())
	{
		auto file = dialog.GetSelected().string();
		std::cout << "Selected filename" << file << std::endl;
		dialog.ClearSelected();
		if (endsWith(file, ".obj"))
		{
			cloth = std::make_shared<Cloth>(file);
		}
		else if (endsWith(file, ".png")) 
		{
			for (auto & me: meshes)
			{
				me->loadTexture(file);
			}
			if (cloth) 
			{
				cloth->loadTexture(file);
			}
		}
	}
}
