#pragma once
#include <utility>
#include <vector>

#include "fumo_engine/components.hpp"
#include "raylib.h"

struct Collision {
    float overlap;
    Vector2 push;
};

// [[nodiscard]] float PointToLineDistance(const Vector2& Point, const Vector2&
// LineStart,
//                                         const Vector2& LineEnd);
[[nodiscard]] std::pair<float, Vector2>
PointToLineDistanceAndIntersection(const Vector2& Point,
                                   const Vector2& LineStart,
                                   const Vector2& LineEnd);
[[nodiscard]] std::pair<float, Vector2>
PointToLineDistanceAndIntersection(const Vector2& Point,
                                   const std::pair<Vector2, Vector2>& line);
[[nodiscard]] std::pair<float, Vector2>
closest_rectangle_side(const std::vector<std::pair<float, Vector2>>& distances);
[[nodiscard]] Vector2 closest_point(const Vector2 target,
                                    const std::vector<Vector2>& points);
// [[nodiscard]] Vector2 LineToLineIntersection(const std::pair<Vector2, Vector2>& line1,
//                                              const std::pair<Vector2, Vector2>&
//                                              line2);
[[nodiscard]] std::pair<float, Vector2>
CircleToRectDistanceAndIntersection(const Vector2& circle_center,
                                    const float& radius,
                                    const Rectangle& rect,
                                    const Body& rect_body);

[[nodiscard]] Collision PlayerToRectCollision(const PlayerShape& player_shape,
                                              const Body& player_body,
                                              const Rectangle& rectangle,
                                              const Body& rectange_body);

[[nodiscard]] Collision PlayerToCircleCollision(const PlayerShape& player_shape,
                                                const Body& player_body,
                                                const Circle& circle_shape,
                                                const Body& circle_body);
