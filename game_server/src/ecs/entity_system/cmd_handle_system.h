#ifndef __CHERRY_CMD_HANDLE_SYSTEM_H__
#define __CHERRY_CMD_HANDLE_SYSTEM_H__

#include "ecs/system/entity_system.h"

namespace cherry {

class CmdHandleSystem
    : public EntitySystem
{
    CmdHandleSystem();
    virtual ~CmdHandleSystem();
public:
    virtual bool Init(World *world) override;
    virtual void Uninit(World *world) override;
    virtual void Update(World *world, int64_t delta_time) override;
public:
    void OnReceiveCmd();
private:

};

}

#endif
