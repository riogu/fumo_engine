#include "objects/player_systems/player_general_systems.hpp"
#include "fumo_engine/global_state.hpp"
#include "objects/components.hpp"

extern std::unique_ptr<GlobalState> global;

EntityId PlayerInitializer::initialize_player() {

    EntityId player_id = global->ECS->create_entity();

    global->ECS->entity_add_component(player_id, PlayerFlag{});
    global->ECS->entity_add_component(player_id, Render{.color = {50, 50, 50, 100}});
    global->ECS->entity_add_component(player_id, CircleShape{.radius = 66.0f});
    global->ECS->entity_add_component(player_id, Body{.position = screenCenter,
                                                      .velocity = {0.0f, 0.0f},
                                                      .smooth_jump_buffer = 1.0f});
    global->ECS->entity_add_component(player_id, AnimationInfo{});

    return player_id;
}

void UpdateCameraCenterSmoothFollow(Camera2D* camera, const Body& player) {
    float minSpeed = 30;
    float minEffectLength = 10;
    float fractionSpeed = 0.8f;

    camera->offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    Vector2 diff = Vector2Subtract(player.position, camera->target);
    float length = Vector2Length(diff);

    float scaling = 8.0f;

    if (length > minEffectLength) {
        float speed = fmaxf(fractionSpeed * length, minSpeed);
        camera->target =
            Vector2Add(camera->target,
                       Vector2Scale(diff, speed * global->frametime / length) * scaling);
    }
}

void PlayerEndFrameUpdater::reset_state() {
    const auto& player_id = global->player_id;
    auto& player_body = global->ECS->get_component<Body>(player_id);
    // const auto& body_movement_ptr = global->ECS->get_system<BodyMovement>();
    if (player_body.jumping) {
    }
    player_body.velocity = {0.0f, 0.0f};
}

void PlayerEndFrameUpdater::end_of_frame_update() {

    BeginMode2D(*global->camera);

    const auto& player_body = global->ECS->get_component<Body>(global->player_id);
    const auto& circle_shape =
        global->ECS->get_component<CircleShape>(global->player_id);
    const auto& render = global->ECS->get_component<Render>(global->player_id);
    DrawCircleV(player_body.position, circle_shape.radius, render.color);

    //-----------------------------------------------------------------
    // extra visualization code
    double gravity_reach = 300.0f;
    Vector2 normalized_velocity = Vector2Normalize(player_body.velocity);
    Vector2 line_end = player_body.position + normalized_velocity * gravity_reach;

    if (player_body.jumping) {
        line_end =
            player_body.position - (player_body.gravity_direction) * gravity_reach;
    }

    DrawLineV(player_body.position, line_end, YELLOW);
    //-----------------------------------------------------------------

    UpdateCameraCenterSmoothFollow(global->camera.get(), player_body);
    EndMode2D();

    reset_state();
}

