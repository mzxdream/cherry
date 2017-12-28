#ifndef __GMAE_EVENT_H__
#define __GAME_EVENT_H__

#include <cstdint>
#include <functional>
#include <map>

#include "game_object.h"

template <typename... Args>
class GameEvent
    : public GameObject
{
public:
    typedef int64_t ID;
    typedef std::function<void (Args...)> Listener;
public:
    GameEvent()
        : next_id_(0)
    {
    }
    virtual ~GameEvent()
    {

    }
public:
    ID AddListener(const Listener &listener)
    {
        ID id = GenerateID();
        listener_list_[id] = listener;
        return id;
    }
    void RemoveListener(ID id)
    {
        listener_list_.erase(id);
    }
    void RemoveAllListener()
    {
        listener_list_.clear();
    }
    void Invoke(Args... args)
    {
        for (auto &iter_listener : listener_list_)
        {
            iter_listener.second(args...);
        }
    }
private:
    ID GenerateID()
    {
        return ++next_id_;
    }
private:
    std::map<ID, Listener> listener_list_;
    ID next_id_;
};

#endif
