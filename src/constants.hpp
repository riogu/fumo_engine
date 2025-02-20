#ifndef CONSTANTS_HPP_
#define CONSTANTS_HPP_
#include "array"
#include "raylib.h"
#include <iostream>
#include <libassert/assert.hpp>
// const int particle_count = 50;

const int screenWidth = 1900;
// const int screenWidth = 1025;
const int screenHeight = 1000;
const Vector2 screenCenter = {(float)screenWidth / 2, (float)screenHeight / 2};
#define GRAVITY 9.81
#define GRAVITATIONAL_CONSTANT 0.6
#define SCALING_FACTOR 1
#define FONT_SIZE 20
const float mouse_radius = 40.0f;
const int color_count = 18;
const float default_mass = 600000;
const float default_radius = 50.0f;
const Color player_color = ORANGE;
// NOTE: add orange back to all colors after adding a sprite to the player
const std::array<Color, 100> all_colors = {
    LIGHTGRAY, GRAY,    DARKGRAY, YELLOW, GOLD,   PINK,  RED,   MAROON,   GREEN,
    LIME,      SKYBLUE, BLUE,     PURPLE, VIOLET, BEIGE, BROWN, DARKBROWN};
#define PRINT(x)                                                                         \
    std::cerr << #x << " ---> " << libassert::highlight_stringify(x) << std::endl;
#endif
