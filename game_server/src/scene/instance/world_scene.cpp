#include <ecs/component/single/scene_grid_view.h>
#include <ecs/system/move_system.h>
#include <ecs/system/scene_grid_view_system.h>
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
    GetSystemManager().AddSystem<MoveSystem>();
    GetSystemManager().AddSystem<SceneGridViewSystem>(this);
    return true;
}

void WorldScene::_Uninit()
{
}

void WorldScene::_Update()
{
    GetSystemManager().Update<MoveSystem>(this);
}

} // namespace cherry
