#ifndef SYSTEM_BASE_HPP
#define SYSTEM_BASE_HPP
#include "engine_constants.hpp"
#include <set>
// NOTE: all systems should inherit from this system

// NOTE: DONT FORGET TO INHERIT PUBLICLY FROM THIS CLASS
// and also override sys_call();
using Priority = uint64_t;
const Priority NO_PRIORITY = 42069;
const Priority MAX_PRIORITY = MAX_SYSTEMS;

class System {
  public:
    std::set<EntityId> sys_entities{};
    // sys_call is a generic method, required to sort systems by priority at the end.
    // it is necessary to work around this method if needed for subsequent function calls
    virtual void sys_call() = 0;
    // NOTE: new variable, used to disable and enable systems
    bool awake = true;
    Priority priority = NO_PRIORITY;
};

// struct SystemListener : System {
//   // NOTE: same as a system, but also listens for events from EventManager
//   // and deals with the events inside of on_notify
//     virtual void sys_call() = 0;
//     virtual void on_notify() = 0;
// };

// class Group : public System {
//     // solution to systems that want multiple groups of entities
//     // i feel, for bigger systems, it is not viable to force them
//     // to divide into parts just to have multiple groups of entities
//     // and a lot of systems just simply dont work with specifying what they want
//
//     // a group is made from an entity query
//     // - it can be requested by a system
//   public:
//     void sys_call() override {}
//     EntityQuery query;
// };

#endif
