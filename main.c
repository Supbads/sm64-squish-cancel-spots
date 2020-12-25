#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include "sm64/gstate.h"


FILE *outputFile;


s16 levelData[] = {
  0x40,
  4,
  -8000, -8000, -8000,
   8000, -8000, -8000,
  -8000, -8000,  8000,
   8000, -8000,  8000,
   0, 2,
   1, 0, 2,
   1, 2, 3,

  0x40,
  4,
  -1103, -3104, -1839,
  -781, -3104, -1516,
  -1758, -3104, -539,
  -2080, -3104, -861,
  0, 2,
  0, 1, 2,
  0, 2, 3,

  0x40,
  8,
  -944, -2446, -1353,
  -1568, -2446, -1163,
  -1351, -2446, -946,
  -1161, -2446, -1571,
  -1351, -2037, -946,
  -1568, -2139, -1163,
  -1161, -2139, -1571,
  -944, -2037, -1353,
  0, 4,
  0, 1, 2,
  0, 3, 1,
  4, 5, 6,
  4, 6, 7,

  0x45
};


s16 pendulumYaw = 0x2000;
Vec3s pendulumPos = { -1389, -3030, -1028 };

// bounds: x:[-1000, 1000] y:[-1000,1000] z:[-100,100]
s16 pendulumModel[1000];

#define MAXX 1000
#define MAXZ 1000
s8 spotGrid[2 * MAXX + 1][2 * MAXZ + 1];


s8 *getSpot(s32 x, s32 z) {
  return &spotGrid[x + MAXX][z + MAXZ];
}


void findSpots(void) {
  Vec3f pushOffsets[] = {
    {-0.49f, 0.0f, -0.49f},
    {-0.49f, 0.0f, 0.49f},
    {0.49f, 0.0f, -0.49f},
    {0.49f, 0.0f, 0.49f},
    {0.0f, 0.0f, 0.0f},
    {0.49f, 0.0f, 0.0f},
    {-0.49f, 0.0f, 0.0f},
    {0.0f, 0.0f, 0.49f},
    {0.0f, 0.0f, -0.49f}};

  for (s32 x = -MAXX; x <= MAXX; x++) {
    for (s32 z = -MAXZ; z <= MAXZ; z++) {
      f32 exampleX = 0;
      f32 exampleZ = 0;
      s16 angle = 0;

      if (pendulumYaw == 0x2000) {
        if (x + z > 150 || x + z < -150)
          continue;
      }

      *getSpot(x, z) = 0;
      Vec3f pos = { pendulumPos[0] + x + 0.5f, pendulumPos[1] + 1500, pendulumPos[2] + z + 0.5f };
      // if (pos[0] == -1084 && pos[2] == -514) {
      //   printf("%f\n", pos[1]);
      // }

      struct Surface *floor;
      f32 floorHeight = find_floor(pos[0], pos[1], pos[2], &floor);

      if (floor == NULL || floorHeight < -6000) continue;

      struct Surface *ceil;
      f32 ceilHeight = find_ceil(pos[0], floorHeight + 80.0f, pos[2], &ceil);

      if (ceil == NULL) continue;

      f32 room = ceilHeight - floorHeight;
      if (!(room >= 0.0f && room <= 150.0f))
        continue;

      s32 steepFloorOrCeil = FALSE;

      if (floor != NULL && floor->normal[1] < 0.5f) {
        angle = atan2s(floor->normal[2], floor->normal[0]);
        steepFloorOrCeil = TRUE;
      }
      
      if (ceil != NULL && ceil->normal[1] > -0.5f) {
        angle = atan2s(ceil->normal[2], ceil->normal[0]);
        steepFloorOrCeil = TRUE;
      }

      if (!steepFloorOrCeil) continue;

      s32 airCondition = FALSE;

      for (size_t i = 0; i < sizeof(pushOffsets) / sizeof(pushOffsets[0]); i++) {
        f32 px = pos[0] + pushOffsets[i][0];
        f32 pz = pos[2] + pushOffsets[i][2];

        g->marioState->pos[0] = px;
        g->marioState->pos[1] = pos[1] + pushOffsets[i][1];
        g->marioState->pos[2] = pz;
        g->marioState->vel[0] = 10.0f * sins(angle);
        g->marioState->vel[1] = 0.0f;
        g->marioState->vel[2] = 10.0f * coss(angle);
        g->marioState->floor = floor;
        g->marioState->floorHeight = floorHeight;
        g->marioState->ceil = ceil;
        g->marioState->ceilHeight = ceilHeight;

        if (perform_ground_step(g->marioState) == GROUND_STEP_LEFT_GROUND) {
          exampleX = px;
          exampleZ = pz;
          airCondition = TRUE;
          break;
        }
      }

      if (!airCondition)
        continue;

      *getSpot(x, z) = 1;
      floor->mark = 1;
      ceil->mark = 2;
      // printf("Spot: pitch=0x%04X\n", g->currentObject->oFaceAnglePitch);
      // fprintf("pitch,yaw,roll,trunc x,trunc z,floor height,ceil height,floor ny,push angle,x,z\n");
      fprintf(outputFile, "%d,%d,%d,%d,%d,%f,%f,%f,%d,%f,%f\n",
        g->currentObject->oFaceAnglePitch,
        g->currentObject->oFaceAngleYaw,
        g->currentObject->oFaceAngleRoll,
        (s16)pos[0],
        (s16)pos[2],
        floorHeight,
        ceilHeight,
        floor->normal[1],
        angle,
        exampleX,
        exampleZ);
    }
  }
}


