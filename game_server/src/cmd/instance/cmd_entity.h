#ifndef __CHERRY_CMD_ENTITY_H__
#define __CHERRY_CMD_ENTITY_H__

#include <cmd/cmd_handler.h>

namespace cherry
{

void HandleAddEntity(const std::vector<std::string> &cmd);
void HandleRemoveEntity(const std::vector<std::string> &cmd);
void HandleListEntity(const std::vector<std::string> &cmd);
void HandleInfoEntity(const std::vector<std::string> &cmd);
void HandleAddComponent(const std::vector<std::string> &cmd);

} // namespace cherry

#endif
