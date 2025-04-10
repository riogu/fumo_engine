#include "fumo_engine/core/global_state.hpp"
#include "objects/generic_systems/factory_systems.hpp"
#include "raylib.h"
// const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

extern std::unique_ptr<GlobalState> global;

Vector2 starter_position = {0.0f, screenCenter.y + 400};
float previous_width = 500.0f;

void make_some_rects();
void screen_border_lines();

void debug_spawn_level_objects() {
    make_some_rects();
    screen_border_lines();
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

    {
        EntityId id_planet = planet_factory->create_rect(starter_position);
        global->ECS->entity_add_component(id_planet, OutlineRectFlag {});

        Rectangle& outline_rect = global->ECS->get_component<Rectangle>(id_planet);
        outline_rect = {.x = 0.0f,
                        .y = 0.0f,
                        .width = screenWidth,
                        .height = screenHeight};
    }
    {
        // make first planet that owns the player
        EntityId id_planet = planet_factory->create_rect_planet(
            {starter_position.x - screenWidth * 3, starter_position.y});

        auto& player_state =
            global->ECS->get_component<EntityState>(global->player_id);
        player_state.player_owning_field = id_planet;

        Rectangle& rect_planet = global->ECS->get_component<Rectangle>(id_planet);
        rect_planet.width = rect_planet.width * 100;
        previous_width = rect_planet.width;

        ParallelGravityField& rect_field =
            global->ECS->get_component<ParallelGravityField>(id_planet);
        rect_field.field_rectangle.width = rect_planet.width;
    }
}

void screen_border_lines() {
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

    {
        EntityId id_planet = planet_factory->create_rect(starter_position);
        global->ECS->entity_add_component(id_planet, OutlineRectFlag {});

        Rectangle& outline_rect = global->ECS->get_component<Rectangle>(id_planet);
        outline_rect = {.x = 0.0f,
                        .y = 0.0f,
                        .width = screenWidth,
                        .height = screenHeight};
    }
    {
        EntityId id_planet = planet_factory->create_rect(starter_position);
        global->ECS->entity_add_component(id_planet, OutlineRectFlag {});

        Rectangle& outline_rect = global->ECS->get_component<Rectangle>(id_planet);
        outline_rect = {.x = screenWidth,
                        .y = 0.0f,
                        .width = screenWidth,
                        .height = screenHeight};
    }
    {
        EntityId id_planet = planet_factory->create_rect(starter_position);
        global->ECS->entity_add_component(id_planet, OutlineRectFlag {});

        Rectangle& outline_rect = global->ECS->get_component<Rectangle>(id_planet);
        outline_rect = {.x = screenWidth * 2,
                        .y = 0.0f,
                        .width = screenWidth,
                        .height = screenHeight};
    }
    {
        EntityId id_planet = planet_factory->create_rect(starter_position);
        global->ECS->entity_add_component(id_planet, OutlineRectFlag {});

        Rectangle& outline_rect = global->ECS->get_component<Rectangle>(id_planet);
        outline_rect = {.x = screenWidth * 3,
                        .y = 0.0f,
                        .width = screenWidth,
                        .height = screenHeight};
    }
    Vector2 positon = screenCenter;
    positon.y -= 230;
    { EntityId circle_planet = planet_factory->create_circular_planet(positon); }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    positon.x += 900;
    {
        EntityId circle_planet = planet_factory->create_circular_planet(positon);
        EntityId circle_planet2 =
            planet_factory->create_circular_planet({positon.x, positon.y - 900});
    }
    Vector2 screncentr = screenCenter;
    screncentr.x -= screenWidth / 3.0f;
    { EntityId rect_planet = planet_factory->create_rect_planet(screncentr); }
    screncentr.x -= screenWidth / 3.0f;
    { EntityId rect_planet = planet_factory->create_rect_planet(screncentr); }
    screncentr.x -= screenWidth / 3.0f;
    { EntityId rect_planet = planet_factory->create_rect_planet(screncentr); }
    screncentr.x -= screenWidth / 3.0f;
    { EntityId rect_planet = planet_factory->create_rect_planet(screncentr); }
    screncentr.x -= screenWidth / 3.0f;
    { EntityId rect_planet = planet_factory->create_rect_planet(screncentr); }
    screncentr.x -= screenWidth / 3.0f;
    { EntityId rect_planet = planet_factory->create_rect_planet(screncentr); }
    screncentr.x -= screenWidth / 3.0f;
    { EntityId rect_planet = planet_factory->create_rect_planet(screncentr); }
}

void make_some_rects() {
    const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();

    {
        // make first planet that owns the player
        EntityId id_planet = planet_factory->create_rect_planet(
            {starter_position.x - screenWidth * 3, starter_position.y});

        auto& player_state =
            global->ECS->get_component<EntityState>(global->player_id);
        player_state.player_owning_field = id_planet;

        Rectangle& rect_planet = global->ECS->get_component<Rectangle>(id_planet);
        rect_planet.width = rect_planet.width * 100;
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
