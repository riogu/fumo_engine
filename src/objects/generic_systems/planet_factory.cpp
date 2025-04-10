#include "fumo_engine/core/global_state.hpp"
#include "objects/generic_systems/factory_systems.hpp"

extern std::unique_ptr<GlobalState> global;
[[nodiscard]] Rectangle make_default_field_rect(Vector2 position);
[[nodiscard]] Rectangle make_default_ground_rect(Vector2 position);
// TODO: move these to constants when you figure out their values
const float default_grav_strength = 900.0f;
const float default_gravity_reach = 150.0f;
const int default_rect_width = 200.0f;
const int default_rect_height = 200.0f;

void LevelEntityFactory::delete_planet(EntityId entity_id) {
    // here to keep track of what this instance of planet factory
    // has created so we can isolate these entities later
    // useful for grouping up entities based on their used context
    global->ECS->destroy_entity(entity_id);
    sys_entities.erase(entity_id);
    // TODO: create a player_owning component,
    // and have gravity updater query for that instead.
    // this is way too coupled by saving it outside of the ECS
    // like we are doing right now
    // const auto& gravity_updater = global->ECS->get_system<GravityUpdater>();
    auto& player_state = global->ECS->get_component<EntityState>(global->player_id);
    if (player_state.player_owning_field == entity_id) {
        player_state.player_owning_field = NO_ENTITY_FOUND;
    }
}

void LevelEntityFactory::delete_all_planets() {
    for (EntityId entity_id : sys_entities) {
        global->ECS->destroy_entity(entity_id);
    }
    sys_entities.clear();
}

EntityId LevelEntityFactory::create_circular_planet(Vector2 position) {

    EntityId entity_id = global->ECS->create_entity();

    global->ECS->entity_add_component(entity_id, ColliderObjectFlag{});
    global->ECS->entity_add_component(entity_id, GravFieldFlag{});
    global->ECS->entity_add_component(
        entity_id, Body{.position = position, .velocity = {0.0f, 0.0f}});
    global->ECS->entity_add_component(entity_id, Render{.color = BLUE});
    global->ECS->entity_add_component(entity_id, Circle{.radius = default_radius * 4});
    global->ECS->entity_add_component(
        entity_id, CircularGravityField{.gravity_radius = default_planet_radius *3,
                                        .gravity_strength = default_grav_strength,
                                        .position = position});
    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect_planet(Vector2 position) {

    EntityId entity_id = global->ECS->create_entity();

    Rectangle ground_rectangle = make_default_ground_rect(position);
    global->ECS->entity_add_component(entity_id, ground_rectangle);

    // placed above the ground rectangle, points downwards
    Rectangle grav_field_rectangle = make_default_field_rect(position);
    grav_field_rectangle.y -= default_rect_height;
    global->ECS->entity_add_component(entity_id,
                                      ParallelGravityField{
                                          .field_rectangle = grav_field_rectangle,
                                          .gravity_strength = default_grav_strength,
                                      });

    global->ECS->entity_add_component(
        entity_id, Body{.position = {ground_rectangle.x, ground_rectangle.y},
                        .velocity = {0.0f, 0.0f}});
    global->ECS->entity_add_component(entity_id, Render{.color = RED});
    global->ECS->entity_add_component(entity_id, ColliderObjectFlag{});
    global->ECS->entity_add_component(entity_id, GravFieldFlag{});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect_field(Vector2 position) {
    EntityId entity_id = global->ECS->create_entity();

    Rectangle grav_field_rectangle = make_default_field_rect(position);
    global->ECS->entity_add_component(
        entity_id, ParallelGravityField{
                       .field_rectangle = grav_field_rectangle,
                       .gravity_strength = default_grav_strength,
                       });

    global->ECS->entity_add_component(entity_id, GravFieldFlag{});
    global->ECS->entity_add_component(entity_id, Render{.color = BLUE});
    global->ECS->entity_add_component(
        entity_id, Body{.position = {grav_field_rectangle.x, grav_field_rectangle.y},
                        .velocity = {0.0f, 0.0f}});

    sys_entities.insert(entity_id);

    return entity_id;
}

EntityId LevelEntityFactory::create_rect(Vector2 position) {
    EntityId entity_id = global->ECS->create_entity();

    Rectangle ground_rectangle = make_default_ground_rect(position);
    global->ECS->entity_add_component(entity_id, ground_rectangle);

    global->ECS->entity_add_component(entity_id, ColliderObjectFlag{});
    global->ECS->entity_add_component(entity_id, Render{.color = RED});

    global->ECS->entity_add_component(
        entity_id, Body{.position = {ground_rectangle.x, ground_rectangle.y},
                        .velocity = {0.0f, 0.0f}});

    sys_entities.insert(entity_id);

    return entity_id;
}

[[nodiscard]] Rectangle make_default_field_rect(Vector2 position) {
    return {.x = position.x,
            .y = position.y,
            .width = default_rect_width,
            .height = default_rect_height};
}
[[nodiscard]] Rectangle make_default_ground_rect(Vector2 position) {
    return {.x = position.x,
            .y = position.y,
            .width = default_rect_width,
            .height = default_rect_height};
}

EntityId LevelEntityFactory::create_rect_field(Vector2 position,
                                               Vector2 grav_direction) {
    EntityId entity_id = global->ECS->create_entity();

    Rectangle grav_field_rectangle = make_default_field_rect(position);
    global->ECS->entity_add_component(
        entity_id, ParallelGravityField{
                       .field_rectangle = grav_field_rectangle,
                       .gravity_direction = grav_direction,
                       .gravity_strength = default_grav_strength,
                       });

    global->ECS->entity_add_component(entity_id, GravFieldFlag{});
    global->ECS->entity_add_component(entity_id, Render{.color = BLUE});
    global->ECS->entity_add_component(
        entity_id, Body{.position = {grav_field_rectangle.x, grav_field_rectangle.y},
                        .velocity = {0.0f, 0.0f}});

    sys_entities.insert(entity_id);

    return entity_id;
}
