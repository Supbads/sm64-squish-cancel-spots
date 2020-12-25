#ifndef _SM64_SM64_H
#define _SM64_SM64_H


#define UNUSED

#define TRUE 1
#define FALSE 0

typedef int8_t s8;
typedef uint8_t u8;
typedef int16_t s16;
typedef uint16_t u16;
typedef int32_t s32;
typedef uint32_t u32;
typedef int64_t s64;
typedef uint64_t u64;

typedef volatile u8   vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8   vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;

typedef float  f32;
typedef double f64;

typedef s32 Mtx_t[4][4];
typedef union
{
    Mtx_t m;
    s64 forc_structure_alignment;
} Mtx;


typedef f32 Vec2f[2];
typedef f32 Vec3f[3];
typedef s16 Vec3s[3];
typedef s32 Vec3i[3];
typedef f32 Vec4f[4];
typedef s16 Vec4s[4];

typedef f32 Mat4[4][4];


#define A_BUTTON                (1 << 15) // 0x8000
#define B_BUTTON                (1 << 14) // 0x4000
#define Z_TRIG                  (1 << 13) // 0x2000
#define START_BUTTON            (1 << 12) // 0x1000
#define U_JPAD                  (1 << 11) // 0x0800
#define D_JPAD                  (1 << 10) // 0x0400
#define L_JPAD                  (1 << 9)  // 0x0200
#define R_JPAD                  (1 << 8)  // 0x0100
#define DUMMY_1                 (1 << 7)  // 0x0080 (unused)
#define DUMMY_2                 (1 << 6)  // 0x0040 (unused)
#define L_TRIG                  (1 << 5)  // 0x0020
#define R_TRIG                  (1 << 4)  // 0x0010
#define U_CBUTTONS              (1 << 3)  // 0x0008
#define D_CBUTTONS              (1 << 2)  // 0x0004
#define L_CBUTTONS              (1 << 1)  // 0x0002
#define R_CBUTTONS              (1 << 0)  // 0x0001


struct Surface
{
    /*0x00*/ s16 type;
    /*0x02*/ s16 force;
    /*0x04*/ s8 flags;
    /*0x05*/ s8 room;
    /*0x06*/ s16 lowerY;
    /*0x08*/ s16 upperY;
    /*0x0A*/ Vec3s vertex1;
    /*0x10*/ Vec3s vertex2;
    /*0x16*/ Vec3s vertex3;
    /*0x1C*/ Vec3f normal;
    /*0x28*/ f32 originOffset;
    /*0x2C*/ struct Object *object;
    s32 mark;
};

#define SURFACE_LAVA     0x0001
#define SURFACE_0004     0x0004
#define SURFACE_HANGABLE 0x0005
#define SURFACE_0009     0x0009
#define SURFACE_000A     0x000A
#define SURFACE_000E     0x000E
#define SURFACE_0012     0x0012
#define SURFACE_0013     0x0013
#define SURFACE_0014     0x0014
#define SURFACE_0015     0x0015
#define SURFACE_001B     0x001B
#define SURFACE_0020     0x0020
#define SURFACE_0021     0x0021
#define SURFACE_0022     0x0022
#define SURFACE_0023     0x0023
#define SURFACE_0024     0x0024
#define SURFACE_0025     0x0025
#define SURFACE_0026     0x0026
#define SURFACE_0027     0x0027
#define SURFACE_0029     0x0029
#define SURFACE_002A     0x002A
#define SURFACE_002C     0x002C
#define SURFACE_002D     0x002D
#define SURFACE_002E     0x002E
#define SURFACE_002F     0x002F
#define SURFACE_0030     0x0030
#define SURFACE_0032     0x0032
#define SURFACE_0033     0x0033
#define SURFACE_0034     0x0034
#define SURFACE_0035     0x0035
#define SURFACE_0036     0x0036
#define SURFACE_0037     0x0037
#define SURFACE_0038     0x0038
#define SURFACE_0072     0x0072
#define SURFACE_0073     0x0073
#define SURFACE_0074     0x0074
#define SURFACE_0075     0x0075
#define SURFACE_0076     0x0076
#define SURFACE_0077     0x0077
#define SURFACE_0078     0x0078
#define SURFACE_0079     0x0079
#define SURFACE_007A     0x007A
#define SURFACE_007B     0x007B
#define SURFACE_00D3     0x00D3

#define SURFACE_CLASS_SLIDE        0x0013
#define SURFACE_CLASS_SLIPPERY     0x0014
#define SURFACE_CLASS_NOT_SLIPPERY 0x0015
#define SURFACE_CLASS_DEFAULT      0x0000

#define SURFACE_FLAG_DYNAMIC      (1 << 0)
#define SURFACE_FLAG_1            (1 << 1)
#define SURFACE_FLAG_X_PROJECTION (1 << 3)




