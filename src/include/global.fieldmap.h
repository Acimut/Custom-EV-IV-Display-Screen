#ifndef GUARD_GLOBAL_FIELDMAP_H
#define GUARD_GLOBAL_FIELDMAP_H

#define OBJECT_EVENTS_COUNT 16

#define METATILE_COLLISION_MASK 0x0C00
#define METATILE_ID_MASK 0x03FF
#define METATILE_ID_UNDEFINED 0x03FF
#define METATILE_ELEVATION_SHIFT 12
#define METATILE_COLLISION_SHIFT 10
#define METATILE_ELEVATION_MASK 0xF000

#define METATILE_ID(tileset, name) (METATILE_##tileset##_##name)

enum
{
    METATILE_ATTRIBUTE_BEHAVIOR,
    METATILE_ATTRIBUTE_TERRAIN,
    METATILE_ATTRIBUTE_2,
    METATILE_ATTRIBUTE_3,
    METATILE_ATTRIBUTE_ENCOUNTER_TYPE,
    METATILE_ATTRIBUTE_5,
    METATILE_ATTRIBUTE_LAYER_TYPE,
    METATILE_ATTRIBUTE_7,
    METATILE_ATTRIBUTE_COUNT,
};

enum
{
    TILE_ENCOUNTER_NONE,
    TILE_ENCOUNTER_LAND,
    TILE_ENCOUNTER_WATER,
};

enum
{
    TILE_TERRAIN_NORMAL,
    TILE_TERRAIN_GRASS,
    TILE_TERRAIN_WATER,
    TILE_TERRAIN_WATERFALL,
};

enum
{
    CONNECTION_SOUTH = 1,
    CONNECTION_NORTH,
    CONNECTION_WEST,
    CONNECTION_EAST,
    CONNECTION_DIVE,
    CONNECTION_EMERGE
};

typedef void (*TilesetCB)(void);

struct Tileset
{
    /*0x00*/ bool8 isCompressed;
    /*0x01*/ bool8 isSecondary;
    /*0x04*/ void *tiles;
    /*0x08*/ void *palettes;
    /*0x0c*/ void *metatiles;
    /*0x10*/ TilesetCB callback;
    /*0x14*/ void *metatileAttributes;
};

struct MapLayout
{
    /*0x00*/ s32 width;
    /*0x04*/ s32 height;
    /*0x08*/ u16 *border;
    /*0x0c*/ u16 *map;
    /*0x10*/ struct Tileset *primaryTileset;
    /*0x14*/ struct Tileset *secondaryTileset;
    /*0x18*/ u8 borderWidth;
    /*0x19*/ u8 borderHeight;
};

struct BackupMapLayout
{
    s32 Xsize;
    s32 Ysize;
    u16 *map;
};

union __attribute__((packed)) ObjectEventRange {
    u8 as_byte;
    struct __attribute__((packed)) {
        u8 x:4;
        u8 y:4;
    } __attribute__((aligned (1))) as_nybbles;
} __attribute__((aligned (1)));

struct ObjectEventTemplate
{
    /*0x00*/ u8 localId;
    /*0x01*/ u8 graphicsId;
    /*0x02*/ u8 inConnection;
    /*0x04*/ s16 x;
    /*0x06*/ s16 y;
    /*0x08*/ u8 elevation;
    /*0x09*/ u8 movementType;
    /*0x0A*/ u16 movementRangeX:4;
             u16 movementRangeY:4;
    /*0x0C*/ u16 trainerType;
    /*0x0E*/ u16 trainerRange_berryTreeId;
    /*0x10*/ const u8 *script;
    /*0x14*/ u16 flagId;
};  /*size = 0x18*/

struct WarpEvent
{
    s16 x, y;
    u8 elevation;
    u8 warpId;
    u8 mapNum;
    u8 mapGroup;
};

struct CoordEvent
{
    u16 x, y;
    u8 elevation;
    u16 trigger;
    u16 index;
    u8 *script;
};

struct HiddenItemStruct
{
    u32 itemId:16;
    u32 hiddenItemId:8; // flag offset to determine flag lookup
    u32 quantity:7;
    u32 isUnderfoot:1;
};

union BgUnion
{ // carried over from diego's FR/LG work, seems to be the same struct
    // in gen 3, "kind" (0x3 in BgEvent struct) determines the method to read the union.
    u8 *script;

    // hidden item type probably
    struct HiddenItemStruct hiddenItemStr;
    u32 hiddenItem;

    // secret base type
    u32 secretBaseId;

};

struct BgEvent
{
    u16 x, y;
    u8 elevation;
    u8 kind;
    // 0x2 padding for the union beginning.
    union BgUnion bgUnion;
};

struct MapEvents
{
    u8 objectEventCount;
    u8 warpCount;
    u8 coordEventCount;
    u8 bgEventCount;

    struct ObjectEventTemplate *objectEvents;
    struct WarpEvent *warps;
    struct CoordEvent *coordEvents;
    struct BgEvent *bgEvents;
};

