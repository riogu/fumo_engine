#include "raylib.h"
#include "fumo_engine/core/global_state.hpp"
#include "objects/factory_systems.hpp"

std::unique_ptr<GlobalState> global;

void register_all_to_ECS();
void initialize_all_textures();

int main(void) {

    InitWindow(screenWidth, screenHeight, "THIS... is a BUCKET.");

    global = std::make_unique<GlobalState>();
    global->initialize();

    initialize_all_textures();
    register_all_to_ECS();

    global->setup_game_state();

    SetTargetFPS(60);
    int count = 0;

    while (!WindowShouldClose()) {
        ClearBackground(BLACK);
        BeginDrawing();

        global->camera->zoom += ((float)GetMouseWheelMove() * 0.05f);

        global->frametime = GetFrameTime();

        global->ECS->run_systems();

        // here because we start with no planets right now (remove whem we make levels)
        if (!count) [[unlikely]] {
            const auto& planet_factory = global->ECS->get_system<LevelEntityFactory>();
            EntityId planet_id = planet_factory->create_circular_planet(
                {screenCenter.x / 2.0f, screenCenter.y});
            count++;

        }

        DrawFPS(10, 10);
        EndDrawing();
    }
    //--------------------------------------------------------------------------------------
    // unload textures before closing the OpenGL context
    global->destroy_and_unload_game();

    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