struct {
  Vec3f pos;
  f32 pitch;
  f32 yaw;
} camera = {{-2000, 0, 0}, 0, 3.14159f / 2.0f};


void updateCamera(GLFWwindow *window) {
  f32 forward = 0;
  f32 sideways = 0;
  f32 upward = 0;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) forward += 1;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) forward -= 1;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) sideways += 1;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) sideways -= 1;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) upward += 1;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) upward -= 1;

  f32 mag = sqrtf(forward*forward + sideways*sideways + upward*upward);
  if (mag != 0) {
    forward /= mag;
    sideways /= mag;
    upward /= mag;

    f32 speed = 35.0f;
    forward *= speed;
    sideways *= speed;
    upward *= speed;

    camera.pos[0] += forward * sinf(camera.yaw);
    camera.pos[2] += forward * cosf(camera.yaw);

    camera.pos[0] += sideways * sinf(camera.yaw - 3.1415926f / 2.0f);
    camera.pos[2] += sideways * cosf(camera.yaw - 3.1415926f / 2.0f);

    camera.pos[1] += upward;
  }

  f32 yaw = 0;
  f32 pitch = 0;

  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) yaw += 1;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) yaw -= 1;
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) pitch += 1;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) pitch -= 1;

  camera.yaw += yaw * 0.03f;
  camera.pitch += pitch * 0.03f;
}


s32 boundMinX, boundMaxX, boundMinY, boundMaxY, boundMinZ, boundMaxZ;


void checkBounds(Vec3s p) {
  if (p[0] < boundMinX) boundMinX = p[0];
  if (p[1] < boundMinY) boundMinY = p[1];
  if (p[2] < boundMinZ) boundMinZ = p[2];

  if (p[0] > boundMaxX) boundMaxX = p[0];
  if (p[1] > boundMaxY) boundMaxY = p[1];
  if (p[2] > boundMaxZ) boundMaxZ = p[2];
}


void renderSurfaces(void) {
  for (int i = 0; i < g->surfacesAllocated; i++) {
    struct Surface *s = &g->surfacePool[i];

    checkBounds(s->vertex1);
    checkBounds(s->vertex2);
    checkBounds(s->vertex3);

    switch (classify_surface(s)) {
    case 'f': glColor4f(0.5f, 0.5f, 1, 1); break;
    case 'c': glColor4f(1, 0.5f, 0.5f, 1); break;
    case 'w': glColor4f(0.3f, 0.8f, 0.3f, 1); break;
    }

    // glBegin(GL_TRIANGLES);
    // glVertex3f(s->vertex1[0], s->vertex1[1], s->vertex1[2]);
    // glVertex3f(s->vertex2[0], s->vertex2[1], s->vertex2[2]);
    // glVertex3f(s->vertex3[0], s->vertex3[1], s->vertex3[2]);
    // glEnd();

    glClearColor(1, 1, 1, 1);

    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(s->vertex1[0], s->vertex1[1], s->vertex1[2]);
    glVertex3f(s->vertex2[0], s->vertex2[1], s->vertex2[2]);
    glVertex3f(s->vertex3[0], s->vertex3[1], s->vertex3[2]);
    glEnd();

    if (s->mark != 0) {
      if (s->mark == 1) {
        glColor4f(0.5f, 0.5f, 1, 1);
        // printf("floor\n");
        // printf("  v1 = (%d, %d, %d)\n", s->vertex1[0], s->vertex1[1], s->vertex1[2]);
        // printf("  v2 = (%d, %d, %d)\n", s->vertex2[0], s->vertex2[1], s->vertex2[2]);
        // printf("  v3 = (%d, %d, %d)\n", s->vertex3[0], s->vertex3[1], s->vertex3[2]);
        // printf("  n = (%f, %f, %f)\n", s->normal[0], s->normal[1], s->normal[2]);
      }
      else if (s->mark == 2) {
        glColor4f(1, 0.5f, 0.5f, 1);
        // printf("ceil\n");
        // printf("  v1 = (%d, %d, %d)\n", s->vertex1[0], s->vertex1[1], s->vertex1[2]);
        // printf("  v2 = (%d, %d, %d)\n", s->vertex2[0], s->vertex2[1], s->vertex2[2]);
        // printf("  v3 = (%d, %d, %d)\n", s->vertex3[0], s->vertex3[1], s->vertex3[2]);
        // printf("  n = (%f, %f, %f)\n", s->normal[0], s->normal[1], s->normal[2]);
      }
      s->mark = 0;

      glBegin(GL_TRIANGLES);
      glVertex3f(s->vertex1[0], s->vertex1[1], s->vertex1[2]);
      glVertex3f(s->vertex2[0], s->vertex2[1], s->vertex2[2]);
      glVertex3f(s->vertex3[0], s->vertex3[1], s->vertex3[2]);
      glEnd();
    }
  }

  // printf("[%d, %d] [%d, %d] [%d, %d]\n", boundMinX, boundMaxX, boundMinY, boundMaxY, boundMinZ, boundMaxZ);
}