struct MapConnection
{
 /*0x00*/ u8 direction;
 /*0x04*/ u32 offset;
 /*0x08*/ u8 mapGroup;
 /*0x09*/ u8 mapNum;
};

struct MapConnections
{
    s32 count;
    struct MapConnection *connections;
};

struct MapHeader
{
    /* 0x00 */ const struct MapLayout *mapLayout;
    /* 0x04 */ const struct MapEvents *events;
    /* 0x08 */ const u8 *mapScripts;
    /* 0x0C */ const struct MapConnections *connections;
    /* 0x10 */ u16 music;
    /* 0x12 */ u16 mapLayoutId;
    /* 0x14 */ u8 regionMapSectionId;
    /* 0x15 */ u8 cave;
    /* 0x16 */ u8 weather;
    /* 0x17 */ u8 mapType;
    /* 0x18 */ bool8 bikingAllowed;
    /* 0x19 */ u8 flags;
    /* 0x1A */ s8 floorNum;
    /* 0x1B */ u8 battleType;
};

// Flags for gMapHeader.flags, as defined in the map_header_flags macro
#define MAP_ALLOW_ESCAPE_ROPE  (1 << 0)
#define MAP_ALLOW_RUN          (1 << 1)
#define MAP_SHOW_MAP_NAME      (1 << 2)
#define UNUSED_MAP_FLAGS       (1 << 3 | 1 << 4 | 1 << 5 | 1 << 6 | 1 << 7)

#define SHOW_MAP_NAME_ENABLED  ((gMapHeader.flags & (MAP_SHOW_MAP_NAME | UNUSED_MAP_FLAGS)) == MAP_SHOW_MAP_NAME)

struct ObjectEvent
{
    /*0x00*/ /* 0*/ u32 active:1;
             /* 1*/ u32 singleMovementActive:1;
             /* 2*/ u32 triggerGroundEffectsOnMove:1;
             /* 3*/ u32 triggerGroundEffectsOnStop:1;
             /* 4*/ u32 disableCoveringGroundEffects:1;
             /* 5*/ u32 landingJump:1;
             /* 6*/ u32 heldMovementActive:1;
             /* 7*/ u32 heldMovementFinished:1;
    /*0x01*/ /* 8*/ u32 frozen:1;
             /* 9*/ u32 facingDirectionLocked:1;
             /*10*/ u32 disableAnim:1;
             /*11*/ u32 enableAnim:1;
             /*12*/ u32 inanimate:1;
             /*13*/ u32 invisible:1;
             /*14*/ u32 offScreen:1;
             /*15*/ u32 trackedByCamera:1;
    /*0x02*/ /*16*/ u32 isPlayer:1;
             /*17*/ u32 hasReflection:1;
             /*18*/ u32 inShortGrass:1;
             /*19*/ u32 inShallowFlowingWater:1;
             /*20*/ u32 inSandPile:1;
             /*21*/ u32 inHotSprings:1;
             /*22*/ u32 hasShadow:1;
             /*23*/ u32 spriteAnimPausedBackup:1;
    /*0x03*/ /*24*/ u32 spriteAffineAnimPausedBackup:1;
             /*25*/ u32 disableJumpLandingGroundEffect:1;
             /*26*/ u32 fixedPriority:1;
             /*27*/ u32 hideReflection:1;
    /*0x04*/        u8 spriteId;
    /*0x05*/        u8 graphicsId;
    /*0x06*/        u8 movementType;
    /*0x07*/        u8 trainerType;
    /*0x08*/        u8 localId;
    /*0x09*/        u8 mapNum;
    /*0x0A*/        u8 mapGroup;
    /*0x0B*/        u8 currentElevation:4;
                    u8 previousElevation:4;
    /*0x0C*/        struct Coords16 initialCoords;
    /*0x10*/        struct Coords16 currentCoords;
    /*0x14*/        struct Coords16 previousCoords;
    /*0x18*/        u8 facingDirection:4;
    /*0x18*/        u8 movementDirection:4;
    /*0x19*/        union ObjectEventRange range;
    /*0x1A*/        u8 fieldEffectSpriteId;
    /*0x1B*/        u8 warpArrowSpriteId;
    /*0x1C*/        u8 movementActionId;
    /*0x1D*/        u8 trainerRange_berryTreeId;
    /*0x1E*/        u8 currentMetatileBehavior;
    /*0x1F*/        u8 previousMetatileBehavior;
    /*0x20*/        u8 previousMovementDirection;
    /*0x21*/        u8 directionSequenceIndex;
    /*0x22*/        u8 playerCopyableMovement;
    /*size = 0x24*/
};

struct ObjectEventGraphicsInfo
{
    /*0x00*/ u16 tileTag;
    /*0x02*/ u16 paletteTag1;
    /*0x04*/ u16 paletteTag2;
    /*0x06*/ u16 size;
    /*0x08*/ s16 width;
    /*0x0A*/ s16 height;
    /*0x0C*/ u8 paletteSlot:4;
             u8 shadowSize:2;
             u8 inanimate:1;
             u8 disableReflectionPaletteLoad:1;
    /*0x0D*/ u8 tracks;
    /*0x10*/ const struct OamData *oam;
    /*0x14*/ const struct SubspriteTable *subspriteTables;
    /*0x18*/ const union AnimCmd *const *anims;
    /*0x1C*/ const struct SpriteFrameImage *images;
    /*0x20*/ const union AffineAnimCmd *const *affineAnims;
};

