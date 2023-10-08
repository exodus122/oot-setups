#pragma once

#include "global.hpp"

// Move an actor. Note that speeds are multiplied by 1.5.
Vec3f translate(Vec3f pos, u16 angle, f32 xzSpeed, f32 ySpeed);

// Move with collider displacement.
Vec3f translate(Vec3f pos, u16 angle, f32 xzSpeed, f32 ySpeed,
                Vec3f displacement);

// Rotate a vector by an angle.
Vec3f rotate(Vec3f v, u16 angle);
