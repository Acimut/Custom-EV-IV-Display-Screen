__aeabi_idiv = 0x082ee814|1;
__aeabi_idivmod = 0x082ee814|1;
__aeabi_uidiv = 0x082ee814|1;
__aeabi_uidivmod = 0x082ee814|1;
__divsi3 = 0x082eeccc|1;
__udivsi3 = 0x082ef2f4|1;
__umodsi3 = 0x082ef36c|1;
memcpy = 0x082f0b60|1;
memset = 0x082f0bc0|1;
strcmp = 0x082f0c14|1;
strcpy = 0x082f0c70|1;


/*gEvIv = 0x0203FFF0;*/
gEvIv = 0x030078b0;

DestroySpriteAndFreeResources = 0x08007620|1;
GetTextWindowPalette    = 0x08098c2c|1;
ListMenuLoadStdPalAt    = 0x08199ec0|1;
BlitMoveInfoIcon        = 0x08199efc|1;/*BlitMenuInfoIcon*/
CopyWindowToVram        = 0x08003658|1;

PlaySE = 0x080a37b8|1;
GetNature = 0x0806d070|1;
gPlayerPartyCount = 0x020244e9;/*EWRAM*/
gPlayerParty = 0x020244ec;/*EWRAM*/
gBaseStatsPtr = 0x080001BC;/*gBaseStats pointer*/
GetMonData = 0x0806a518|1;
GetSpeciesName = 0x0806b914|1;
GetMonNickname = 0x081b1764|1;


SetMainCallback2 = 0x08000540|1;
ScriptContext2_Enable = 0x08098e68|1;
AllocZeroed = 0x08000b4c|1;
Free = 0x08000b60|1;
CreateTask = 0x080a8fc4|1;
RunTasks = 0x080a9120|1;
AnimateSprites = 0x080069c0|1;
BuildOamBuffer = 0x08006a0c|1;
UpdatePaletteFade = 0x080a1a30|1;

ResetSpriteData = 0x08006974|1;
ResetPaletteFade = 0x080a1a88|1;
FreeAllSpritePalettes = 0x0800870c|1;
ResetTasks = 0x080a8f64|1;
ScanlineEffect_Stop = 0x080b9fcc|1;

SetVBlankCallback = 0x080006f0|1;
CopyToBgTilemapBuffer = 0x080022f0|1;
SetGpuReg = 0x080010b4|1;
CopyBgTilemapBufferToVram = 0x0800236c|1;
BeginNormalPaletteFade = 0x080a1ae8|1;
gPaletteFade = 0x02037fd4;/*EWRAM*/
gTasks = 0x03005e00;/*IWRAM*/
gMain = 0x030022c0;/*IWRAM*/
gSaveBlock1Ptr = 0x03005d8c;/*IWRAM*/
ResetBgsAndClearDma3BusyFlags = 0x080017bc|1;
InitBgsFromTemplates = 0x080017e8|1;
ChangeBgX = 0x08001d04|1;
ChangeBgY = 0x08001e7c|1;
InitWindows = 0x080031c0|1;
DeactivateAllTextPrinters = 0x080045b0|1;
SetBgTilemapBuffer = 0x08002250|1;
ShowBg = 0x08001b30|1;
FillBgTilemapBufferRect_Palette0 = 0x08002704|1;

ResetTempTileDataBuffers = 0x08199624|1;
DecompressAndCopyTileDataToVram = 0x08199694|1;
FreeTempTileDataBuffersIfPossible = 0x08199648|1;
LoadPalette = 0x080a194c|1;

FillWindowPixelBuffer = 0x08003c48|1;
AddTextPrinterParameterized3 = 0x08199a68|1;
PutWindowTilemap = 0x0800378c|1;

LoadOam = 0x08007188|1;
ProcessSpriteCopyRequests = 0x0800742c|1;
TransferPlttBuffer = 0x080a19d4|1;

DestroyTask = 0x080a90b0|1;
FreeAllWindowBuffers = 0x08003604|1;
CB2_ReturnToFieldFadeFromBlack = 0x080861fc|1;

ConvertIntToDecimalStringN = 0x08008cc0|1;
StringAppend = 0x08008bc0|1;
StringCopy = 0x08008ba0|1;
gStringVar1 = 0x02021cc4;/*EWRAM*/
gStringVar2 = 0x02021dc4;/*EWRAM*/
gStringVar3 = 0x02021ec4;/*EWRAM*/
gStringVar4 = 0x02021fc4;/*EWRAM*/

PlayCry_Normal = 0x080a3288|1;
IsCryPlaying = 0x080a3700|1;
CreateMonSprite_FieldMove = 0x080b5ffc|1;/*unused*/
FreeResourcesAndDestroySprite = 0x080b6084|1;
SpriteCallbackDummy = 0x08007428|1;
gSprites = 0x02020630;/*EWRAM*/

GetMonSpritePalStructFromOtIdPersonality = 0x0806e7cc|1;
IndexOfSpritePaletteTag = 0x08008804|1;
PreservePaletteInWeather = 0x080ac40c|1;
CreateMonPicSprite_HandleDeoxys = 0x0818d3c8|1;

/*summary screen*/
sMonSummaryScreen   = 0x0203cf1c;/*EWRAM*/
gLastViewedMonIndex = 0x0203cf20;/*EWRAM*/
gMonSpritesGfxPtr   = 0x020244d4;/*EWRAM*/

    /*func*/
CB2_InitSummaryScreen               = 0x081bf704|1;
SummaryScreen_SetAnimDelayTaskId    = 0x081c44ac|1;
CreateMonSpritesGfxManager          = 0x0806f2a8|1;

MenuHelpers_ShouldWaitForLinkRecv   = 0x08121E04|1;
StopCryAndClearCrySongs             = 0x080a36b0|1;
DestroyMonSpritesGfxManager         = 0x0806f478|1;
SummaryScreen_DestroyAnimDelayTask  = 0x081c44b8|1;

GetLRKeysPressed        = 0x08121ce8|1;
ChangeSummaryPokemon    = 0x081c0224|1;
ChangePage              = 0x081c06ac|1;
StopPokemonAnimations   = 0x081c4510|1;
BeginCloseSummaryScreen = 0x081c006c|1;
SwitchToMoveSelection   = 0x081c0a68|1;

GetStringRightAlignXOffset  = 0x081dafc8|1;
PrintAOrBButtonIcon         = 0x081c2458|1;
PrintTextOnWindow           = 0x081c21c4|1;
InBattleFactory             = 0x081a67f8|1;
InSlateportBattleTent       = 0x081b9ab4|1;
RemoveWindowByIndex         = 0x081c2988|1;

ClearWindowTilemap              = 0x080038a4|1;
ScheduleBgCopyTilemapToVram     = 0x081995c0|1;

GetBoxMonData   = 0x0806a674|1;
BoxMonToMon     = 0x08069004|1;

SeekToNextMonInBox  = 0x080D1DA0|1;
IsMultiBattle       = 0x081B0E70|1;

VarGet      = 0x0809d6a8|1;
VarSet      = 0x0809d6c4|1;
FlagSet     = 0x0809d754|1;
FlagClear   = 0x0809d77c|1;
FlagGet     = 0x0809d7a4|1;
