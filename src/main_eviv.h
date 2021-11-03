#include "include/global.h"

#include "include/main.h"
#include "include/script.h"
#include "include/sound.h"

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

#ifdef FIRERED
//FIRERED
#include "include/quest_log.h"
#endif

extern void CB2_ShowEvIv(void);

#ifdef EMERALD
//EMERALD
extern u16 *GetTextWindowPalette(u8 id);//emerald   //text_window.h
extern void CB2_ReturnToFieldFadeFromBlack(void);//emerald  //overworld.h
#endif
