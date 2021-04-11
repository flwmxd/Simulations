#include "FFDPolygon.h"
#include "Interpolation.h"
#include "Application.h"
#include "ModelLayer2D.h"



FFDPolygon::FFDPolygon()
{

}

auto FFDPolygon::generateRandom() -> void
{
	vertices.clear();
	indexBuffer.clear();

	auto size = rand.nextInt(3, 30);
	dt::Delaunay<float> triangulation;
	std::vector<dt::Vector2<float>> points;
	for (int i = 0; i < size; ++i) {
		auto x = rand.nextReal(-0.5, 0.5);
		auto y = rand.nextReal(-0.5, 0.5);
		points.emplace_back(x, y);
	}
	//generate triangles
	auto& triangles = triangulation.triangulate(points);
	auto& edge = triangulation.getEdges();
	auto& ver = triangulation.getVertices();

	vertices.resize(ver.size());
	for (auto & e : triangles)
	{
		for (int32_t i = 0;i< ver.size();i++)
		{
			vertices[i] = { ver[i].x,ver[i].y };
			if (*e.a == ver[i])
			{
				indexBuffer.emplace_back(i);
			}
			if (*e.b == ver[i])
			{
				indexBuffer.emplace_back(i);
			}
			if (*e.c == ver[i])
			{
				indexBuffer.emplace_back(i);
			}
		}
	}
	originVertices = vertices;
}

auto FFDPolygon::draw( opengl::Shader *shader, const glm::mat4& m, const glm::mat4& v, const glm::mat4& p, BatchCommand& command) -> void
{

	if (command.getType() == BatchCommand::Type::POINTS) 
	{
		for (auto & c : controlPoints)
		{
			c.draw(shader, m, v, p, command);
		}
	}
	else 
	{
		command.drawPolygon(vertices,indexBuffer, glm::mat4(1), glm::vec4{ 0,0,1,1 }, *shader);
	}
	
}

auto FFDPolygon::onMouseMove(MouseMoveEvent* event) -> bool 
{
	if (mouseDown) {
		auto delta = (lastPosition - event->position) / 300.f;
		lastPosition = event->position;
		if (current != nullptr)
		{
			current->setPosition(current->getPoint() - glm::vec3{ delta,0 });
			if (attenuation) {
				attenuate(glm::vec3{ delta,0 });
			}
			deform();
		}
	}

	for (auto& c : controlPoints)
	{
		if (c.onMouseMove(event)) {
			return true;
		}
	}

	return false;
}

auto FFDPolygon::onMouseClick(MouseClickEvent* event) -> bool 
{
	for (auto& c : controlPoints)
	{
		if (c.onMouseClick(event)) {
			mouseDown = true;
			lastPosition = event->position;
			if (current != nullptr) {
				current->setSelected(false);
			}
			app->getRenderer().getImLayer().setCurrentPoint(&c);
			current = &c;
			current->setSelected(true);
			return true;
		}
	}
	return false;
}

auto FFDPolygon::onMouseRelease(MouseReleaseEvent* event) -> bool 
{
	mouseDown = false;
	for (auto& c : controlPoints)
	{
		if (c.onMouseRelease(event)) {
			return true;
		}
	}

	return false;
}

