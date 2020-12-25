#include "gstate.h"

#include <stdio.h>

// #include <ultra64.h>

// #include "sm64.h"
// #include "mario.h"
// #include "area.h"
// #include "camera.h"
// #include "mario_step.h"
// #include "math_util.h"
// #include "sound_init.h"

// TODO: Inline this when func_80251BF8 is decompiled.
// const f32 D_80335430[1] = {0.8660254f};

// TODO: Inline this as 0.8f when func_8025335C is decompiled.
// extern const f32 D_803354C0;

void func_80252070(struct MarioState *m)
{
    m->marioObj->OBJECT_FIELD_S32(0x22) = m->faceAngle[1];
    if (m->forwardVel > 0.0f) {
        //! ((s16)0x8000) has undefined behavior. Therefore, this downcast has
        // undefined behavior if m->floorAngle >= 0.
        s16 angleTemp = m->floorAngle + 0x8000;
        s16 faceAngleTemp = m->faceAngle[1] - angleTemp;
        f32 y = sins(faceAngleTemp) * m->forwardVel;
        f32 x = coss(faceAngleTemp) * m->forwardVel * 0.75f;
        m->forwardVel = sqrtf(y * y + x * x);
        m->faceAngle[1] = atan2s(x, y) + angleTemp;
    }
    drop_and_set_mario_action(m, ACT_STEEP_JUMP, 0);
}

static void func_8025219C(struct MarioState *m, f32 unk1, f32 unk2)
{
    m->vel[1] = unk1 + 0.0f + m->forwardVel * unk2;
    if (m->squishTimer != 0 || m->quicksandDepth > 1.0f) {
        m->vel[1] *= 0.5f;
    }
}

static u32 set_mario_action_airborne(struct MarioState *m, u32 action, u32 actionArg)
{
    f32 tmp;

    if (m->squishTimer != 0 || m->quicksandDepth >= 1.0f) {
        if (action == ACT_DOUBLE_JUMP || action == ACT_TWIRLING) {
            action = ACT_JUMP;
        }
    }

    switch (action) {
        case ACT_DOUBLE_JUMP:
            func_8025219C(m, 52.0f, 0.25f);
            m->forwardVel *= 0.8f;
            break;

        case ACT_BACKFLIP:
            // m->marioObj->header.gfx.unk38.animID = -1;
            m->forwardVel = -16.0f;
            func_8025219C(m, 62.0f, 0.0f);
            break;

        case ACT_TRIPLE_JUMP:
            func_8025219C(m, 69.0f, 0.0f);
            m->forwardVel *= 0.8f;
            break;

        case ACT_FLYING_TRIPLE_JUMP:
            func_8025219C(m, 82.0f, 0.0f);
            break;

        case ACT_WATER_JUMP:
        case ACT_HOLD_WATER_JUMP:
             if (actionArg == 0) {
                 func_8025219C(m, 42.0f, 0.0f);
             }
             break;

        case ACT_BURNING_JUMP:
             m->vel[1] = 31.5f;
             m->forwardVel = 8.0f;
             break;

        case ACT_RIDING_SHELL_JUMP:
            func_8025219C(m, 42.0f, 0.25f);
            break;

        case ACT_JUMP:
        case ACT_HOLD_JUMP:
            // m->marioObj->header.gfx.unk38.animID = -1;
            func_8025219C(m, 42.0f, 0.25f);
            m->forwardVel *= 0.8f;
            break;

        case ACT_WALL_KICK_AIR:
        case ACT_TOP_OF_POLE_JUMP:
            func_8025219C(m, 62.0f, 0.0f);
            if (m->forwardVel < 24.0f) {
                m->forwardVel = 24.0f;
            }
            m->wallKickTimer = 0;
            break;

        case ACT_SIDE_FLIP:
            func_8025219C(m, 62.0f, 0.0f);
            m->forwardVel = 8.0f;
            m->faceAngle[1] = m->intendedYaw;
            break;

        case ACT_STEEP_JUMP:
            // m->marioObj->header.gfx.unk38.animID = -1;
            func_8025219C(m, 42.0f, 0.25f);
            m->faceAngle[0] = -0x2000;
            break;

        case ACT_LAVA_BOOST:
            m->vel[1] = 84.0f;
            if (actionArg == 0) {
                m->forwardVel = 0.0f;
            }
            break;

        case ACT_DIVE:
            if ((tmp = m->forwardVel + 15.0f) > 48.0f) {
                tmp = 48.0f;
            }
            mario_set_forward_vel(m, tmp);
            break;

        case ACT_LONG_JUMP:
            // m->marioObj->header.gfx.unk38.animID = -1;
            func_8025219C(m, 30.0f, 0.0f);
            m->marioObj->oMarioLongJumpIsSlow = m->forwardVel > 16.0f ? FALSE : TRUE;
            if ((m->forwardVel *= 1.5f) > 48.0f) {
                m->forwardVel = 48.0f;
            }
            break;

        case ACT_SLIDE_KICK:
            m->vel[1] = 12.0f;
            if (m->forwardVel < 32.0f) {
                m->forwardVel = 32.0f;
            }
            break;

        case ACT_JUMP_KICK:
            m->vel[1] = 20.0f;
            break;
    }

    m->peakHeight = m->pos[1];
    m->flags |= MARIO_UNKNOWN_08;
    return action;
}

