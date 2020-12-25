#ifndef _SM64_GSTATE_H
#define _SM64_GSTATE_H


#include <stdint.h>
#include <stddef.h>


#ifdef __cplusplus
extern "C"
{
#endif


#include "sm64.h"


struct GState
{
    u32 globalTimer;
    s32 surfaceNodesAllocated;
    s32 surfacesAllocated;
    struct SurfaceNode surfaceNodePool[7000];
    struct Surface surfacePool[2300];
    s32 numStaticSurfaceNodes;
    s32 numStaticSurfaces;
    SpatialPartitionCell staticSurfacePartition[16][16];
    SpatialPartitionCell dynamicSurfacePartition[16][16];
    struct MarioState marioStates[1];
    struct MarioState *marioState;
    struct Object objectPool[240];
    struct Controller controllers[3];
    struct Object *currentObject;

#ifdef GSTATE_EXTRA
    GSTATE_EXTRA
#endif
};

struct GInput
{
    f32 stickMag;
    s16 intendedDYaw;
    u32 buttonsDown;
};

extern struct GState *g;


struct GState *gstate_new(struct GState *src);
void gstate_copy(struct GState *st, struct GState *src);
void gstate_finalize(struct GState *st);
void gstate_advance(struct GState *st, struct GInput *in);


#ifdef __cplusplus
}
#endif

#endif