auto FFDPolygon::setCell(int32_t cell) -> void
{
	current = nullptr;
	this->cell = cell;
	controlPoints.clear();
	const auto topLeft = glm::vec2{ -0.5,0.5 };
	const auto bottomRight = glm::vec2{ 0.5,-0.5 };
	auto step = (bottomRight - topLeft) / (float)(cell);
	for (auto i = 0; i <= cell; i++)
	{
		for (auto j = 0; j <= cell ; j++)
		{
			controlPoints.emplace_back(glm::vec3{ topLeft + glm::vec2{step.x * i, step.y * j} ,0 });
		}
	}

	int32_t xPoints = cell + 1;
	int32_t yPoints = cell + 1;

	for (auto i = 0; i < xPoints - 1; i++) {
		for (auto j = 0; j < yPoints; j++) {
			auto a = &getControlPoints()[j + (i * xPoints)];
			auto b = &getControlPoints()[j + ((i + 1) * yPoints)];
			parent->getLines().emplace_back(a);
			parent->getLines().emplace_back(b);
			a->setAdjacents(b);
			b->setAdjacents(a);
		}
	}


	for (auto i = 0; i < xPoints; i++) {
		for (auto j = 0; j < yPoints - 1; j++) {
			auto a = &getControlPoints()[j + (i * xPoints)];
			auto b = &getControlPoints()[j + 1 + (i * yPoints)];
			a->setAdjacents(b);
			b->setAdjacents(a);
			parent->getLines().emplace_back(a);
			parent->getLines().emplace_back(b);
		}
	}
}

//deform the 2d mesh
auto FFDPolygon::deform() -> void
{

	if (type == CageType::Grid) 
	{
		for (auto i = 0; i < vertices.size(); i++)
		{
			auto& a = originVertices[i];
			auto& v = vertices[i];

			auto x = Interpolation::convertPointToGrid(v.x, cell - 1);
			auto y = Interpolation::convertPointToGrid(-v.y, cell - 1);

			if (x > -1 && x < cell && y > -1 && y < cell)
				v = Interpolation::bilinearFilter(x, y, a.x, a.y, cell, controlPoints);
		}
	}

	else if(type == CageType::Triangle)
	{
		for (auto i = 0; i < vertices.size(); i++)
		{
			auto& a = originVertices[i];
			auto& v = vertices[i];

			for (auto & tran : triangles)
			{
				if (contains(tran, glm::vec2{ a.x,a.y })) {
					v = Interpolation::barycentricInterpolation(
						tran.a,
						tran.b,
						tran.c,
						glm::vec3(a,1)
					);
				}
			}
		}
	}
}

auto FFDPolygon::attenuate(const glm::vec3& delta) -> void
{
	auto& adj = current->getAdjacents();

	for (int32_t i =0;i<adj.size();i++)
	{
		attenuateOthers(i, adj[i], delta);
	}
}

auto FFDPolygon::reset() -> void
{
	vertices = originVertices;
	if (type == CageType::Grid) 
	{
		setCell(cell);
	}
	else 
	{
		for (auto& con : controlPoints)
		{
			con.reset();
		}
	}
}
//generate the triangle cage
auto FFDPolygon::genTriangleCage(int32_t vertexCount, std::vector<RenderObject* > & lines) -> void
{
	current = nullptr;
	controlPoints.clear();

	//generate random points
	std::vector<dt::Vector2<float>> points;
	for (int i = 0; i < vertexCount; ++i) {
		auto x = rand.nextReal(-0.5, 0.5);
		auto y = rand.nextReal(-0.5, 0.5);
		points.emplace_back(x,y);
	}
	//generate triangles
	const std::vector<dt::Triangle<float>> triangles = triangulation.triangulate(points);
	auto vertices = triangulation.getVertices();
	auto edge = triangulation.getEdges();
	for (auto & v : vertices)
	{
		controlPoints.emplace_back(glm::vec3{ v.x, v.y, 0 });
	}

	//convert them with control points
	for (auto & t : triangles)
	{
		FFDTriangle ffd;
		for (auto& con : controlPoints)
		{
			if (con == glm::vec2{ t.a->x, t.a->y }) 
			{
				ffd.a = &con;
			}
			if (con == glm::vec2{ t.b->x, t.b->y })
			{
				ffd.b = &con;
			}
			if (con == glm::vec2{ t.c->x, t.c->y })
			{
				ffd.c = &con;
			}
		}
		//add adjacents points;
		ffd.a->setAdjacents(ffd.b);
		ffd.a->setAdjacents(ffd.c);
		ffd.b->setAdjacents(ffd.a);
		ffd.b->setAdjacents(ffd.c);
		ffd.c->setAdjacents(ffd.a);
		ffd.c->setAdjacents(ffd.b);
		this->triangles.emplace_back(ffd);
	}

	//convert them with control points
	for (auto& e : edge) {
		for (auto & con : controlPoints)
		{
			if (con == glm::vec2{ e.v->x, e.v->y })
			{
				lines.emplace_back(&con);
			}
			if (con == glm::vec2{ e.w->x, e.w->y })
			{
				lines.emplace_back(&con);
			}
		}
	}
}