void render(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();
  glFrustum(-1, 1, -1, 1, 2, 10000);
  glRotatef(-camera.pitch * 180.0f / 3.1415926f, 1, 0, 0);
  glRotatef(-camera.yaw * 180.0f / 3.1415926f, 0, 1, 0);
  glRotatef(180, 0, 1, 0);
  glTranslatef(-camera.pos[0], -camera.pos[1], -camera.pos[2]);

  renderSurfaces();

  for (s32 x = -MAXX; x <= MAXX; x++) {
    for (s32 z = -MAXZ; z <= MAXZ; z++) {
      if (*getSpot(x, z)) {
        Vec3f pos = { pendulumPos[0] + x + 0.5f, pendulumPos[1] + 1500, pendulumPos[2] + z + 0.5f };
        
        struct Surface *floor;
        f32 floorHeight = find_floor(pos[0], pos[1], pos[2], &floor);

        struct Surface *ceil;
        f32 ceilHeight = find_ceil(pos[0], floorHeight + 80.0f, pos[2], &ceil);

        glColor3f(1, 0.5f, 0);
        glBegin(GL_LINES);
        glVertex3f(pos[0], ceilHeight, pos[2]);
        glVertex3f(pos[0], pos[1], pos[2]);
        glEnd();
      }
    }
  }

  glfwSwapBuffers(window);
  glfwPollEvents();
}


GLFWwindow *openWindow(void) {
  glfwInit();

  GLFWwindow *window =
    glfwCreateWindow(960, 960, "Pendulum Squish Spots", NULL, NULL);
  glfwMakeContextCurrent(window);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  glLineWidth(1.0f);

  return window;
}


void loadPendulumModel(void) {
  FILE *seg7 = fopen("ttc_segment7.bin", "rb");

  u32 model = 0x0701508C; // pendulum
  // u32 model = 0x070158E0; // hand
  // u32 model = 0x070159D0; // spinner
  // u32 model = 0x07014F70; // cube
  fseek(seg7, model - 0x07000000, SEEK_SET);

  for (size_t i = 0; i < sizeof(pendulumModel) / 2; i++) {
    s16 val;
    val = fgetc(seg7) << 8;
    val |= fgetc(seg7);
    pendulumModel[i] = val;
  }

  fclose(seg7);
}


int main(void) {
  outputFile = fopen("output.csv", "wb");
  fprintf(outputFile, "pitch,yaw,roll,trunc x,trunc z,floor height,ceil height,floor ny,push angle,x,z\n");

  g = gstate_new(NULL);
  loadPendulumModel();
  g->currentObject = &g->objectPool[0];
  g->currentObject->collisionData = pendulumModel;
  g->currentObject->oFaceAngleRoll = 0;
  g->currentObject->oFaceAngleYaw = pendulumYaw;
  g->currentObject->oFaceAnglePitch = 0;
  g->currentObject->oPosX = pendulumPos[0];
  g->currentObject->oPosY = pendulumPos[1];
  g->currentObject->oPosZ = pendulumPos[2];
  load_area_terrain(0, levelData, NULL, NULL);

  GLFWwindow *window = openWindow();

  double accumTime = 0;
  double lastTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    double currentTime = glfwGetTime();
    accumTime += currentTime - lastTime;
    lastTime = currentTime;

    // while (accumTime >= 1.0/30) {
      clear_dynamic_surfaces();
      load_object_collision_model();
      findSpots();
      // if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS)
      //   g->currentObject->oFaceAngleRoll += 0x40;
      // if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS)
      //   g->currentObject->oFaceAngleRoll -= 0x40;
        g->currentObject->oFaceAngleRoll += 0x10;
      // g->currentObject->oFaceAngleRoll = -21788;

      if ((s16)g->currentObject->oFaceAngleRoll == 0) {
        break;
      }

      updateCamera(window);
      accumTime -= 1.0/30;
    // }

    render(window);
  }

  glfwTerminate();

  gstate_finalize(g);
  fclose(outputFile);
  return 0;
}
