

#pragma once
#include <memory> 

class Edge;
class Face;

class HalfEdge
{
public:
    HalfEdge(int32_t start,int32_t end,int32_t faceId);
    HalfEdge() = default;
    inline auto getOpposite() const{ return opposite; }
    inline auto getStart() const { return start; }
    inline auto getEnd() const  { return end; }
    inline auto getFaceId() const{ return faceId; }

    auto setOpposite(int32_t edge) -> void;
    auto setVertex(int32_t start,int32_t end) -> void;
    
private:
	int32_t start = -1;
	int32_t end = -1;
    int32_t opposite = -1;
    int32_t faceId = -1;

};