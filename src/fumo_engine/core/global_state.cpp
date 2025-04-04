#include "fumo_engine/core/global_state.hpp"

#include "objects/player_systems/player_general_systems.hpp"

void GlobalState::setup_game_state() {
    player_id = PlayerInitializer::initialize_player();

    camera = std::make_unique<Camera2D>();
    const auto& player_body = ECS->get_component<Body>(player_id);
    // camera->target = player_body.position;
    camera->target = screenCenter;
    camera->rotation = 0.0f;
    camera->zoom = 0.8f;
    camera->offset = screenCenter;
    auto& player_animation = ECS->get_component<AnimationInfo>(player_id);
    AnimationPlayer::play(player_animation, "idle");
}
