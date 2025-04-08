#pragma once
#include "constants.hpp"
#include "raylib.h"
#include "raymath.h"
#include <string_view>
// struct Rectangle;

struct Body {
    float rotation{}; // NOTE: in degrees?
    Vector2 position = screenCenter;
    Vector2 velocity{0.0f, 0.0f};
    Vector2 gravity_direction = {0.0f, 1.0f};
    Vector2 x_direction = {-gravity_direction.y, gravity_direction.x};

    bool inverse_direction = false;
    // player events and state

    // NOTE: follows the gravity direction, not the vertical player direction
    [[nodiscard]] Vector2 get_y_velocity() {
        return gravity_direction * Vector2DotProduct(velocity, gravity_direction);
    }
    [[nodiscard]] float get_dot_y_velocity() {
        return Vector2DotProduct(velocity, gravity_direction);
    }
    [[nodiscard]] Vector2 get_x_velocity() {
        return x_direction * Vector2DotProduct(velocity, x_direction);
    }
    void scale_velocity(float scale) { velocity += gravity_direction * scale; }

    // -------------------------------------------------------------------------------
    // trash to delete
    int iterations{};
    Vector2 acceleration{0.0f, 0.0f};
    // -------------------------------------------------------------------------------
};

struct Circle {
    float radius;
};

struct PlayerShape {
    // TODO: move this to the PlayerFlag

    float radius;
    Vector2 top_circle_center;
    Vector2 bottom_circle_center;

    std::pair<Vector2, Vector2> left_line;  // .first is the bottom point
    std::pair<Vector2, Vector2> right_line; // .first is the bottom point

    void update_capsule_positions(const Body& player_body) {
        top_circle_center =
            player_body.position - player_body.gravity_direction * radius;
        bottom_circle_center =
            player_body.position + player_body.gravity_direction * radius;

        left_line.second = top_circle_center - player_body.x_direction * radius;
        left_line.first = bottom_circle_center - player_body.x_direction * radius;

        right_line.second = top_circle_center + player_body.x_direction * radius;
        right_line.first = bottom_circle_center + player_body.x_direction * radius;
    }
};

// NOTE: we only want a single side for now, add a rectangle version
// later
// WARNING: **NOT** counted from the surface of the object we are on
struct ParallelGravityField {
    // this is parallel to a surface and points in one direction
    // the field has one direction INSIDE this rectangle

    // NOTE: assume the field points towards the bottom side of the rectangle
    Rectangle field_rectangle{};

    Vector2 gravity_direction = {0.0f, 1.0f}; // default is vertical
    float gravity_strength{};
    float rotation{}; // in degrees
    // Vector2 position = screenCenter;

    bool is_inside_field(const Body& player_body, const PlayerShape& player_shape) const;
    void update_gravity(Body& player_body);
};

// WARNING: **NOT** counted from the surface of the object we are on
struct CircularGravityField {
    // FIXME: save the push on the grav fields themselves later when refactoring
    // Vector2 push_direction;
    double gravity_radius;
    float gravity_strength;
    Vector2 position = screenCenter;
    Vector2 gravity_direction;

    bool is_inside_field(const Body& player_body, const PlayerShape& player_shape,
                         const Body& circular_body) const;
    void update_gravity(Body& player_body, const Body& body_planet);
};


// NOTE: to make a really modular and reusable timer class,
// consider using std::function to send in a function call
// after the timer ends

struct Timer {
    friend struct TimerHandler;

    float seconds_duration;
    std::string_view system_name = "NO_NAME";

    void make_timer(float _duration_seconds, std::string_view _system_name) {
        seconds_duration = _duration_seconds;
        system_name = _system_name;

        starting_time = GetTime();
        ending_time = starting_time + seconds_duration;
    }

    // another templated constructor we might wanna use in the future
    // template<typename T>
    // Timer(float duration_seconds) : duration_seconds(duration_seconds) {
    //     system_name = libassert::type_name<T>();
    //
    //     starting_time = GetTime();
    //     ending_time = starting_time + duration_seconds;
    // }

  private:
    float starting_time;
    float ending_time;
};

struct AnimationInfo {

    // keeps track of an entity's animation
    // so we can update that entity's SpriteSheet2D
    // a vector to hold the animations we want to play for the entity
    std::vector<std::string_view> sheet_vector{std::string_view("NO_SHEET")};
    // NOTE: it is a bit of overhead, but i am following godot's implementation
    // for this. for now i do not need this kind of behavior, but i will for stuff
    // like a landing animation being queued after a jump.
    // we also want to be able to replace the head element
    int frame_progress{1}; // progress along the frame
    int frame_speed{};     // number of frames to show per second
    int sprite_frame_count{};
    std::string_view current_sheet_name = "NO_SHEET";
    Rectangle current_region_rect{};
    int sub_counter{};
    bool is_running = false;

    float sprite_scaling{};
};
// ------------------------------------------------------------------------
// NOTE: not used as components
// ------------------------------------------------------------------------

// NOTE: when making an animation, we created an EntityId for that animation,
// which we then attribute to whatever entity should hold that animation

struct SpriteSheet2D {
    // used to animate entities
    Texture2D texture_sheet;
    std::string_view sprite_sheet_name;
    int sprite_frame_count;
    int base_frame_speed{}; // number of frames to show per second
    Rectangle base_region_rect{.x = 0.0f,
                               .y = 0.0f,
                               .width = (float)texture_sheet.width / sprite_frame_count,
                               .height = (float)texture_sheet.height};
    // base rect that defines how we go through each animation frame
    bool looping = false;
};

struct Sprite2D {
    Texture2D texture;
    std::string_view sprite_name;
    Rectangle region_rect;
};