static u32 set_mario_action_moving(struct MarioState *m, u32 action, UNUSED u32 actionArg)
{
    s16 floorClass = mario_get_floor_class(m);
    f32 forwardVel = m->forwardVel;
    f32 mag = m->intendedMag <= 8.0f ? m->intendedMag : 8.0f;

    switch (action) {
        case ACT_WALKING:
            if (floorClass != SURFACE_CLASS_SLIDE) {
                if (0.0f <= forwardVel && forwardVel < mag) {
                    m->forwardVel = mag;
                }
            }
            m->marioObj->OBJECT_FIELD_S32(0x22) = 0;
            break;

        case ACT_HOLD_WALKING:
            if (0.0f <= forwardVel && forwardVel < mag / 2.0f) {
                m->forwardVel = mag / 2.0f;
            }
            break;

        case ACT_BEGIN_SLIDING:
            if (mario_facing_downhill(m, 0)) {
                action = ACT_BUTT_SLIDE;
            } else {
                action = ACT_STOMACH_SLIDE;
            }
            break;

        case ACT_HOLD_BEGIN_SLIDING:
            if (mario_facing_downhill(m, 0)) {
                action = ACT_HOLD_BUTT_SLIDE;
            } else {
                action = ACT_HOLD_STOMACH_SLIDE;
            }
            break;
    }

    return action;
}

static u32 set_mario_action_submerged(struct MarioState *m, u32 action, UNUSED u32 actionArg)
{
    if (action == ACT_METAL_WATER_JUMP || action == ACT_HOLD_METAL_WATER_JUMP) {
        m->vel[1] = 32.0f;
    }

    return action;
}

static u32 set_mario_action_cutscene(struct MarioState *m, u32 action, UNUSED u32 actionArg)
{
    switch (action) {
        case ACT_EMERGE_FROM_PIPE:
            m->vel[1] = 52.0f;
            break;

        case ACT_FALL_AFTER_STAR_GRAB:
            mario_set_forward_vel(m, 0.0f);
            break;

        case ACT_UNKNOWN_124:
            mario_set_forward_vel(m, 2.0f);
            break;

        case ACT_UNKNOWN_12B:
        case ACT_UNKNOWN_12C:
            m->vel[1] = 64.0f;
            break;
    }

    return action;
}

