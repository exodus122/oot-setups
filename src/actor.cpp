#include "actor.hpp"

#include "sys_math.hpp"

Vec3f translate(Vec3f pos, u16 angle, f32 xzSpeed, f32 ySpeed) {
  Vec3f velocity = {Math_SinS(angle) * xzSpeed, ySpeed,
                    Math_CosS(angle) * xzSpeed};
  return pos + velocity * 1.5f;
}

Vec3f translate(Vec3f pos, u16 angle, f32 xzSpeed, f32 ySpeed,
                Vec3f displacement) {
  Vec3f velocity = {Math_SinS(angle) * xzSpeed, ySpeed,
                    Math_CosS(angle) * xzSpeed};
  return pos + (velocity * 1.5f + displacement);
}

Vec3f rotate(Vec3f v, u16 angle) {
  f32 s = Math_SinS(angle);
  f32 c = Math_CosS(angle);
  return {v.z * s + v.x * c, v.y, v.z * c - v.x * s};
}
