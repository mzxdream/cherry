#ifndef __GAME_ENTITY_H__
#define __GAME_ENTITY_H__

#include <map>
#include <cstdint>

#include "game_object.h"

class GameEntity
    : public GameObject
{
public:
    typedef int64_t ID;
    static const ID INVALID_ID;
public:
    GameEntity();
    virtual ~GameEntity();
public:
    ID GetID() const;
    template <typename T, typename ...Args>
    T * AddComponent(Args... args);
    template <typename T>
    T * GetComponent();
private:
    std::map<ID, GameComponent *> component_list_;
};

#endif
