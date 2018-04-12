#ifndef __CHERRY_CMD_HANDLE_SYSTEM_H__
#define __CHERRY_CMD_HANDLE_SYSTEM_H__

#include <mzx/ecs.h>
#include <mzx/event.h>

namespace cherry {

class World;
class Event;

class CmdHandleSystem
    : public mzx::EntitySystem<CmdHandleSystem>
{
public:
    CmdHandleSystem(World *world);
    virtual ~CmdHandleSystem();
public:
    virtual bool Init() override;
    virtual void Uninit() override;
    virtual void Update(int64_t delta_time) override;
public:
    void OnRecvCmd(const Event *event);
private:
    World *world_;
    std::list<std::string> cmd_list_;
    mzx::EventID cmd_event_id_;
};

}

#endif
