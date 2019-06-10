#ifndef __CHERRY_WORLD_SCENE_H__
#define __CHERRY_WORLD_SCENE_H__

#include <scene/scene.h>

namespace cherry
{

class WorldScene : public Scene
{
public:
    WorldScene(SceneUUID uuid);
    virtual ~WorldScene();
    WorldScene(const WorldScene &) = delete;
    WorldScene &operator=(const WorldScene &) = delete;

private:
    virtual bool _Init() override;
    virtual void _Uninit() override;
    virtual void _Update() override;

private:
};

} // namespace cherry

#endif
