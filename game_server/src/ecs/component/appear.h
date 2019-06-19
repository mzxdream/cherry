#ifndef __CHERRY_APPEAR_H__
#define __CHERRY_APPEAR_H__

#include <cstdint>
#include <set>

namespace mzx
{

class Entity;

}

namespace cherry 
{

struct Appear
{
    explicit Appear()
    {
    }
    std::set<mzx::Entity *> follower_list;
};

} // namespace Appear

#endif