struct UnknownStruct5 {
    /*0x00*/ s16 unk00; // flags
    /*0x02*/ s16 unk02;
    /*0x04*/ s16 unk04;
    /*0x06*/ s16 unk06;
    /*0x08*/ s16 unk08;
    /*0x0C*/ void *unk0C;
    /*0x10*/ void *unk10;
};

struct GraphNode
{
    /*0x00*/ s16 type; // structure type
    /*0x02*/ s16 flags; // hi = drawing layer, lo = rendering modes
    /*0x04*/ struct GraphNode *prev;
    /*0x08*/ struct GraphNode *next;
    /*0x0C*/ struct GraphNode *parent;
    /*0x10*/ struct GraphNode *children;
};

// struct AnimInfo?
struct GraphNodeObject_sub
{
    /*0x00 0x38*/ s16 animID;
    /*0x02 0x3A*/ s16 animYTrans;
    /*0x04 0x3C*/ struct UnknownStruct5 *curAnim;
    /*0x08 0x40*/ s16 animFrame;
    /*0x0A 0x42*/ u16 animTimer;
    /*0x0C 0x44*/ s32 animFrameAccelAssist;
    /*0x10 0x48*/ s32 animAccel;
};

// TODO this is the first member of ObjectNode/Object
struct GraphNodeObject
{
    /*0x00*/ struct GraphNode node;
    /*0x14*/ struct GraphNode *asGraphNode;
    /*0x18*/ s8 unk18;
    /*0x19*/ s8 unk19;
    /*0x1A*/ Vec3s angle;
    /*0x20*/ Vec3f pos;
    /*0x2C*/ Vec3f scale;
    /*0x38*/ struct GraphNodeObject_sub unk38;
    // /*0x4C*/ struct SpawnInfo *unk4C;
    /*0x50*/ void *throwMatrix; // matrix ptr
    /*0x54*/ f32 unk54;
    /*0x58*/ f32 unk58;
    /*0x5C*/ f32 unk5C;
};

struct ObjectNode
{
    struct GraphNodeObject gfx;
    struct ObjectNode *next;
    struct ObjectNode *prev;
};

struct Object
{
    /*0x000*/ struct ObjectNode header;
    /*0x068*/ struct Object *parentObj;
    /*0x06C*/ struct Object *prevObj;
    /*0x070*/ u32 collidedObjInteractTypes;
    /*0x074*/ s16 active;
    /*0x076*/ s16 numCollidedObjs;
    /*0x078*/ struct Object *collidedObjs[4];
    /*0x088*/
    union
    {
        // Object fields. See object_fields.h.
        u32 asU32[0x50];
        s32 asS32[0x50];
        f32 asF32[0x50];
        s16 asS16[2 * 0x50];
        void *asVoidP[0x50];
        s16 *asS16P[0x50];
        s32 *asS32P[0x50];
        u32 *asAnims[0x50];
        // struct ChainSegment *asChainSegment[0x50];
        struct Object *asObject[0x50];
        void *asVoidPtr[0x50];
    } rawData;
    /*0x1C8*/ u32 unk1C8;
    /*0x1CC*/ u32 *behScript;
    /*0x1D0*/ u32 stackIndex;
    /*0x1D4*/ u32 stack[8];
    /*0x1F4*/ s16 unk1F4;
    /*0x1F6*/ s16 unk1F6;
    /*0x1F8*/ f32 hitboxRadius;
    /*0x1FC*/ f32 hitboxHeight;
    /*0x200*/ f32 unk200;
    /*0x204*/ f32 unk204;
    /*0x208*/ f32 unk208;
    /*0x20C*/ void *behavior;
    /*0x210*/ u32 unk210;
    /*0x214*/ struct Object *platform;
    /*0x218*/ void *collisionData;
    /*0x21C*/ float unk21C[4][4];
    /*0x25C*/ void *unk25C;
};


struct Controller
{
    /*0x00*/ s16 rawStickX;       //
    /*0x02*/ s16 rawStickY;       //
    /*0x04*/ float stickX;        // [-64, 64] positive is right
    /*0x08*/ float stickY;        // [-64, 64] positive is up
    /*0x0C*/ float stickMag;      // distance from center [0, 64]
    /*0x10*/ u16 buttonDown;
    /*0x12*/ u16 buttonPressed;
    // /*0x14*/ OSContStatus* statusData;
    // /*0x18*/ OSContPad *controllerData;]
    s16 intendedYaw;
    u32 buttonsDownPrev;
};




#include "surface_collision.h"
#include "surface_load.h"
#include "mario.h"
#include "object_fields.h"
#include "math_util.h"
#include "interaction.h"
#include "mario_step.h"
#include "mario_actions_moving.h"
#include "mario_actions_airborne.h"
#include "mario_actions_object.h"


extern f32 sqrtf(f32 x);


#endif