//The current i could not be the index of points;
//Could the i be a weight calculated by distance ?

auto FFDPolygon::attenuateOthers(int32_t ixx, ControlPoint* point, const glm::vec3 & delta) -> void
{
	
	auto i = glm::distance(point->getPoint(),current->getPoint());
	auto n = glm::distance(point->getPoint(), current->getOriginPoint()) + 1;

	float si = 1;
	if (attenuateK >= 0)
	{
		si = 1.f - std::pow((i+1) / (n), attenuateK + 1.f);
	}
	else 
	{
		si = std::pow(1.f - (i+1) / (n), -attenuateK + 1.f);
	}
	point->setPosition(point->getPoint() - delta * si);
}

auto FFDPolygon::save(const std::string& name) -> void
{

	if (name == "") {
		return ;
	}

	std::ofstream out(name + ".ffd");
	out << vertices.size() << std::endl;

	for (auto i = 0; i < vertices.size(); i++) {
		auto& v = vertices[i];
		out << "Vertex " << i << " " << v.x << " " << v.y << std::endl;
	}
	out << indexBuffer.size() << std::endl;;
	for (auto i = 0; i < indexBuffer.size(); i++) {
		auto& v = indexBuffer[i];
		out << "Index " << i << " " << v << std::endl;
	}

	out.flush();
	out.close();
}

auto FFDPolygon::loadFile(const std::string& name) -> void
{
	vertices.clear();
	indexBuffer.clear();

	std::ifstream in(name);
	int32_t vertexSize = 0;
	in >> vertexSize;
	
	for (auto i = 0; i < vertexSize; i++) {
		std::string type;
		int32_t index = 0;
		float x,y;
		in >> type >> index >> x >> y;
		vertices.emplace_back(x, y);
	}
	int32_t indexSize = 0;
	in >> indexSize;

	for (auto i = 0; i < indexSize; i++) {
		std::string type;
		int32_t index,index2;
		in >> type >> index >> index2;
		indexBuffer.emplace_back(index2);
	}

	in.close();

	originVertices = vertices;
}

auto FFDPolygon::setParent(ModelLayer2D* parent) -> void
{
	this->parent = parent;
}

//judge whether a triangle contains a point
auto FFDPolygon::contains(FFDTriangle& triangle, const glm::vec2& p) -> bool
{
	auto divisor = 
		(triangle.b->getOriginPoint().y - triangle.c->getOriginPoint().y) *
		(triangle.a->getOriginPoint().x - triangle.c->getOriginPoint().x) + (triangle.c->getOriginPoint().x - triangle.b->getOriginPoint().x) * 
		(triangle.a->getOriginPoint().y - triangle.c->getOriginPoint().y);

	auto a = ((triangle.b->getOriginPoint().y - triangle.c->getOriginPoint().y) * (p.x - triangle.c->getOriginPoint().x) + (triangle.c->getOriginPoint().x - triangle.b->getOriginPoint().x) * (p.y - triangle.c->getOriginPoint().y)) / divisor;
	auto b = ((triangle.c->getOriginPoint().y - triangle.a->getOriginPoint().y) * (p.x - triangle.c->getOriginPoint().x) + (triangle.a->getOriginPoint().x - triangle.c->getOriginPoint().x) * (p.y - triangle.c->getOriginPoint().y)) / divisor;
	auto c = 1 - a - b;
	return a >= 0 && a <= 1 && b >= 0 && b <= 1 && c >= 0 && c <= 1;
}

