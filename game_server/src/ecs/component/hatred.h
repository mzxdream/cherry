#ifndef __CHERRY_HATRED_H__
#define __CHERRY_HATRED_H__

#include <algorithm>
#include <cstdint>
#include <map>
#include <set>

namespace mzx
{

class Entity;

}

namespace cherry
{

struct HatredPublish
{
    std::set<mzx::Entity *> subscribe_list;
};

struct HatredSubscribe
{
    using SortType =
        std::multimap<int64_t, mzx::Entity *, std::greater<int64_t>>;
    SortType publish_list;
    std::map<mzx::Entity *, SortType::iterator> entity_index;
};

struct HatredDistanceCheck
{
    explicit HatredDistanceCheck(double d)
        : distance(d)
    {
    }
    double distance;
};

} // namespace cherry

#endif
