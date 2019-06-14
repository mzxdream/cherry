#ifndef __CHERRY_CMD_HANDLER_H__
#define __CHERRY_CMD_HANDLER_H__

#include <mzx/singleton.h>
#include <mzx/system/cmd_line.h>
#include <scene/scene.h>

namespace cherry
{

class CmdHandler : public mzx::Singleton<CmdHandler>
{
    friend mzx::Singleton<CmdHandler>;

private:
    CmdHandler();
    ~CmdHandler();

public:
    Scene *GetSelectScene();
    void SetSelectSceneUUID(SceneUUID uuid);
    bool Start();
    void Stop();
    bool Regist(const char *name,
                std::function<void(const std::vector<std::string> &)> cb);

private:
    SceneUUID select_scene_uuid_{SCENE_UUID_INVALID};
};

#define CHERRY_CMD_REGIST(name, callback)                                      \
    class CmdRegister##name                                                    \
    {                                                                          \
        const static bool is_regist_;                                          \
    };                                                                         \
    const bool CmdRegister##name::is_regist_ =                                 \
        CmdHandler::Instance().Regist(#name, callback);

} // namespace cherry

#endif
