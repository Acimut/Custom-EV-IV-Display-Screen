#ifndef GUARD_DATA_H
#define GUARD_DATA_H

#include "global.h"

#define SPECIES_SHINY_TAG 500
#define TRAINER_ENCOUNTER_MUSIC(trainer)((gTrainers[trainer].encounterMusic_gender & 0x7F))

struct MonCoords
{
    // This would use a bitfield, but some function
    // uses it as a u8 and casting won't match.
    u8 size; // u8 width:4, height:4;
    u8 y_offset;
};

extern const u8 gSpeciesNames[][POKEMON_NAME_LENGTH + 1];
extern const u8 gMoveNames[][13];
extern const u16 gUnknown_8251CB8[];
extern const u16 gUnknown_8251FEE[];
extern const u16 gUnknown_8252324[];
extern const u16 gUnknown_82539D4[];

extern const u8 gTrainerClassNames[][13];

extern const struct MonCoords gMonFrontPicCoords[];
extern const struct CompressedSpriteSheet gMonFrontPicTable[];
extern const struct MonCoords gMonBackPicCoords[];
extern const struct CompressedSpriteSheet gMonBackPicTable[];
extern const struct CompressedSpritePalette gMonPaletteTable[];
extern const struct CompressedSpritePalette gMonShinyPaletteTable[];
extern const union AnimCmd *const *const gTrainerFrontAnimsPtrTable[];
extern const struct MonCoords gTrainerFrontPicCoords[];
extern const struct CompressedSpriteSheet gTrainerFrontPicTable[];
extern const struct CompressedSpritePalette gTrainerFrontPicPaletteTable[];
extern const union AnimCmd *const *const gTrainerBackAnimsPtrTable[];
extern const struct MonCoords gTrainerBackPicCoords[];
extern const struct CompressedSpritePalette gTrainerBackPicPaletteTable[];

extern const struct CompressedSpriteSheet gSpriteSheet_EnemyShadow;
extern const struct SpriteTemplate gSpriteTemplate_EnemyShadow;

extern const u8 gEnemyMonElevation[NUM_SPECIES];

extern const u8 *const gBattleAnims_General[];
extern const u8 *const gBattleAnims_Special[];

extern const struct OamData gUnknown_824F010;
extern const struct OamData gUnknown_824F018;
extern const union AnimCmd *const gSpriteAnimTable_82349BC[];
extern const union AffineAnimCmd *const gSpriteAffineAnimTable_BattlerPlayer[];
extern const union AffineAnimCmd *const gSpriteAffineAnimTable_BattlerOpponent[];
extern const struct SpriteFrameImage gSpriteImages_BattlerPlayerLeft[];
extern const struct SpriteFrameImage gSpriteImages_BattlerOpponentLeft[];
extern const struct SpriteFrameImage gSpriteImages_BattlerPlayerRight[];
extern const struct SpriteFrameImage gSpriteImages_BattlerOpponentRight[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Red[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Leaf[];
extern const struct SpriteFrameImage gTrainerBackPicTable_Pokedude[];
extern const struct SpriteFrameImage gTrainerBackPicTable_OldMan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSBrendan[];
extern const struct SpriteFrameImage gTrainerBackPicTable_RSMay[];

#endif // GUARD_DATA_H
