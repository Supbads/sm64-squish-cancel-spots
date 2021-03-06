#ifndef _SM64_OBJECT_FIELDS_H
#define _SM64_OBJECT_FIELDS_H


/**
 * The array [0x88, 0x1C8) in struct Object consists of fields that can vary by
 * object type. These macros provide access to these fields.
 */


#define OBJECT_FIELD_U32(index) rawData.asU32[index]
#define OBJECT_FIELD_S32(index) rawData.asS32[index]
#define OBJECT_FIELD_F32(index) rawData.asF32[index]
#define OBJECT_FIELD_S16(index) rawData.asS16[index]
#define OBJECT_FIELD_VOIDP(index) rawData.asVoidP[index]
#define OBJECT_FIELD_S16P(index) rawData.asS16P[index]
#define OBJECT_FIELD_S32P(index) rawData.asS32P[index]
#define OBJECT_FIELD_ANIMS(index) rawData.asAnims[index]
// #define OBJECT_FIELD_CHAIN_SEGMENT(index) rawData.asChainSegment[index]
#define OBJECT_FIELD_OBJECT(index) rawData.asObject[index]
#define OBJECT_FIELD_VPTR(index) rawData.asVoidPtr[index]


/* Common fields */
#define /*0x088*/ oUnk88 OBJECT_FIELD_S32(0x00)
#define /*0x08C*/ oFlags OBJECT_FIELD_U32(0x01)
#define /*0x09C*/ oCollectable OBJECT_FIELD_S32(0x05)
#define /*0x0A0*/ oPosX OBJECT_FIELD_F32(0x06)
#define /*0x0A4*/ oPosY OBJECT_FIELD_F32(0x07)
#define /*0x0A8*/ oPosZ OBJECT_FIELD_F32(0x08)
#define /*0x0AC*/ oVelX OBJECT_FIELD_F32(0x09)
#define /*0x0B0*/ oVelY OBJECT_FIELD_F32(0x0A)
#define /*0x0B4*/ oVelZ OBJECT_FIELD_F32(0x0B)
#define /*0x0B8*/ oForwardVel OBJECT_FIELD_F32(0x0C)
#define /*0x0C4*/ oAnglePitch OBJECT_FIELD_S32(0x0F)
#define /*0x0C8*/ oAngleYaw OBJECT_FIELD_S32(0x10)
#define /*0x0CC*/ oAngleRoll OBJECT_FIELD_S32(0x11)
#define /*0x0D0*/ oFaceAnglePitch OBJECT_FIELD_S32(0x12)
#define /*0x0D4*/ oFaceAngleYaw OBJECT_FIELD_S32(0x13)
#define /*0x0D8*/ oFaceAngleRoll OBJECT_FIELD_S32(0x14)
#define /*0x0DC*/ oGraphYOffset OBJECT_FIELD_F32(0x15)
#define /*0x0E0*/ oUnkE0 OBJECT_FIELD_U32(0x16)
#define /*0x0E4*/ oGravity OBJECT_FIELD_F32(0x17)
#define /*0x0E8*/ oUnkE8 OBJECT_FIELD_F32(0x18)
#define /*0x0EC*/ oUnkEC OBJECT_FIELD_U32(0x19)
#define /*0x0F0*/ oAnimState OBJECT_FIELD_S32(0x1A)
// 0x0F4-0x110 (0x1B-0x22) are object specific and defined below the common fields.
#define /*0x114*/ oAngleVelPitch OBJECT_FIELD_S32(0x23)
#define /*0x118*/ oAngleVelYaw OBJECT_FIELD_S32(0x24)
#define /*0x11C*/ oAngleVelRoll OBJECT_FIELD_S32(0x25)
#define /*0x120*/ oAnimations OBJECT_FIELD_ANIMS(0x26)
#define /*0x124*/ oHeldState OBJECT_FIELD_U32(0x27)
#define /*0x128*/ oUnk128 OBJECT_FIELD_F32(0x28)
#define /*0x12C*/ oUnk12C OBJECT_FIELD_F32(0x29)
#define /*0x130*/ oInteractType OBJECT_FIELD_U32(0x2A)
#define /*0x134*/ oInteractStatus OBJECT_FIELD_U32(0x2B)
#define /*0x138*/ oUnk138 OBJECT_FIELD_F32(0x2C)
#define /*0x13C*/ oUnk13C OBJECT_FIELD_F32(0x2D)
#define /*0x140*/ oUnk140 OBJECT_FIELD_F32(0x2E)
#define /*0x144*/ oBehParam OBJECT_FIELD_S32(0x2F)
#define /*0x14C*/ oAction OBJECT_FIELD_S32(0x31)
#define /*0x150*/ oUnk150 OBJECT_FIELD_U32(0x32)
#define /*0x154*/ oTimer OBJECT_FIELD_S32(0x33)
#define /*0x158*/ oUnk158 OBJECT_FIELD_F32(0x34)
#define /*0x15C*/ oDistanceToMario OBJECT_FIELD_F32(0x35)
#define /*0x160*/ oAngleToMario OBJECT_FIELD_S32(0x36)
#define /*0x164*/ oHomeX OBJECT_FIELD_F32(0x37)
#define /*0x168*/ oHomeY OBJECT_FIELD_F32(0x38)
#define /*0x16C*/ oHomeZ OBJECT_FIELD_F32(0x39)
#define /*0x170*/ oFriction OBJECT_FIELD_F32(0x3A)
#define /*0x174*/ oBuoyancy OBJECT_FIELD_F32(0x3B)
#define /*0x178*/ oSoundStateID OBJECT_FIELD_S32(0x3C)
#define /*0x17C*/ oOpacity OBJECT_FIELD_S32(0x3D)
#define /*0x180*/ oUnk180 OBJECT_FIELD_S32(0x3E)
#define /*0x184*/ oUnk184 OBJECT_FIELD_S32(0x3F)
#define /*0x188*/ oUnk188 OBJECT_FIELD_S32(0x40)
#define /*0x18C*/ oPrevAction OBJECT_FIELD_S32(0x41)
#define /*0x190*/ oUnk190 OBJECT_FIELD_U32(0x42)
#define /*0x194*/ oCollisionDistance OBJECT_FIELD_F32(0x43)
#define /*0x198*/ oUnk198 OBJECT_FIELD_S32(0x44)
#define /*0x19C*/ oDrawingDistance OBJECT_FIELD_F32(0x45)
#define /*0x1A0*/ oUnk1A0 OBJECT_FIELD_U32(0x46)
#define /*0x1A8*/ oUnk1A8 OBJECT_FIELD_U32(0x48)
#define /*0x1AC*/ oUnk1AC OBJECT_FIELD_S16(0x49 * 2)
#define /*0x1AE*/ oUnk1AE OBJECT_FIELD_S16(0x49 * 2 + 1)
#define /*0x1B0*/ oUnk1B0 OBJECT_FIELD_S16(0x4A * 2)
#define /*0x1C4*/ oUnk1C4 OBJECT_FIELD_S32(0x4F)

/* Mario */
#define /*0x0F4*/ oMarioParticleFlags OBJECT_FIELD_S32(0x1B)
#define /*0x108*/ oMarioPoleUnk108 OBJECT_FIELD_S32(0x20)
#define /*0x108*/ oMarioReadingSignDYaw OBJECT_FIELD_S32(0x20)
#define /*0x10C*/ oMarioPoleYawVel OBJECT_FIELD_S32(0x21)
#define /*0x10C*/ oMarioCannonObjectYaw OBJECT_FIELD_S32(0x21)
#define /*0x10C*/ oMarioTornadoYawVel OBJECT_FIELD_S32(0x21)
#define /*0x10C*/ oMarioReadingSignDPosX OBJECT_FIELD_F32(0x21)
#define /*0x110*/ oMarioPolePos OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioCannonInputYaw OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioTornadoPosY OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioReadingSignDPosZ OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioWhirlpoolPosY OBJECT_FIELD_F32(0x22)
#define /*0x110*/ oMarioBurnTimer OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioLongJumpIsSlow OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioSteepJumpYaw OBJECT_FIELD_S32(0x22)
#define /*0x110*/ oMarioWalkingPitch OBJECT_FIELD_S32(0x22)


#endif
