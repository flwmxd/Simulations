#pragma once
#include <functional>
#include <set>

class HalfEdge;

class HalfEdgeVertex final
{
public:
    HalfEdgeVertex() = delete;

    HalfEdgeVertex(float x,float y,float z,int32_t i = 0)
    {
        value[0] = x;
        value[1] = y;
        value[2] = z;
        index = i;
    }

    inline auto operator==(const HalfEdgeVertex & v) -> bool
    {
        return v.value[0] == value[0] && v.value[1] == value[1] && v.value[2] == value[2];
    }    
    
    inline auto operator==(const HalfEdgeVertex & v) const -> bool
    {
        return v.value[0] == value[0] && v.value[1] == value[1] && v.value[2] == value[2];
    }

    float value[3] = {-1,-1,-1};
    float index = 0;
    std::vector<int32_t> faces;
    std::set<int32_t> neighbors;
};

namespace std
{
	template<> struct hash<HalfEdgeVertex>
	{
		typedef HalfEdgeVertex argument_type;
		typedef std::size_t result_type;
		size_t operator()(const HalfEdgeVertex& s) const
		{
			size_t h1 = std::hash<float>()(s.value[0]);
			size_t h2 = std::hash<float>()(s.value[1]);
			return h1 ^ (h2 << 1);
		}
	};
}
