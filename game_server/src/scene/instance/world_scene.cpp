#include <ecs/component/single/scene_grid_view.h>
#include <ecs/system/hatred_system.h>
#include <ecs/system/move_system.h>
#include <ecs/system/scene_grid_view_system.h>
#include <ecs/system/show_system.h>
#include <scene/instance/world_scene.h>

namespace cherry
{

WorldScene::WorldScene(SceneUUID uuid)
    : Scene(uuid)
{
}

WorldScene::~WorldScene()
{
}

bool WorldScene::_Init()
{
    GetSystemManager().AddSystem<ShowSystem>(this);
    GetSystemManager().AddSystem<MoveSystem>();
    GetSystemManager().AddSystem<SceneGridViewSystem>(this);
    GetSystemManager().AddSystem<HatredSystem>(this);
    return true;
}

void WorldScene::_Uninit()
{
}

void WorldScene::_Update()
{
    GetSystemManager().Update<MoveSystem>(this);
    GetSystemManager().Update<SceneGridViewSystem>(this);
    GetSystemManager().Update<HatredSystem>(this);
}

} // namespace cherry
