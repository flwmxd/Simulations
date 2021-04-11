#pragma once
#include <functional>
#include <memory> 


class HalfEdge;


class Face final
{
public:
    Face(int32_t first,int32_t second,int32_t third);
    inline auto getFirst() const -> int32_t { return first; }
    inline auto getSecond() const -> int32_t { return second;}
    inline auto getThird() const -> int32_t { return third; }
private:
    int32_t first;
    int32_t second;
    int32_t third;
};
