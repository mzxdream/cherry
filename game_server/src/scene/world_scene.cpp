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
    system_manager_.Configure();
    return true;
}

void WorldScene::_Uninit()
{
    system_manager_.Unconfigure();
}

void WorldScene::_Update(int64_t delta_time)
{
    system_manager_.Update(delta_time);
}

}
