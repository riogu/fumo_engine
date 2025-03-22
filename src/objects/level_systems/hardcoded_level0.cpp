#include "fumo_engine/core/global_state.hpp"
#include "objects/generic_systems/factory_systems.hpp"
#include "raylib.h"
// const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

extern std::unique_ptr<GlobalState> global;

Vector2 starter_position = {screenCenter.x - 150.0f, screenCenter.y + 400};
float previous_width = 500.0f;

void make_some_rects();
void screen_border_lines();

void debug_spawn_level_objects() {
    // EntityId id_planet;
    make_some_rects();
    screen_border_lines();
}

void screen_border_lines() {
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

    {
        EntityId id_planet = planet_factory->create_rect(starter_position);
        global->ECS->entity_add_component(id_planet, OutlineRectFlag{});

        Rectangle& outline_rect = global->ECS->get_component<Rectangle>(id_planet);
        outline_rect = {.x = 0.0f, .y = 0.0f, .width = screenWidth, .height = screenHeight};
    }
}

void make_some_rects() {
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

    {
        // make first planet that owns the player
        EntityId id_planet = planet_factory->create_rect_planet(starter_position);

        auto& player_shape = global->ECS->get_component<PlayerShape>(global->player_id);
        player_shape.player_owning_field = id_planet;

        Rectangle& rect_planet = global->ECS->get_component<Rectangle>(id_planet);
        rect_planet.width = rect_planet.width * 2;
        previous_width = rect_planet.width;

        ParallelGravityField& rect_field =
            global->ECS->get_component<ParallelGravityField>(id_planet);
        rect_field.field_rectangle.width = rect_planet.width;
    }

    {
        starter_position.x += previous_width;

        EntityId id_planet = planet_factory->create_rect(starter_position);
        Rectangle& no_grav_rect = global->ECS->get_component<Rectangle>(id_planet);

        previous_width = no_grav_rect.width;
    }
}
