#ifndef __CHERRY_ENTITY_SYSTEM_H__
#define __CHERRY_ENTITY_SYSTEM_H__

namespace cherry {

class World;

class EntitySystem
{
public:
    virtual bool Init(World *world) { return true; }
    virtual void Uninit(World *world) {}
    virtual void Update(World *world, int64_t delta_time) {}
};

}

#endif