enum {
    PLAYER_AVATAR_STATE_NORMAL,
    PLAYER_AVATAR_STATE_MACH_BIKE,
    PLAYER_AVATAR_STATE_ACRO_BIKE,
    PLAYER_AVATAR_STATE_SURFING,
    PLAYER_AVATAR_STATE_UNDERWATER,
    PLAYER_AVATAR_STATE_FIELD_MOVE,
    PLAYER_AVATAR_STATE_FISHING,
    PLAYER_AVATAR_STATE_WATERING,
};

#define PLAYER_AVATAR_FLAG_ON_FOOT    (1 << PLAYER_AVATAR_STATE_NORMAL)
#define PLAYER_AVATAR_FLAG_MACH_BIKE  (1 << PLAYER_AVATAR_STATE_MACH_BIKE)
#define PLAYER_AVATAR_FLAG_ACRO_BIKE  (1 << PLAYER_AVATAR_STATE_ACRO_BIKE)
#define PLAYER_AVATAR_FLAG_SURFING    (1 << PLAYER_AVATAR_STATE_SURFING)
#define PLAYER_AVATAR_FLAG_UNDERWATER (1 << PLAYER_AVATAR_STATE_UNDERWATER)
#define PLAYER_AVATAR_FLAG_FIELD_MOVE (1 << PLAYER_AVATAR_STATE_FIELD_MOVE)
#define PLAYER_AVATAR_FLAG_FISHING    (1 << PLAYER_AVATAR_STATE_FISHING)
#define PLAYER_AVATAR_FLAG_DASH       (1 << PLAYER_AVATAR_STATE_WATERING)

enum
{
    ACRO_BIKE_NORMAL,
    ACRO_BIKE_TURNING,
    ACRO_BIKE_WHEELIE_STANDING,
    ACRO_BIKE_BUNNY_HOP,
    ACRO_BIKE_WHEELIE_MOVING,
    ACRO_BIKE_STATE5,
    ACRO_BIKE_STATE6,
};

enum
{
    COLLISION_NONE,
    COLLISION_OUTSIDE_RANGE,
    COLLISION_IMPASSABLE,
    COLLISION_ELEVATION_MISMATCH,
    COLLISION_OBJECT_EVENT,
    COLLISION_STOP_SURFING,
    COLLISION_LEDGE_JUMP,
    COLLISION_PUSHED_BOULDER,
    COLLISION_UNKNOWN_WARP_6C_6D_6E_6F,
    COLLISION_WHEELIE_HOP,
    COLLISION_ISOLATED_VERTICAL_RAIL,
    COLLISION_ISOLATED_HORIZONTAL_RAIL,
    COLLISION_VERTICAL_RAIL,
    COLLISION_HORIZONTAL_RAIL,
    COLLISION_COUNT
};

// player running states
enum
{
    NOT_MOVING,
    TURN_DIRECTION, // not the same as turning! turns your avatar without moving. also known as a turn frame in some circles
    MOVING,
};

// player tile transition states
enum
{
    T_NOT_MOVING,
    T_TILE_TRANSITION,
    T_TILE_CENTER, // player is on a frame in which they are centered on a tile during which the player either stops or keeps their momentum and keeps going, changing direction if necessary.
};

struct PlayerAvatar /* 0x202E858 */
{
    /*0x00*/ u8 flags;
    /*0x01*/ u8 transitionFlags; // used to be bike, but it's not that in Emerald and probably isn't here either. maybe transition flags?
    /*0x02*/ u8 runningState; // this is a static running state. 00 is not moving, 01 is turn direction, 02 is moving.
    /*0x03*/ u8 tileTransitionState; // this is a transition running state: 00 is not moving, 01 is transition between tiles, 02 means you are on the frame in which you have centered on a tile but are about to keep moving, even if changing directions. 2 is also used for a ledge hop, since you are transitioning.
    /*0x04*/ u8 spriteId;
    /*0x05*/ u8 objectEventId;
    /*0x06*/ bool8 preventStep;
    /*0x07*/ u8 gender;
    // These are not used in FRLG
    u8 acroBikeState;
    u8 newDirBackup;
    u8 bikeFrameCounter;
    u8 bikeSpeed;
    u32 directionHistory;
    u32 abStartSelectHistory;
    u8 dirTimerHistory[8];
    // For the Rocket mazes
    u16 lastSpinTile;
};

struct Camera
{
    bool8 active:1;
    s32 x;
    s32 y;
};

extern struct ObjectEvent gObjectEvents[OBJECT_EVENTS_COUNT];
extern u8 gSelectedObjectEvent;
extern struct MapHeader gMapHeader;
extern struct PlayerAvatar gPlayerAvatar;
extern struct Camera gCamera;

#endif // GUARD_GLOBAL_FIELDMAP_H
