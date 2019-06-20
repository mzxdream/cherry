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

struct ViewPublish
{
    std::set<mzx::Entity *> subscribe_list;
};

struct ViewSubscribe
{
    std::set<mzx::Entity *> publish_list;
};

} // namespace cherry

#endif
