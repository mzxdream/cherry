#include "scene/world_scene.h"

namespace cherry {

WorldScene::WorldScene(SceneID id, SceneUUID uuid)
    : Scene(id, uuid)
{

}

WorldScene::~WorldScene()
{

}

bool WorldScene::_Init()
{
    //system_manager_.AddSystem<CmdHandleSystem>(this);
    GetSystemManager().Configure();
    return true;
}

void WorldScene::_Uninit()
{
    GetSystemManager().Unconfigure();
}

void WorldScene::_Update(int64_t delta_time)
{
    GetSystemManager().Update(delta_time);
}

}
