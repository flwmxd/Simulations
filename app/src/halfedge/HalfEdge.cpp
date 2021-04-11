#include "HalfEdge.h"


HalfEdge::HalfEdge(int32_t start,int32_t end, int32_t faceId)
    :start(start),end(end), faceId(faceId)
{

}

auto HalfEdge::setOpposite(int32_t edge) -> void
{
    opposite = edge;
}


auto HalfEdge::setVertex(int32_t start,int32_t end) -> void
{
    this->start = start;
    this->end = end;
}