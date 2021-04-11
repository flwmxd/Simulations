#pragma once
#include <memory>

class Edge final
{
public:
    Edge(int32_t start,int32_t end);
    inline auto getStart() const -> int32_t { return start; }
    inline auto getEnd()  const -> int32_t  { return end; }
    auto operator== (const Edge & edge) -> bool{ return start == edge.start && end == edge.end; }
    auto operator== (const Edge & edge) const -> bool{ return start == edge.start && end == edge.end; }
private:
    int32_t start;
    int32_t end;
};


namespace std
{
    template<> struct hash<Edge>
    {
        typedef Edge argument_type;
        typedef std::size_t result_type;
        size_t operator()(const Edge &s) const
        {
            size_t h1 = std::hash<size_t>()(s.getStart());
            size_t h2 = std::hash<size_t>()(s.getEnd());
            
            if(s.getStart() > s.getEnd()){
                std::swap(h1,h2);
            }
            return h1 ^ (h2 << 1);
        }
    };
}
 