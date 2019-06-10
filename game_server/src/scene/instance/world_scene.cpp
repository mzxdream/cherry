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
    return true;
}

void WorldScene::_Uninit()
{
}

void WorldScene::_Update()
{
}

} // namespace cherry