u32 set_mario_action(struct MarioState *m, u32 action, u32 actionArg)
{
    switch (action & ACT_GROUP_MASK) {
        case ACT_GROUP_MOVING:
            action = set_mario_action_moving(m, action, actionArg);
            break;

        case ACT_GROUP_AIRBORNE:
            action = set_mario_action_airborne(m, action, actionArg);
            break;

        case ACT_GROUP_SUBMERGED:
            action = set_mario_action_submerged(m, action, actionArg);
            break;

        case ACT_GROUP_CUTSCENE:
            action = set_mario_action_cutscene(m, action, actionArg);
            break;
    }

    m->flags &= ~(MARIO_UNKNOWN_16 | MARIO_UNKNOWN_17);
    if (!(m->action & ACT_FLAG_AIR)) {
        m->flags &= ~MARIO_UNKNOWN_18;
    }

    m->prevAction = m->action;
    m->action = action;
    m->actionArg = actionArg;
    m->actionState = 0;
    m->actionTimer = 0;
    return TRUE;
}

void mario_set_forward_vel(struct MarioState *m, f32 forwardVel)
{
    m->forwardVel = forwardVel;
    m->slideVelX = sins(m->faceAngle[1]) * m->forwardVel;
    m->slideVelZ = coss(m->faceAngle[1]) * m->forwardVel;
    m->vel[0] = m->slideVelX;
    m->vel[2] = m->slideVelZ;
}

s32 drop_and_set_mario_action(struct MarioState *m, u32 action, u32 actionArg)
{
    mario_stop_riding_and_holding(m);
    return set_mario_action(m, action, actionArg);
}

s16 mario_get_floor_class(struct MarioState *m)
{
    s16 class;
    // if ((m->area->v02 & 0x0007) == 6)
    //     class = SURFACE_CLASS_SLIDE;
    // else
        class = SURFACE_CLASS_DEFAULT;

    if (m->floor != NULL)
    {
        switch (m->floor->type)
        {
        case SURFACE_0073: class = SURFACE_CLASS_SLIDE; break;
        case SURFACE_0074: class = SURFACE_CLASS_SLIDE; break;
        case SURFACE_0075: class = SURFACE_CLASS_SLIDE; break;
        case SURFACE_0078: class = SURFACE_CLASS_SLIDE; break;
        case SURFACE_0079: class = SURFACE_CLASS_SLIPPERY; break;
        case SURFACE_007A: class = SURFACE_CLASS_NOT_SLIPPERY; break;
        case SURFACE_0013: class = SURFACE_CLASS_SLIDE; break;
        case SURFACE_0014: class = SURFACE_CLASS_SLIPPERY; break;
        case SURFACE_0015: class = SURFACE_CLASS_NOT_SLIPPERY; break;
        case SURFACE_002A: class = SURFACE_CLASS_SLIPPERY; break;
        case SURFACE_002E: class = SURFACE_CLASS_SLIDE; break;
        case SURFACE_0035: class = SURFACE_CLASS_SLIPPERY; break;
        case SURFACE_0036: class = SURFACE_CLASS_SLIDE; break;
        case SURFACE_0037: class = SURFACE_CLASS_NOT_SLIPPERY; break;
        }
    }

    if (m->action == ACT_CRAWLING && m->floor->normal[1] > 0.5f && class == SURFACE_CLASS_SLIDE)
        class = SURFACE_CLASS_NOT_SLIPPERY;

    return class;
}

s32 mario_facing_downhill(struct MarioState *m, s32 useForwardVel)
{
    s16 yaw = m->faceAngle[1];
    if (useForwardVel && m->forwardVel < 0.0f)
        yaw += 0x8000;

    s16 dyaw = m->floorAngle - yaw;
    return dyaw > -0x4000 && dyaw < 0x4000;
}

struct Surface *func_8025181C(Vec3f pos, f32 offsetY, f32 radius)
{
    struct WallCollisionData colData;
    colData.x = pos[0];
    colData.y = pos[1];
    colData.z = pos[2];
    colData.offsetY = offsetY;
    colData.radius = radius;

    struct Surface *wall = NULL;
    if (find_wall_collisions(&colData) != 0)
        wall = colData.walls[colData.numWalls - 1];

