#include "collision_data.hpp"
#include "sys_math.hpp"
#include "sys_math3d.hpp"

// Tomb collision data

BgCamInfo object_spot02_objects_Col_0133ECCamDataList[] = {
    {0x0000, 0, NULL},
};

SurfaceType object_spot02_objects_Col_0133ECSurfaceType[] = {
    {0x00000000, 0x000007C2},
    {0x00200000, 0x000007C2},
};

CollisionPoly object_spot02_objects_Col_0133ECPolygons[] = {
    {0x0000, 0x0000, 0x0001, 0x0002, 0x8613, 0x242A, 0xF189, 0xFCC8},
    {0x0000, 0x0003, 0x0000, 0x0002, 0x8089, 0x0000, 0xF456, 0xFCBA},
    {0x0000, 0x0004, 0x0005, 0x0006, 0x79ED, 0x242A, 0xF189, 0xFCC8},
    {0x0000, 0x0004, 0x0006, 0x0007, 0x7F77, 0x0000, 0xF456, 0xFCBA},
    {0x0001, 0x0008, 0x0009, 0x0005, 0x7FFF, 0x0000, 0x0000, 0xFD06},
    {0x0001, 0x0008, 0x0005, 0x0004, 0x7FFF, 0x0000, 0x0000, 0xFD06},
    {0x0000, 0x000A, 0x0009, 0x0008, 0x5580, 0x5F40, 0x0000, 0xFADE},
    {0x0000, 0x000B, 0x000C, 0x000A, 0x0000, 0x7877, 0xD4BF, 0xFAE0},
    {0x0000, 0x000B, 0x000A, 0x0008, 0x0000, 0x7877, 0xD4BF, 0xFAE0},
    {0x0000, 0x000B, 0x000D, 0x000C, 0xAA80, 0x5F40, 0x0000, 0xFADE},
    {0x0001, 0x0001, 0x000D, 0x000B, 0x8001, 0x0000, 0x0000, 0xFD06},
    {0x0001, 0x0002, 0x0001, 0x000B, 0x8001, 0x0000, 0x0000, 0xFD06},
    {0x0001, 0x0002, 0x000B, 0x0008, 0x0000, 0x0000, 0x8001, 0xFC9A},
    {0x0001, 0x0002, 0x0008, 0x0004, 0x0000, 0x0000, 0x8001, 0xFC9A},
    {0x0000, 0x0007, 0x0006, 0x0000, 0x0000, 0x0000, 0x7FFF, 0xFDFA},
    {0x0000, 0x0007, 0x0000, 0x0003, 0x0000, 0x0000, 0x7FFF, 0xFDFA},
    {0x0000, 0x0000, 0x0006, 0x0005, 0x0000, 0x7E37, 0x1549, 0xFF2D},
    {0x0000, 0x0000, 0x0005, 0x0001, 0x0000, 0x7E37, 0x1549, 0xFF2D},
    {0x0001, 0x0009, 0x000A, 0x000C, 0x0000, 0x0000, 0x7FFF, 0x00EB},
    {0x0001, 0x0009, 0x000C, 0x000D, 0x0000, 0x0000, 0x7FFF, 0x00EB},
    {0x0001, 0x0005, 0x0009, 0x000D, 0x0000, 0x0000, 0x7FFF, 0x00EB},
    {0x0001, 0x0005, 0x000D, 0x0001, 0x0000, 0x0000, 0x7FFF, 0x00EB},
};

Vec3s object_spot02_objects_Col_0133ECVertices[] = {
    {-889, 127, 518},   {-762, 254, -235},  {-762, 0, -870},
    {-889, 0, 518},     {762, 0, -870},     {762, 254, -235},
    {889, 127, 518},    {889, 0, 518},      {762, 1082, -870},
    {762, 1082, -235},  {508, 1310, -235},  {-762, 1082, -870},
    {-508, 1310, -235}, {-762, 1082, -235},
};

CollisionHeader object_spot02_objects_Col_0133EC = {
    {-889, 0, -870},
    {889, 1310, 518},
    14,
    object_spot02_objects_Col_0133ECVertices,
    22,
    object_spot02_objects_Col_0133ECPolygons,
    object_spot02_objects_Col_0133ECSurfaceType,
    object_spot02_objects_Col_0133ECCamDataList,
    0,
    NULL};

// Invisible seam poly
void findSeamHeights() {
  Vec3f v1 = {629, 194, -73};
  Vec3f v2 = {694, 180, -72};
  Vec3f v3 = {694, 228, -76};
  Vec3f normal = {0x0054 * SHT_MINV, 0x0aa1 * SHT_MINV, 0x7f8e * SHT_MINV};
  s16 dist = 0x0037;

  for (f32 x = 693.0f; x <= 695.0f; x += 0.01) {
    for (f32 z = -77.0f; z <= -75.0f; z += 0.01) {
      f32 yIntersect;
      if (Math3D_TriChkPointParaYIntersectInsideTri(&v1, &v2, &v3, normal.x,
                                                    normal.y, normal.z, dist, z,
                                                    x, &yIntersect, 1.0f)) {
        printf("x=%.7g y=%.7g z=%.7g\n", x, yIntersect, z);
      }
    }
  }
}

