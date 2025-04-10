#pragma once
#include "fumo_engine/components.hpp"
#include "fumo_engine/core/system_base.hpp"

// class BodyMovement : public System {
// prefer making a body movement system, instead of
// adding a move_towards() method to the Body component (follow ECS logic)
// public:
// NOTE: you need to give a definition of sys_call() virtual method,
// otherwise the compiler wont allocate a vtable for this class,
// since it will treat it as virtual and not allow instantiations of it
// void sys_call() override {};

enum struct DIRECTION { LEFT, RIGHT, UP, DOWN , NO_DIRECTION};

namespace BodyMovement {
void jump(Body& body, const EntityId& entity_id);
void hard_coded_jump();
void move(const EntityId& entity_id, const DIRECTION& direction);
} // namespace BodyMovement

struct JumpHandler: System {
    // wrapper for hard coded jump
    // (want to call it every frame)
    void sys_call() override {
        // BodyMovement::hard_coded_jump();
    }
};

namespace Debugger {
void global_debug();
}; // namespace Debugger

// void update_position(Body& body);
// void reset_velocity(Body& body);
// void move_vertically(Body& body, float amount);
// void move_horizontally(Body& body, float amount);
// void move_towards(Body& body, Body& target);
// void move_towards_position(Body& body, Vector2 position);
// void move_vertically_fixed(Body& body, float amount);
// void move_horizontally_fixed(Body& body, float amount);