    pos[0] = colData.x;
    pos[1] = colData.y;
    pos[2] = colData.z;
    return wall;
}

f32 func_802518D0(Vec3f pos, f32 height, struct Surface **pceil)
{
    return find_ceil(pos[0], height + 80.0f, pos[2], pceil);
}

s32 mario_floor_is_slope(struct MarioState *m)
{
    // if ((m->area->v02 & 0x0007) == 0x0006 && m->floor->normal.y < 0.9998477f)
    //     return TRUE;

    f32 threshold;
    switch (mario_get_floor_class(m)) {
    case SURFACE_CLASS_SLIDE: threshold = 0.9848077f; break;
    case SURFACE_CLASS_SLIPPERY: threshold = 0.9396926f; break;
    case SURFACE_CLASS_NOT_SLIPPERY: threshold = 0.0f;      break;
    default:     threshold = 0.7880108f; break;
    }

    return m->floor->normal[1] <= threshold;
}

s32 func_80252C30(struct MarioState *m)
{
    if (m->quicksandDepth >= 11.0f)
    {
        if (m->heldObj == NULL)
            return set_mario_action(m, ACT_QUICKSAND_JUMP_LAND, 0);
        else
            return set_mario_action(m, ACT_HOLD_QUICKSAND_JUMP_LAND, 0);
    }

    // if (func_80251BF8(m))
    //     func_80252070(m);

    if (m->doubleJumpTimer == 0 || m->squishTimer != 0)
    {
        set_mario_action(m, ACT_JUMP, 0);
    }
    else
    {
        switch (m->prevAction) {
        case ACT_JUMP_LAND:
            set_mario_action(m, ACT_DOUBLE_JUMP, 0);
            break;

        case ACT_FREEFALL_LAND:
            set_mario_action(m, ACT_DOUBLE_JUMP, 0);
            break;

        case ACT_DOUBLE_JUMP_LAND:
            if (m->flags & MARIO_WING_CAP)
                set_mario_action(m, ACT_FLYING_TRIPLE_JUMP, 0);
            else if (m->forwardVel > 20.0f)
                set_mario_action(m, ACT_TRIPLE_JUMP, 0);
            else
                set_mario_action(m, ACT_JUMP, 0);
            break;

        case ACT_SIDE_FLIP_LAND_STOP:
            set_mario_action(m, ACT_DOUBLE_JUMP, 0);
            break;

        default:
            set_mario_action(m, ACT_JUMP, 0);
            break;
        }
    }

    m->doubleJumpTimer = 0;
    return TRUE;
}

s32 func_80252E74(struct MarioState *m, u32 action, u32 actionArg)
{
    if (m->quicksandDepth >= 11.0f)
    {
        if (m->heldObj == NULL)
            return set_mario_action(m, ACT_QUICKSAND_JUMP_LAND, 0);
        else
            return set_mario_action(m, ACT_HOLD_QUICKSAND_JUMP_LAND, 0);
    }

    // if (p80251BF8(m) != 0)
    //     jumpOnSteepSlope(m);
    // else
        set_mario_action(m, action, actionArg);
    return TRUE;
}

s32 func_80252FEC(struct MarioState *m)
{
    if (m->input & INPUT_A_PRESSED)
        return set_mario_action(m, ACT_JUMP, 0);

    if (m->input & INPUT_OFF_FLOOR)
        return set_mario_action(m, ACT_FREEFALL, 0);

    if (m->input & INPUT_NONZERO_ANALOG)
        return set_mario_action(m, ACT_WALKING, 0);

    if (m->input & INPUT_ABOVE_SLIDE)
        return set_mario_action(m, ACT_BEGIN_SLIDING, 0);

    return FALSE;
}

