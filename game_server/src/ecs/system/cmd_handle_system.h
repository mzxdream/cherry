#ifndef __CHERRY_CMD_HANDLE_SYSTEM_H__
#define __CHERRY_CMD_HANDLE_SYSTEM_H__

#include <mzx/ecs.h>
#include <mzx/simple_event.h>

namespace cherry
{

class CmdHandleSystem : public mzx::EntitySystem<CmdHandleSystem>
{
public:
    CmdHandleSystem();
    virtual ~CmdHandleSystem();
    CmdHandleSystem(const CmdHandleSystem &) = delete;
    CmdHandleSystem &operator=(const CmdHandleSystem &) = delete;

private:
    virtual bool _Init() override;
    virtual void _Uninit() override;
    virtual void _Update() override;

public:
    void OnRecvCmd(const mzx::SimpleEventBase *event);
};

} // namespace cherry

#endif
