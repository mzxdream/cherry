#ifndef __CHERRY_SESSION_H__
#define __CHERRY_SESSION_H__

#include <unordered_map>

namespace cherry
{

class Session final
{
};

class SessionManager final
{
public:
    SessionManager();
    ~SessionManager();
    SessionManager(const SessionManager &);
    SessionManager &operator=(const SessionManager &);

public:
    void OnNewConnect();
private:
    std::unordered_map<uint64_t, Session *> session_list_;
};

} // namespace cherry

#endif
