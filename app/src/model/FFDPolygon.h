#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "Point.h"
#include "engine/RenderObject.h"
#include "engine/Randomizer.h"
#include <vector2.h>
#include <triangle.h>
#include <delaunay.h>

class ModelLayer2D;

enum class CageType
{
	Grid,
	Triangle,
};


struct FFDTriangle 
{
	ControlPoint* a;
	ControlPoint* b;
	ControlPoint* c;
};

class FFDPolygon : public RenderObject
{
public:
	friend class ImLayer;
	FFDPolygon();
	auto generateRandom() -> void;
	auto draw(opengl::Shader* shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, BatchCommand& command) -> void override;
	virtual auto onMouseMove(MouseMoveEvent* event) -> bool override;
	virtual auto onMouseClick(MouseClickEvent* event) -> bool override;
	virtual auto onMouseRelease(MouseReleaseEvent* event) -> bool override;

	inline auto& getControlPoints() { return controlPoints; }
	inline auto isActive() const { return active; }
	auto setCell(int32_t cell) -> void;
	auto deform() -> void;
	auto attenuate(const glm::vec3 & delta) -> void;
	auto reset() -> void;
	auto genTriangleCage(int32_t vertexCount, std::vector<RenderObject* >& lines)-> void;
	auto attenuateOthers(int32_t i,ControlPoint * point, const glm::vec3& delta) -> void;
	auto save(const std::string & name) -> void;
	auto loadFile(const std::string& name) -> void;
	auto setParent(ModelLayer2D* parent) -> void;
private:
	//judge whether a triangle contains a point
	auto contains(FFDTriangle& triangle, const glm::vec2& p) -> bool;

	Randomizer rand;
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> originVertices;
	glm::vec2 position;
	glm::vec2 origin;
	std::vector<ControlPoint> controlPoints;
	bool active = true;
	bool mouseDown = false;
	bool attenuation = false;
	float attenuateK = 1;

	glm::vec2 lastPosition = { 0,0 };
	int32_t cell = 0;
	CageType type = CageType::Grid;
	dt::Delaunay<float> triangulation;
	std::vector<FFDTriangle> triangles;
	std::vector<uint32_t> indexBuffer;

	ControlPoint* current = nullptr;
	ModelLayer2D * parent = nullptr;
};