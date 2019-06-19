#ifndef __CHERRY_VIEW_H__
#define __CHERRY_VIEW_H__

#include <cstdint>
#include <set>

namespace mzx
{

class Entity;

}

namespace cherry
{

struct View
{
    explicit View()
    {
    }
    std::set<mzx::Entity *> see_list;
};

} // namespace cherry

#endif
