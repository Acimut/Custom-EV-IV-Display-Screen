__aeabi_idiv = 0x082e7088|1;
__aeabi_idivmod = 0x082e7088|1;
__aeabi_uidiv = 0x082e7088|1;
__aeabi_uidivmod = 0x082e7088|1;
__divsi3 = 0x082e7540|1;
__udivsi3 = 0x082e7b68|1;
__umodsi3 = 0x082e7be0|1;
memcpy = 0x082e93d4|1;
memset = 0x082e9434|1;
strcmp = 0x082e9488|1;
strcpy = 0x082e94e4|1;


/*gEvIv = 0x0203FFF0;*/
gEvIv = 0x030078b0;

DestroySpriteAndFreeResources = 0x08007620|1;
GetTextWindowPalette = 0x08098c18|1;
ListMenuLoadStdPalAt = 0x0819a2bc|1;
BlitMoveInfoIcon = 0x0819a2f8|1;/*BlitMenuInfoIcon*/
CopyWindowToVram = 0x08003658|1;

PlaySE = 0x080a37a4|1;
GetNature = 0x0806d070|1;
gPlayerPartyCount = 0x020244e9;
gPlayerParty = 0x020244ec;
gBaseStatsPtr = 0x080001BC;
GetMonData = 0x0806a518|1;
GetSpeciesName = 0x0806b914|1;
GetMonNickname = 0x081b1b44|1;


SetMainCallback2 = 0x08000540|1;
ScriptContext2_Enable = 0x08098e54|1;
AllocZeroed = 0x08000b4c|1;
Free = 0x08000b60|1;
CreateTask = 0x080a8fb0|1;
RunTasks = 0x080a910c|1;
AnimateSprites = 0x080069c0|1;
BuildOamBuffer = 0x08006a0c|1;
UpdatePaletteFade = 0x080a1a1c|1;

ResetSpriteData = 0x08006974|1;
ResetPaletteFade = 0x080a1a74|1;
FreeAllSpritePalettes = 0x0800870c|1;
ResetTasks = 0x080a8f50|1;
ScanlineEffect_Stop = 0x080b9fb8|1;

SetVBlankCallback = 0x080006f0|1;/*SetVBlankCallback_*/
CopyToBgTilemapBuffer = 0x080022f0|1;
SetGpuReg = 0x080010b4|1;
CopyBgTilemapBufferToVram = 0x0800236c|1;
BeginNormalPaletteFade = 0x080a1ad4|1;
gPaletteFade = 0x02037fd4;
gTasks = 0x03005e00;
gMain = 0x030022c0;
gSaveBlock1Ptr = 0x03005d8c;
ResetBgsAndClearDma3BusyFlags = 0x080017bc|1;
InitBgsFromTemplates = 0x080017e8|1;
ChangeBgX = 0x08001d04|1;
ChangeBgY = 0x08001e7c|1;
InitWindows = 0x080031c0|1;
DeactivateAllTextPrinters = 0x080045b0|1;
SetBgTilemapBuffer = 0x08002250|1;
ShowBg = 0x08001b30|1;
FillBgTilemapBufferRect_Palette0 = 0x08002704|1;

ResetTempTileDataBuffers = 0x08199a20|1;
DecompressAndCopyTileDataToVram = 0x08199a90|1;
FreeTempTileDataBuffersIfPossible = 0x08199a44|1;
LoadPalette = 0x080a1938|1;

FillWindowPixelBuffer = 0x08003c48|1;
AddTextPrinterParameterized3 = 0x08199e64|1;
PutWindowTilemap = 0x0800378c|1;

LoadOam = 0x08007188|1;
ProcessSpriteCopyRequests = 0x0800742c|1;
TransferPlttBuffer = 0x080a19c0|1;

DestroyTask = 0x080a909c|1;
FreeAllWindowBuffers = 0x08003604|1;
CB2_ReturnToFieldFadeFromBlack = 0x080861e8|1;

ConvertIntToDecimalStringN = 0x08008cc0|1;
StringAppend = 0x08008bc0|1;
StringCopy = 0x08008ba0|1;
gStringVar1 = 0x02021cc4;
gStringVar2 = 0x02021dc4;
gStringVar3 = 0x02021ec4;
gStringVar4 = 0x02021fc4;

PlayCry_Normal = 0x080a3274|1;
IsCryPlaying = 0x080a36ec|1;
CreateMonSprite_FieldMove = 0x080b5fe8|1;/*static u8*/
FreeResourcesAndDestroySprite = 0x080b6070|1;
SpriteCallbackDummy = 0x08007428|1;
gSprites = 0x02020630;

GetMonSpritePalStructFromOtIdPersonality = 0x0806e7cc|1;
IndexOfSpritePaletteTag = 0x08008804|1;
PreservePaletteInWeather = 0x080ac3f8|1;
CreateMonPicSprite_HandleDeoxys = 0x0818d7d8|1;

/*summary screen*/
sMonSummaryScreen   = 0x0203cf1c;/*EWRAM*/
gLastViewedMonIndex = 0x0203cf20;/*EWRAM*/
gMonSpritesGfxPtr   = 0x020244d4;/*EWRAM*/

/*func*/
CB2_InitSummaryScreen = 0x081bfae4|1;
SummaryScreen_SetAnimDelayTaskId = 0x081c488c|1;
CreateMonSpritesGfxManager = 0x0806f2ac|1;

MenuHelpers_ShouldWaitForLinkRecv = 0x081221ec|1;
StopCryAndClearCrySongs = 0x080a369c|1;
DestroyMonSpritesGfxManager = 0x0806f47c|1;
SummaryScreen_DestroyAnimDelayTask = 0x081c4898|1;

GetLRKeysPressed = 0x081220d0|1;
ChangeSummaryPokemon = 0x081c0604|1;
ChangePage = 0x081c0a8c|1;
StopPokemonAnimations = 0x081c48f0|1;
BeginCloseSummaryScreen = 0x081c044c|1;
SwitchToMoveSelection = 0x081c0e48|1;

GetStringRightAlignXOffset = 0x081db368|1;
PrintAOrBButtonIcon = 0x081c2838|1;
PrintTextOnWindow = 0x081c25a4|1;
InBattleFactory = 0x081a6bf4|1;
InSlateportBattleTent = 0x081b9e94|1;
RemoveWindowByIndex = 0x081c2d68|1;

ClearWindowTilemap = 0x080038a4|1;
ScheduleBgCopyTilemapToVram = 0x081999bc|1;

GetBoxMonData = 0x0806a674|1;
BoxMonToMon = 0x08069004|1;

SeekToNextMonInBox = 0x080d214c|1;
IsMultiBattle = 0x081b1250|1;

VarGet = 0x0809d694|1;
VarSet = 0x0809d6b0|1;
FlagSet = 0x0809d740|1;
FlagClear = 0x0809d768|1;
FlagGet = 0x0809d790|1;
