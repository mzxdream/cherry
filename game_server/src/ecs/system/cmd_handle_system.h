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
    CmdHandleSystem(const CmdHandleSystem &) = delete;
    CmdHandleSystem & operator=(const CmdHandleSystem &) = delete;
private:
    virtual bool _Init() override;
    virtual void _Uninit() override;
    virtual void _Update(int64_t delta_time) override;
    virtual bool _Configure() override;
    virtual void _Unconfigure() override;
public:
    void OnRecvCmd(const Event *event);
private:
    World *world_;
    std::list<std::string> cmd_list_;
    mzx::EventID cmd_event_id_;
};

}

#endif
