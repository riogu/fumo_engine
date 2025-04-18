#include "player_collisions.hpp"

#include "fumo_engine/collisions_and_physics/point_line_collisions.hpp"
#include "fumo_engine/core/global_state.hpp"

extern std::unique_ptr<GlobalState> global;

void PlayerCollisionRunner::check_collisions() {
    bool collision_happened = false;

    const auto& player_id = global->player_id;
    auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);
    auto& player_body = global->ECS->get_component<Body>(global->player_id);
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);

    EntityQuery query_fumo_rect {.component_mask =
                                     global->ECS->make_component_mask<FumoRect>(),
                                 .component_filter = Filter::All};

    for (const auto& obstacle_id : sys_entities) {
        const auto& obstacle_body = global->ECS->get_component<Body>(obstacle_id);

        if (global->ECS->filter(obstacle_id, query_fumo_rect)) {
            const auto& fumo_rect =
                global->ECS->get_component<FumoRect>(obstacle_id);
            if (player_to_rect_collision_solving(player_shape,
                                                 player_body,
                                                 fumo_rect,
                                                 obstacle_body)) {
                collision_happened = true;
            }
        } else {
            const auto& circle_shape =
                global->ECS->get_component<Circle>(obstacle_id);
            if (player_to_circle_collision_solving(player_shape,
                                                   player_body,
                                                   circle_shape,
                                                   obstacle_body)) {
                collision_happened = true;
            }
        }
    }

    player_state.colliding = collision_happened;
    if (collision_happened) {
        global->event_handler->add_event({EVENT_::ENTITY_COLLIDED, player_id});
        return;
    }
}

bool PlayerCollisionRunner::player_to_rect_collision_solving(
    PlayerShape& player_shape,
    Body& player_body,
    const FumoRect& fumo_rect,
    const Body& fumo_rect_body) {
    // NOTE: assume fumo_rects are NOT rotated
    // (add rotation support later if i want to add slopes and stuff)

    // -------------------------------------------------------------------------------
    // solve the collision
    Collision collision =
        PlayerToRectCollision(player_shape, player_body, fumo_rect, fumo_rect_body);

    if (collision.overlap == 0.0f) {
        // no collision happened
        return false;
    }
    // -------------------------------------------------------------------------------
    // points towards the player_body
    FumoVec2 push = FumoVec2Normalize(collision.push);
    float correction = 1.0f; // this value is here to stabilize the simulation
    push = FumoVec2Scale(push, collision.overlap * correction);
    player_body.position += push;
    player_shape.update_capsule_positions(player_body);
    return true;
}

bool PlayerCollisionRunner::player_to_circle_collision_solving(
    PlayerShape& player_shape,
    Body& player_body,
    const Circle& circle_shape,
    const Body& circle_body) {
    // -------------------------------------------------------------------------------
    // solve the collision
    Collision collision = PlayerToCircleCollision(player_shape,
                                                  player_body,
                                                  circle_shape,
                                                  circle_body);

    if (collision.overlap == 0.0f) {
        // no collision happened
        return false;
    }
    // -------------------------------------------------------------------------------
    // points towards the player_body
    FumoVec2 push = FumoVec2Normalize(collision.push);
    float correction = 1.0f; // this value is here to stabilize the simulation
    push = FumoVec2Scale(push, collision.overlap * correction);
    player_body.position += push;
    player_shape.update_capsule_positions(player_body);
    return true;
}