bool simulateJump(Collision* col, Vec3f pos, u16 angle, f32 initialSpeed,
                  int jsFrame, bool holdUp, bool debug) {
  f32 xzSpeed = initialSpeed;
  f32 ySpeed = -4.0f;
  for (int i = 0; i < 20; i++) {
    if (debug) {
      printf(
          "i=%02d x=%.7g y=%.7g z=%.7g x_raw=%08x y_raw=%08x z_raw=%08x "
          "angle=%04x "
          "xzSpeed=%.7g ySpeed=%.7g\n",
          i, pos.x, pos.y, pos.z, floatToInt(pos.x), floatToInt(pos.y),
          floatToInt(pos.z), angle, xzSpeed, ySpeed);
    }

    if (i > jsFrame + 1) {
      ySpeed -= 1.2f;
    } else {
      ySpeed -= 1.0f;
    }
    Vec3f velocity = {Math_SinS(angle) * xzSpeed, ySpeed,
                      Math_CosS(angle) * xzSpeed};
    Vec3f posNext = pos + velocity * 1.5f;

    CollisionPoly* wallPoly;
    CollisionPoly* floorPoly;
    int dynaId;
    f32 floorHeight;
    // TODO: no need to run every frame
    pos = col->runChecks(pos, posNext, &wallPoly, &floorPoly, &dynaId, &floorHeight);

    if (pos.y <= floorHeight && pos.y > 280) {
      return true;
    }

    if (i == 0) {
      // jump
      xzSpeed = 6.0f;
      ySpeed = 7.5f;
    } else if (i == jsFrame) {
      // jumpslash
      xzSpeed = 3.0f;
      ySpeed = 4.5f;
    } else if (i > jsFrame) {
      if (holdUp) {
        xzSpeed += 0.05f;
      }
    }
  }

  return false;
}

void findJumpSpots(Collision* col) {
  int i = 0;
  for (f32 speed = 6.0f; speed <= 9.0f; speed += 0.25f) {
    for (int angle = 0x2200; angle < 0x2600; angle += 0x10) {
      for (f32 x = 693.0f; x <= 695.0f; x += 0.05) {
        for (f32 z = -77.0f; z <= -76.0f; z += 0.05) {
          for (int js = 9; js <= 11; js++) {
            if (i % 10000 == 0) {
              fprintf(stderr, "i=%d speed=%g angle=%04x x=%.7g z=%.7g\r", i,
                      speed, angle, x, z);
            }
            i++;

            Vec3f pos = col->findFloor({x, 250, z});
            if (pos.y < 230.0f) {
              continue;
            }

            if (simulateJump(col, pos, angle, speed, js, true, false)) {
              printf(
                  "speed=%.7g angle=%04x x=%.7g y=%.7g z=%.7g x_raw=%08x "
                  "y_raw=%08x z_raw=%08x js=%d\n",
                  speed, angle, pos.x, pos.y, pos.z, floatToInt(pos.x),
                  floatToInt(pos.y), floatToInt(pos.z), js);
            }
          }
        }
      }
    }
  }
}

bool simulateClip(Collision* col, Vec3f pos, bool debug) {
  f32 xzSpeed = 6.0f;
  f32 ySpeed = -4.0f;
  u16 angle = 0xcbb8;
  int jsFrame = 11;
  for (int i = 0; i < 30; i++) {
    if (debug) {
      printf(
          "i=%02d x=%.7g y=%.7g z=%.7g x_raw=%08x y_raw=%08x z_raw=%08x "
          "angle=%04x "
          "xzSpeed=%.7g ySpeed=%.7g\n",
          i, pos.x, pos.y, pos.z, floatToInt(pos.x), floatToInt(pos.y),
          floatToInt(pos.z), angle, xzSpeed, ySpeed);
    }

    if (i > jsFrame + 1) {
      ySpeed -= 1.2f;
    } else {
      ySpeed -= 1.0f;
    }
    Vec3f velocity = {Math_SinS(angle) * xzSpeed, ySpeed,
                      Math_CosS(angle) * xzSpeed};
    Vec3f posNext = pos + velocity * 1.5f;

    CollisionPoly* wallPoly;
    CollisionPoly* floorPoly;
    int dynaId;
    f32 floorHeight;
    pos = col->runChecks(pos, posNext, &wallPoly, &floorPoly, &dynaId, &floorHeight);

    if (i == 0) {
      // jump
      xzSpeed = 6.0f;
      ySpeed = 7.5f;
    }

    if (i < 5) {
      // hold down
      Math_ScaledStepToS(&angle, 0xc000, 400);
    } else if (i < jsFrame) {
      // release down
      xzSpeed = std::max(0.0f, xzSpeed - 1.0f);
    } else if (i == jsFrame) {
      // jumpslash
      xzSpeed = 3.0f;
      ySpeed = 4.5f;
      angle = 0x4000;
    } else {
      xzSpeed -= 0.10f;
    }
  }

  return false;
}

int main(int argc, char* argv[]) {
  Collision col(&spot02_sceneCollisionHeader_003C54, PLAYER_AGE_ADULT, {620, 180, -320}, {850, 230, 156});
  col.addDynapoly(&object_spot02_objects_Col_0133EC, {0.1f, 0.1f, 0.1f},
                  {0, (s16)0xc000, 0}, {762, 180, 80});

  // findSeamHeights();

  // Jolin's demo
//   simulateJump(
//       &col,
//       {intToFloat(0x442d9005), intToFloat(0x436aaf8a), intToFloat(0xc299107a)},
//       0x23f0, 9.0f, 10, true);

  // findJumpSpots(&col);

  // Bliny's setup
  simulateClip(
      &col,
      {intToFloat(0x444536ca), intToFloat(0x4390f49f),
      intToFloat(0x40c7e93f)}, true);
}
