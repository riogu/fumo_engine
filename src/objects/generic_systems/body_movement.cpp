#include <sched.h>

#include <cerrno>

#include "fumo_engine/core/global_state.hpp"
#include "raylib.h"
#include "raymath.h"
#include "systems.hpp"

extern std::unique_ptr<GlobalState> global;

// const static float movement_scaling = 20000.0f;
const static float movement_scaling = 560.0f;
const static float jump_scaling = 400.0f;

void BodyMovement::move(const EntityId& entity_id, const DIRECTION& direction) {
    auto& body = global->ECS->get_component<Body>(entity_id);
    auto& player_state = global->ECS->get_component<EntityState>(entity_id);
    auto& player_animation = global->ECS->get_component<AnimationInfo>(entity_id);

    switch (direction) {
        case DIRECTION::LEFT:
            body.velocity -= body.x_direction * movement_scaling;
            if (player_state.on_ground) {
                body.inverse_direction = true;
                AnimationPlayer::play(player_animation, "run");
            }
            break;
        case DIRECTION::RIGHT:
            body.velocity += body.x_direction * movement_scaling;
            if (player_state.on_ground) {
                body.inverse_direction = false;
                AnimationPlayer::play(player_animation, "run");
            }
            break;

        default:
            PANIC("UNREACHABLE");
            std::unreachable();
    }
}


// void BodyMovement::jump(Body& body, const EntityId& entity_id) {
//     // auto& player_state = global->ECS->get_component<EntityState>(entity_id);
//     // TODO:
//     // Bonus Air Time
//     // Peak control
//     // Fast Falling
//     // Lerp the jump movement
    // global->event_handler->add_event({EVENT_::ENTITY_JUMPED, entity_id});
//
//
//     // body.velocity += Vector2Negate(body.gravity_direction) * jump_scaling;
// }


// void move_horizontally(Body& body, float amount) {
//     if (body.inverse_direction == true) {
//         body.velocity -= body.x_direction * amount * movement_scaling;
//         return;
//     }
//     body.velocity += body.x_direction * amount * movement_scaling;
// }

//
// void move_vertically(Body& body, float amount) {
//     body.velocity -= body.gravity_direction * amount * movement_scaling;
// }
// void BodyMovement::move_towards(Body& body, Body& target) {
//     Vector2 direction = Vector2Normalize(target.position - body.position);
//     float sqr_distance = Vector2DistanceSqr(target.position, body.position);
//     body.velocity = direction * sqr_distance * global->frametime;
// }
//
// void BodyMovement::move_towards_position(Body& body, Vector2 position) {
//     Vector2 direction = Vector2Normalize(position - body.position);
//     float sqr_distance = Vector2DistanceSqr(position, body.position);
//     body.velocity = direction * sqr_distance * global->frametime;
// }
