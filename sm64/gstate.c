#include "gstate.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


struct GState *g = NULL;


#define _COPYPTR(ptr, fromSt, toSt) \
    ((__typeof__(ptr))((u8 *)(ptr) - (u8 *)(fromSt) + (u8 *)(toSt)))
#define COPYPTR(ptr, fromSt, toSt) \
    ((ptr) == NULL ? NULL : _COPYPTR(ptr, fromSt, toSt))
#define FIXPTR(lptr, fromSt, toSt) \
    do { lptr = COPYPTR(lptr, fromSt, toSt); } while (0)


struct GState *gstate_new(struct GState *src)
{
    // TODO: Check for null return
    struct GState *st = calloc(1, sizeof(struct GState));

    if (src != NULL)
    {
        gstate_copy(st, src);
    }
    else
    {
        st->marioState = &st->marioStates[0];
        st->marioState->marioObj = &st->objectPool[0];
        st->marioState->controller = &st->controllers[0];
        st->marioState->health = 0x880;
    }

    return st;
}

void gstate_copy(struct GState *st, struct GState *src)
{
    memcpy(st, src, sizeof(struct GState));
 
    for (s32 i = 0; i < st->surfaceNodesAllocated; i++)
    {
        FIXPTR(st->surfaceNodePool[i].next, src, st);
        FIXPTR(st->surfaceNodePool[i].surface, src, st);
    }

    for (s32 i = 0; i < 16; i++)
    {
        for (s32 j = 0; j < 16; j++)
        {
            for (s32 k = 0; k < 3; k++)
            {
                FIXPTR(st->staticSurfacePartition[i][j][k].next, src, st);
                FIXPTR(st->staticSurfacePartition[i][j][k].surface, src, st);
                FIXPTR(st->dynamicSurfacePartition[i][j][k].next, src, st);
                FIXPTR(st->dynamicSurfacePartition[i][j][k].surface, src, st);
            }
        }
    }

    FIXPTR(st->marioState, src, st);
    FIXPTR(st->marioState->wall, src, st);
    FIXPTR(st->marioState->ceil, src, st);
    FIXPTR(st->marioState->floor, src, st);
    FIXPTR(st->marioState->interactObj, src, st);
    FIXPTR(st->marioState->heldObj, src, st);
    FIXPTR(st->marioState->usedObj, src, st);
    FIXPTR(st->marioState->riddenObj, src, st);
    FIXPTR(st->marioState->marioObj, src, st);
    FIXPTR(st->marioState->controller, src, st);
}

void gstate_finalize(struct GState *st)
{
    // free(st);
}

void gstate_advance(struct GState *st, struct GInput *in)
{
    struct Controller *c = st->marioState->controller;

    c->stickMag = in->stickMag;
    c->intendedYaw = st->marioState->faceAngle[1] + in->intendedDYaw;
    c->buttonDown = in->buttonsDown;
    c->buttonPressed = c->buttonDown & (c->buttonDown ^ c->buttonsDownPrev);

    g = st;
    updateMario(NULL);
    g = NULL;

    c->buttonsDownPrev = in->buttonsDown;
}
