
#include "ImLayer.h"
#include <imgui.h>


#include "Application.h"
#include "model/Model.h"
#include "model/FFDPolygon.h"
#include "model/ModelLayer.h"
#include "model/ModelLayer2D.h"
#include "model/ModelLayerBVH.h"
#include "model/ModelLayerCloth.h"

#include "ik/IkEffector.h"


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

ImLayer::ImLayer()
	:handler(EventHandler::PRIORITY_MAX)
{
}

auto ImLayer::init() -> void
{

	ImGuiIO& io = ImGui::GetIO();
	//ImGui::StyleColorsDark();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	dialog.SetTitle("Choose 3d Obj File");
	dialog.SetTypeFilters({ ".obj" ,".ffd",".bvh"});
	dialog.SetPwd("./models");

	app->getEventDispatcher().addEventHandler(&handler);

	handler.mouseClickHandler = [](auto event) {
		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureMouse;
	};
	handler.mouseMoveHandler = [](auto event) {
		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureMouse;
	};
	handler.mouseRelaseHandler = [](auto event) {
		ImGuiIO& io = ImGui::GetIO();
		return io.WantCaptureMouse;
	};
}
auto ImLayer::mainMenuBar() -> void
{

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("(Animation Course Work)", NULL, false, false);
			if (ImGui::MenuItem("Open", "Choose a .obj file")) {
				dialog.Open();
			}

			if (ImGui::MenuItem("Open ffd2d", "Choose a .ffd file")) {
				dialog.Open();
			}

			if (ImGui::MenuItem("OpenBVH", "Choose a .bvh file")) {
				dialog.Open();
			}

			ImGui::EndMenu();
		}
	
		ImGui::EndMainMenuBar();
	}
}
auto ImLayer::drawIKFunction() -> void
{
	if (layerBVH->getSkeleton() != nullptr) 
	{
	
		if (lastJoint != nullptr) 
		{
			/*auto& off = lastJoint->offset();
			float offset[3] = { off.x(),off.y(),off.z() };
			if (ImGui::DragFloat3("Offset", offset, 0.01)) 
			{
				off = { offset[0],offset[1],offset[2] };
			}*/
		}
		auto& obj = layerBVH->getSkeleton()->getBvhObject();
		recursiveDraw(obj.root_joint());
	}
}


auto ImLayer::recursiveDraw(std::shared_ptr<bvh11::Joint> & jonit) -> void
{
	ImGui::SetNextTreeNodeOpen(true);
	if (jonit != nullptr)
	{
		if (jonit->children().size() == 0)
		{
			if (ImGui::Selectable(jonit->name().c_str()))
			{
				if (lastJoint != nullptr)
				{
					lastJoint->setSelected(false);
				}
				jonit->setSelected(true);
				lastJoint = jonit;
			}
		
		}
		else 
		{
			if (ImGui::TreeNodeEx(format("%s", jonit->name().c_str()).c_str(), ImGuiTreeNodeFlags_Selected)) {
				int32_t i = 0;
				if (ImGui::IsItemClicked())
				{
					if (lastJoint != nullptr)
					{
						lastJoint->setSelected(false);
					}
					jonit->setSelected(true);
					lastJoint = jonit;
				}
				for (auto & j : jonit->children())
				{
					recursiveDraw(j);
				}
				ImGui::TreePop();
			}
		}
	}
}


