
#pragma once

#include "Layer.h"
#include "event/EventHandler.h"
#include <bvh11.hpp>
#include <imgui.h>
#include <imfilebrowser.h>

class Model;
class ModelLayer;
class ModelLayer2D;
class ModelLayerBVH;
class ModelLayerCloth;
class ModelLayerFluid;
class FFDPolygon;
class ControlPoint;


class ImLayer : public Layer
{
public:
	ImLayer();
	auto init() -> void;
	auto draw(const Timestep & step) -> void override;
	auto setCurrentPoint(ControlPoint* point) -> void;
	auto setModel(Model* model) -> void;
	auto setPolygon(FFDPolygon* model) -> void;
	auto setModelLayer(ModelLayer* layer) -> void;
	auto setModelLayer2D(ModelLayer2D* layer) -> void;
	auto setModelLayerBVH(ModelLayerBVH* layer) -> void;
	auto setModelLayerCloth(ModelLayerCloth* layer) -> void;
	auto setModelLayerFluid(ModelLayerFluid* layer) -> void;

	auto mainMenuBar() -> void;
private:
	auto drawIKFunction() -> void ;
	auto recursiveDraw(std::shared_ptr<bvh11::Joint>& jonit) -> void;
	EventHandler handler;
	ControlPoint * point = nullptr;
	Model* model = nullptr;
	FFDPolygon* polygon;
	ModelLayer * layer = nullptr;
	ModelLayer2D * layer2D = nullptr;
	ModelLayerBVH* layerBVH = nullptr;
	ModelLayerCloth* layerCloth = nullptr;
	ModelLayerFluid* layerFluid = nullptr;

	std::shared_ptr<bvh11::Joint> lastJoint;
	ImGui::FileBrowser dialog;


	template <typename... Args>
	auto format(const std::string& format, Args... args) -> std::string
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args...);
		return std::string(buf.get(), buf.get() + size - 1);
	}

};