s32 func_802519A8(struct MarioState *m)
{
    // if ((m->area->v02 & 0x0007) == 0x0006 && m->floor->normal.y < v80335400)
    //     return TRUE;

    f32 threshold;
    switch (mario_get_floor_class(m)) {
    case SURFACE_CLASS_SLIDE: threshold = 0.9848077f; break;
    case SURFACE_CLASS_SLIPPERY: threshold = 0.9396926f; break;
    case SURFACE_CLASS_NOT_SLIPPERY: threshold = 0.0f;      break;
    default:     threshold = 0.7880108f; break;
    }

    return m->floor->normal[1] <= threshold;
}

static void setMarioInputABZ(struct MarioState *m) {
  if (m->controller->buttonPressed & A_BUTTON)
    m->input |= INPUT_A_PRESSED;

  if (m->controller->buttonDown & A_BUTTON)
    m->input |= INPUT_A_DOWN;
  
  if (m->squishTimer == 0) {
    if (m->controller->buttonPressed & B_BUTTON)
      m->input |= INPUT_B_PRESSED;
    
    if (m->controller->buttonDown & Z_TRIG)
      m->input |= INPUT_Z_DOWN;
    
    if (m->controller->buttonPressed & Z_TRIG)
      m->input |= INPUT_Z_PRESSED;
  }
  
  if (m->input & INPUT_A_PRESSED)
    m->framesSinceA = 0;
  else if (m->framesSinceA < 255)
    m->framesSinceA += 1;
  
  if (m->input & INPUT_B_PRESSED)
    m->framesSinceB = 0;
  else if (m->framesSinceB < 255)
    m->framesSinceB += 1;
}

static void setMarioInputAnalog(struct MarioState *m) {
  struct Controller *inp = m->controller;
  f32 squareMag = (inp->stickMag / 64.0f) * (inp->stickMag / 64.0f) * 64.0f;
  
  if (m->squishTimer == 0)
    m->intendedMag = squareMag / 2.0f;
  else
    m->intendedMag = squareMag / 8.0f;
  
  if (m->intendedMag > 0.0f) {
    m->intendedYaw = inp->intendedYaw; //atan2xy(inp->stickY, inp->stickX) + m->area->v24->v02;
    m->input |= INPUT_NONZERO_ANALOG;
  }
  else {
    m->intendedYaw = m->faceAngle[1];
  }
}

static void setMarioInputSurfaces(struct MarioState *m) {
  // TODO: Look into sub-update movement via walls (since these numbers
  // may differ from quarter step wall resolution)
  resolve_wall_collisions(&m->pos[0], &m->pos[1], &m->pos[2], 60.0f, 50.0f);
  resolve_wall_collisions(&m->pos[0], &m->pos[1], &m->pos[2], 30.0f, 24.0f);
  //! Walls are persistent. Any usages or nullity checks before the first
  // quarter step of a frame are manipulatable.
  
  m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);
  if (m->floor == NULL) {
    //! Couldn't this be a bit offset by e.g. quicksand, riding shell air?
    vec3f_copy(m->pos, m->marioObj->header.gfx.pos);
    m->floorHeight = find_floor(m->pos[0], m->pos[1], m->pos[2], &m->floor);
  }

  m->ceilHeight = func_802518D0(m->pos, m->floorHeight, &m->ceil);
  // f32 val = p80381D3C(m->pos.x, m->pos.z);
  m->waterLevel = find_water_level(m->pos[0], m->pos[2]);

  if (m->floor == NULL) {
  //   p8024A860(m, 0x12);
    return;
  }

  //! Look into floor angle manip. It is usually only computed here.
  m->floorAngle = atan2s(m->floor->normal[2], m->floor->normal[0]);
  // m->v14 = p8025167C(m);

  //! HSW - standing between waterLevel-100 and waterLevel-40 prevents sliding
  if (m->pos[1] > m->waterLevel - 40 && func_802519A8(m))
    m->input |= INPUT_ABOVE_SLIDE;

  if ((m->floor->flags & SURFACE_FLAG_DYNAMIC) || (m->ceil != NULL && (m->ceil->flags & SURFACE_FLAG_DYNAMIC))) {
    f32 room = m->ceilHeight - m->floorHeight;
    if (room >= 0.0f && room <= 150.0f)
      m->input |= INPUT_SQUISHED;
  }

  if (m->pos[1] > m->floorHeight + 100.0f)
    m->input |= INPUT_OFF_FLOOR;

  if (m->pos[1] < m->waterLevel - 10)
    m->input |= INPUT_IN_WATER;

  // if (m->pos[1] < val - 100.0f)
  //   m->input |= input_unknown_8;
}

