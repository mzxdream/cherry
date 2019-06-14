#ifndef __CHERRY_CMD_SCENE_H__
#define __CHERRY_CMD_SCENE_H__

#include <cmd/cmd_handler.h>

namespace cherry
{

class Scene;

void HandleCreateWorldScene(const std::vector<std::string> &cmd);
void HandleDestroyScene(const std::vector<std::string> &cmd);
void HandleListScene(const std::vector<std::string> &cmd);
void HandleSelectScene(const std::vector<std::string> &cmd);
Scene *GetSelectScene();

} // namespace cherry

#endif
