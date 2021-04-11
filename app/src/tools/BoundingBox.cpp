
#include "BoundingBox.h"

auto BoundingBox::define(const std::vector<Vertex>& vertex) -> void
{
	if (vertex.size() > 0) {
		merge(vertex);
	}
}

auto BoundingBox::merge(const std::vector<Vertex>& vertex) -> void
{
	for (auto & v:vertex)
	{
		merge(v);
	}
}

auto BoundingBox::merge(const Vertex& v) -> void
{
	if (v.pos.x < minValue.x)
		minValue.x = v.pos.x;
	if (v.pos.y < minValue.y)
		minValue.y = v.pos.y;
	if (v.pos.z < minValue.z)
		minValue.z = v.pos.z;
	if (v.pos.x > maxValue.x)
		maxValue.x = v.pos.x;
	if (v.pos.y > maxValue.y)
		maxValue.y = v.pos.y;
	if (v.pos.z > maxValue.z)
		maxValue.z = v.pos.z;
}

auto BoundingBox::clear() -> void
{
	minValue = { M_INFINITY, M_INFINITY, M_INFINITY };
	maxValue = { -M_INFINITY, -M_INFINITY, -M_INFINITY };
}