auto ImLayer::draw(const Timestep& step) -> void
{

	//mainMenuBar();
	ImGui::Begin("Settings");
	static bool frame = true;
	if (ImGui::Checkbox("WireFrame", &frame)) 
	{
		app->getRenderer().setWireFrame(frame);
	}

	ImGui::Checkbox("FFD 2D", &layer2D->active);
	ImGui::Checkbox("FFD 3D", &layer->active);

	ImGui::Separator();


	if (layer->active) 
	{
		
		if (ImGui::InputInt("order of cage", &model->order)) {
			model->setOrder(model->order,layer);
		}

		if (point != nullptr)
		{
			if (ImGui::DragFloat3("position", &point->point[0], 0.001)) 
			{
				model->deform();
			}
		}
	}

	if (layer2D->active) {
		static int32_t cell = layer2D->cell;

		if (polygon != nullptr)
		{
			
			if (ImGui::Button("random polygon"))
			{
				polygon->generateRandom();
			}

			if (ImGui::Button("reset polygon"))
			{
				polygon->reset();
			}	
			

			if (ImGui::RadioButton("Grid Cage", (int32_t*)(&polygon->type), (int32_t)CageType::Grid)) 
			{
				layer2D->setCell(cell);
			}
			
			if (ImGui::RadioButton("Triangle Cage", (int32_t*)(&polygon->type), (int32_t)CageType::Triangle)) 
			{
				layer2D->setTriangleCage(layer2D->vertexCount);
			}

			if (polygon->type == CageType::Grid) 
			{
				if (ImGui::InputInt("gird cage set cells", &cell))
				{
					layer2D->setCell(cell);
				}
			}
			else if (polygon->type == CageType::Triangle) 
			{
				if (ImGui::InputInt("triangle cage", &layer2D->vertexCount))
				{
					layer2D->setTriangleCage(layer2D->vertexCount);
				}
			}

			ImGui::Checkbox("attenuation", &polygon->attenuation);
			if (polygon->attenuation) {
				ImGui::DragFloat("K of attenuation", &polygon->attenuateK,0.001);
			}

			ImGui::Checkbox(polygon->active ? "hide polygon" : "show polygon", &polygon->active);


			if (ImGui::Button("save current model")) 
			{
				ImGui::OpenPopup("Input you file name");

				// Always center this window when appearing
				ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
			}

			if (ImGui::BeginPopupModal("Input you file name", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{

				static char buff[128] = {};
				ImGui::InputText("filename", buff, 128);

				if (ImGui::Button("OK", ImVec2(120, 0))) {
					polygon->save(buff);
					ImGui::CloseCurrentPopup();
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) {
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}

		}
	}
	
	ImGui::Separator();

	ImGui::Checkbox("InverseKinematics", &layerBVH->active); 
	ImGui::SameLine();
	if (ImGui::Button("Load BVH File")) {
		dialog.Open();
	}
	ImGui::SameLine();

	static const char* dialogName = "Input you file name";
	if (ImGui::Button("Save BVH File")) {
		//TODO
	
		if (layerBVH->skeleton != nullptr) 
		{

			ImGui::OpenPopup(dialogName);
			// Always center this window when appearing
			ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
		}
	
	}


	if (ImGui::BeginPopupModal(dialogName, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{

		static char buff[128] = {};
		ImGui::InputText("fileName", buff, 128);

		if (ImGui::Button("OK", ImVec2(120, 0))) {
			layerBVH->skeleton->getBvhObject().WriteBvhFile(buff);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	if (layerBVH->active && layerBVH->getSkeleton()) {


		if (ImGui::TreeNode("root")) {
			drawIKFunction();
			ImGui::TreePop();
		}

		ImGui::SliderInt("Frame", &layerBVH->getSkeleton()->frame, 0, layerBVH->getSkeleton()->getBvhObject().frames() - 1);
		ImGui::SameLine();

		if (ImGui::SmallButton(layerBVH->getSkeleton()->pause ? "Play" : "Pause"))
		{
			layerBVH->getSkeleton()->pause = !layerBVH->getSkeleton()->pause;
		}

		ImGui::Separator();


		if (lastJoint != nullptr) {


			auto euler = layerBVH->getSkeleton()->getInverseKinematics()->getLocalEulerAngle(lastJoint, layerBVH->getSkeleton()->frame);
			float euOffset[3] = { euler.x,euler.y,euler.z };
			ImGui::SliderFloat3("Rotation", euOffset, -180, 180);
			ImGui::Separator();
			auto off = layerBVH->getSkeleton()->getInverseKinematics()->getTranslation(lastJoint, layerBVH->getSkeleton()->frame);
			float offset[3] = { off.x,off.y,off.z };
			if (ImGui::DragFloat3("Translation", offset, 0.05))
			{
				layerBVH->getSkeleton()->computeIKWithSingleJoint({
						offset[0] - off.x,
						offset[1] - off.y,
						offset[2] - off.z
					}, lastJoint);
				ImGui::Separator();
			}

			static bool useEffector = false;



			ImGui::InputFloat("tolerance", &layerBVH->getSkeleton()->getInverseKinematics()->tol);
			ImGui::InputFloat("delta ", &layerBVH->getSkeleton()->getInverseKinematics()->delta);

			ImGui::Separator();
			static bool damped = layerBVH->getSkeleton()->getInverseKinematics()->method == InverseKinematics::Jacobian::Damped;
			if (ImGui::Checkbox("Damped", &damped))
			{
				layerBVH->getSkeleton()->getInverseKinematics()->method = damped ? InverseKinematics::Jacobian::Damped : InverseKinematics::Jacobian::PseudoInverse;
			}

			if (damped)
			{
				ImGui::DragFloat("Damped Value ", &layerBVH->getSkeleton()->getInverseKinematics()->damping, 0.001);
			}

			ImGui::Checkbox("UseEffector", &useEffector); ImGui::SameLine(); ImGui::Checkbox("LockRoot", &layerBVH->getSkeleton()->getInverseKinematics()->lockRoot);
			ImGui::Separator();

			if (useEffector)
			{
				if (ImGui::SmallButton("Bind IK With Effector"))
				{
					layerBVH->getSkeleton()->bindJoint(lastJoint);
				}
				if (layerBVH->getSkeleton()->getInverseKinematics()->getEffector() != nullptr)
				{
					auto& off = layerBVH->getSkeleton()->getInverseKinematics()->getEffector()->position;
					float offset[3] = { off.x,off.y,off.z };
					if (ImGui::DragFloat3("Move Effector", offset, 0.01))
					{
						off = { offset[0],offset[1],offset[2] };
					}
				}
			}
			

		}
	}

	ImGui::Separator();

	ImGui::Checkbox("Cloth Simulation", &layerCloth->active);
	if (layerCloth != nullptr && layerCloth->active) {
		layerCloth->onImGui();
	}

	ImGui::Separator();

	ImGui::Checkbox("Fluid Simulation", &layerFluid->active);
	if (layerFluid != nullptr && layerFluid->active) {
		layerFluid->onImGui();
	}

	ImGui::End();
	if (layerCloth != nullptr && layerCloth->active) {
		layerCloth->onImGuiEnd();
	}
	dialog.Display();
	if (dialog.HasSelected())
	{
		auto file = dialog.GetSelected().string();
		std::cout << "Selected filename" << file << std::endl;
		dialog.ClearSelected();
		if (endsWith(file, ".ffd"))
		{
			if (layer2D != nullptr)
			{
				layer2D->loadModel(file);
			}
		}
		else if (endsWith(file, ".bvh"))
		{
			if (layerBVH != nullptr)
			{
				layerBVH->loadModel(file);
			}
		}
		else
		{
			if (layer != nullptr)
			{
				layer->loadModel(file);
			}
		}

	}
}

auto ImLayer::setCurrentPoint(ControlPoint* point) -> void
{
	this->point = point;
}

auto ImLayer::setModel(Model* model) -> void
{
	this->model = model;
}

auto ImLayer::setPolygon(FFDPolygon* model) -> void
{
	polygon = model;
}

auto ImLayer::setModelLayer(ModelLayer* layer) -> void
{
	this->layer = layer;
}

auto ImLayer::setModelLayer2D(ModelLayer2D* layer) -> void
{
	layer2D = layer;
}

auto ImLayer::setModelLayerBVH(ModelLayerBVH* layer) -> void
{
	layerBVH = layer;
}

auto ImLayer::setModelLayerCloth(ModelLayerCloth* layer) -> void
{
	layerCloth = layer;
}

auto ImLayer::setModelLayerFluid(ModelLayerFluid* layer) -> void
{
	layerFluid = layer;
}

