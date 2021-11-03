#include "include/global.h"
#include "include/main.h"


#if defined(BPRE)
#define FIRERED
#endif
#if defined(BPRS)
#define FIRERED
#endif

#ifndef FIRERED
#define FIRERED
#endif

#if defined(BPEE)
#define EMERALD
#undef FIRERED
#endif
#if defined(BPES)
#define EMERALD
#undef FIRERED
#endif
#ifdef EMERALD
#define SE_RG_CARD_FLIP             249 // SE_RG_CARD1
#define SE_RG_CARD_FLIPPING         250 // SE_RG_CARD2
#define SE_RG_CARD_OPEN             251 // SE_RG_CARD3
#endif

extern void CB2_ShowEvIv(void);


extern u8 *GetMonNickname(struct Pokemon *mon, u8 *dest);
extern void PlaySE(u16 songNum);
extern void ScriptContext2_Enable(void);
extern void ScanlineEffect_Stop(void);
extern void SetGpuReg(u8 regOffset, u16 value);
extern void ResetTempTileDataBuffers(void);
extern void *DecompressAndCopyTileDataToVram(u8 bgId, const void *src, u32 size, u16 offset, u8 mode);
extern bool8 FreeTempTileDataBuffersIfPossible(void);
extern bool8 IsCryPlaying(void);
extern u8 CreateMonSprite_FieldMove(u16 species, u32 otId, u32 personality, s16 x, s16 y, u8 subpriority);
extern void FreeResourcesAndDestroySprite(struct Sprite * sprite, u8 spriteId);

extern u16 *stdpal_get(u8 id);//firered
extern u16 *GetTextWindowPalette(u8 id);//emerald

extern void QuestLog_CutRecording(void);//firered

extern void CB2_ReturnToFieldFromDiploma(void);//firered
extern void CB2_ReturnToFieldFadeFromBlack(void);//emerald

extern void PlayCry1(u16 species, s8 pan);  //emerald
extern void PlayCry7(u16 species, u8 mode); // exclusive to FR/LG