static void setMarioInput(struct MarioState *m) {
  m->particleFlags = 0;
  m->input = 0;
  // m->collidedObjTypes = m->marioObj->collidedObjTypes;
  m->flags &= 0x00FFFFFF;

  setMarioInputABZ(m);
  setMarioInputAnalog(m);
  setMarioInputSurfaces(m);
  // debugFloorAngle(m);
  
  // if (v8033B4D8 & 0x2000) {
  //   if (m->action & maf_unknown_04000000)
  //     m->input |= input_first_person;
  //   else
  //     v8033B4D8 &= ~0x2000;
  // }
  
  if (!(m->input & (INPUT_NONZERO_ANALOG | INPUT_A_PRESSED)))
    m->input |= INPUT_UNKNOWN_5;
  
  if (m->marioObj->oInteractStatus & 0x00000013)
    m->input |= INPUT_UNKNOWN_10;
  
  // nop80254E3C(m);
  if (m->wallKickTimer > 0)
    m->wallKickTimer -= 1;
  if (m->doubleJumpTimer > 0)
    m->doubleJumpTimer -= 1;
}

u32 updateMario(struct Object *obj) {
  if (g->marioState->action == ACT_UNINITIALIZED) return 0;
  // marioState->marioObj->g.graphicsFlags &= ~0x0010;

  // p8025410C(marioState);
  setMarioInput(g->marioState);
  // p80250624(marioState);
  // handleObjCollisions(marioState);
  
  if (g->marioState->floor == NULL) return 0;

  s32 cont = TRUE;
  while (cont) {
    u32 startAction = g->marioState->action;

    // temporary idle hack
    if (g->marioState->action == ACT_IDLE) {
        struct MarioState *m = g->marioState;
        if (m->input & INPUT_NONZERO_ANALOG) {
            m->faceAngle[1] = m->intendedYaw;
            cont = set_mario_action(m, ACT_WALKING, 0);
        }
        else {
            cont = FALSE;
        }
        continue;
    }

    switch (g->marioState->action & ACT_GROUP_MASK) {
    // case ACT_GROUP_STATIONARY: cont = mario_execute_stationary_action(g->marioState); break;
    case ACT_GROUP_MOVING: cont = mario_execute_moving_action(g->marioState); break;
    case ACT_GROUP_AIRBORNE: cont = mario_execute_airborne_action(g->marioState); break;
    // case ACT_GROUP_SUBMERGED: cont = mario_execute_submerged_action(g->marioState); break;
    // case ACT_GROUP_CUTSCENE: cont = mario_execute_cutscene_action(g->marioState); break;
    // case ACT_GROUP_AUTOMATIC: cont = mario_execute_automatic_action(g->marioState); break;
    // case ACT_GROUP_OBJECT: cont = mario_execute_object_action(g->marioState); break;
    }

    // if (cont && g->marioState->action == startAction) {
    //     // printf("%08X\n", g->marioState->action);
    //     // g->error = "Unhandled action";
    //     return 0;
    // }
  }
  
  // p80254164(marioState);
  // p8025335C(marioState);
  // p80253C94(marioState);
  // updateMarioHealth(marioState);
  // p80254088(marioState);
  // p8025435C(marioState);

  // if (marioState->floor->type == surface_002C)
  //   p802ADC20(0, (s32) marioState->floor->v02 << 8);

  // if (marioState->floor->type == surface_0038)
  //   p802ADC20(1, 0);
  
  // p80249040();
  // marioState->marioObj->interactStatus = 0;
  // return g->marioState->particleFlags;
  return 0;
}
