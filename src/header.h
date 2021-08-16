#include "include/global.h"
#include "include/menu.h"
#include "include/task.h"
#include "include/constants/fanfares.h"
#include "include/constants/songs.h"
#include "include/bg.h"
#include "include/main.h"
#include "include/malloc.h"
#include "include/palette.h"
#include "include/sprite.h"
#include "include/window.h"
#include "include/sprite.h"

extern struct Sprite gSprites[65];
extern struct Pokemon gPlayerParty[PARTY_SIZE];
extern u8 gPlayerPartyCount;
extern const struct BaseStats gBaseStats[];

extern u8 GetNature(struct Pokemon *mon);
extern u32 GetMonData(struct Pokemon *mon, s32 field, u8* data);
extern u8 *GetMonNickname(struct Pokemon *mon, u8 *dest);
extern void GetSpeciesName(u8 *name, u16 species);
extern u8 *StringAppend(u8 *dest, const u8 *src);
extern u8 *StringCopy(u8 *dest, const u8 *src);
extern u8 *ConvertIntToDecimalStringN(u8 *dest, s32 value, /*enum StringConvertMode*/u8 mode, u8 n);
extern u8 gStringVar1[32];
extern u8 gStringVar2[20];
extern u8 gStringVar3[20];
extern u8 gStringVar4[1000];
extern void PlaySE(u16 songNum);

extern u16 *stdpal_get(u8 id);

extern void SetMainCallback2(MainCallback callback);
extern void QuestLog_CutRecording(void);
extern void ScriptContext2_Enable(void);
extern void *AllocZeroed(u32 size);
extern void Free(void *pointer);
extern u8 CreateTask(TaskFunc func, u8 priority);
extern void RunTasks(void);
extern void AnimateSprites(void);
extern void BuildOamBuffer(void);
extern u8 UpdatePaletteFade(void);

extern void ResetSpriteData(void);
extern void ResetPaletteFade(void);
extern void FreeAllSpritePalettes(void);
extern void ResetTasks(void);
extern void ScanlineEffect_Stop(void);

extern void SetVBlankCallback(IntrCallback callback);
extern void CopyToBgTilemapBuffer(u8 bg, const void *src, u16 mode, u16 destOffset);
extern void SetGpuReg(u8 regOffset, u16 value);
extern void CopyBgTilemapBufferToVram(u8 bg);
extern bool8 BeginNormalPaletteFade(u32 selectedPalettes, s8 delay, u8 startY, u8 targetY, u16 blendColor);
extern struct PaletteFadeControl gPaletteFade;
//extern struct Task gTasks[]; //ya en task.h
//extern struct Main gMain;//ya en main.h
//extern const struct BaseStats gBaseStats[]; gBaseStats = 0x08254784;

extern void ResetBgsAndClearDma3BusyFlags(bool32 enableWindowTileAutoAlloc);
extern void InitBgsFromTemplates(u8 bgMode, const struct BgTemplate *templates, u8 numTemplates);
extern u32 ChangeBgX(u8 bg, u32 value, u8 op);
extern u32 ChangeBgY(u8 bg, u32 value, u8 op);
extern bool16 InitWindows(const struct WindowTemplate *templates);
extern void DeactivateAllTextPrinters (void);
extern void SetBgTilemapBuffer(u8 bg, void *tilemap);
extern void ShowBg(u8 bg);
extern void FillBgTilemapBufferRect_Palette0(u8 bg, u16 tileNum, u8 x, u8 y, u8 width, u8 height);

extern void ResetTempTileDataBuffers(void);
extern void *DecompressAndCopyTileDataToVram(u8 bgId, const void *src, u32 size, u16 offset, u8 mode);
extern bool8 FreeTempTileDataBuffersIfPossible(void);
extern void LoadPalette(const void *src, u16 offset, u16 size);

extern void FillWindowPixelBuffer(u8 windowId, u8 fillValue);
extern void AddTextPrinterParameterized3(u8 windowId, u8 fontId, u8 x, u8 y, const u8 * color, s8 speed, const u8 * str);
extern void PutWindowTilemap(u8 windowId);

extern void LoadOam(void);
extern void ProcessSpriteCopyRequests(void);
extern void TransferPlttBuffer(void);

extern void DestroyTask(u8 taskId);
extern void FreeAllWindowBuffers(void);
extern void CB2_ReturnToFieldFromDiploma(void);

extern void PlayCry7(u16 species, u8 mode); // exclusive to FR/LG
extern bool8 IsCryPlaying(void);
extern u8 CreateMonSprite_FieldMove(u16 species, u32 otId, u32 personality, s16 x, s16 y, u8 subpriority);
extern void FreeResourcesAndDestroySprite(struct Sprite * sprite, u8 spriteId);
extern void SpriteCallbackDummy(struct Sprite *sprite);
extern void DestroySpriteAndFreeResources(struct Sprite *sprite);
