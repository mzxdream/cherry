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
    return true;
}

void WorldScene::_Uninit()
{

}

void WorldScene::_Update(int64_t delta_time)
{

}

}
