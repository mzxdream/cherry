#ifndef __CHERRY_SCENE_GRID_VIEW_H__
#define __CHERRY_SCENE_GRID_VIEW_H__

#include <cstdint>
#include <map>
#include <set>

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
    std::map<uint32_t, std::set<mzx::Entity *>> grid_list;
    std::map<mzx::Entity *, uint32_t> entity_grid_index;
    std::set<mzx::Entity *> publish_add_list;
    std::set<mzx::Entity *> subscribe_add_list;
    std::set<mzx::Entity *> position_change_list;
};

} // namespace cherry

#endif
