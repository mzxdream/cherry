#ifndef __CHERRY_SCENE_GRID_VIEW_H__
#define __CHERRY_SCENE_GRID_VIEW_H__

#include <cstdint>
#include <map>
#include <mzx/util.h>
#include <set>
#include <unordered_map>

namespace mzx
{

class Entity;

}

namespace cherry
{

struct SceneGridView
{
    explicit SceneGridView(double size)
        : grid_size(size)
    {
    }
    double grid_size{0};
    std::unordered_map<std::pair<int, int>, std::set<mzx::Entity *>,
                       mzx::PairHash>
        grid_list;
};

} // namespace cherry

#endif
