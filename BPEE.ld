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


gEvIv = 0x0203FFF0;

DestroySpriteAndFreeResources = 0x08007620|1;
GetTextWindowPalette = 0x08098c18|1;

PlaySE = 0x080a37a4|1;
GetNature = 0x0806d070|1;
gPlayerPartyCount = 0x020244e9;
gPlayerParty = 0x020244ec;
gBaseStatsPtr = 0x080001BC;
GetMonData = 0x0806a518|1;
GetSpeciesName = 0x0806b914|1;
GetMonNickname = 0x081b1b44|1;

/* tio terry */
ListMenuLoadStdPalAt = 0x0819a2bc|1;
GetMonGender = 0x08069FB4|1;
BlitMoveInfoIcon = 0x0819A2F8|1;
IsMonShiny = 0x0806EBA4|1;
gPokeBallTiles = 0x08338d18;         /* const 0826056c g 00000060 gBallSpriteSheets */
gPokeBallPals = 0x08338d20;          /* const 082605cc g 00000060 gBallSpritePalettes */
gBallSpriteTemplates = 0x0832c588;   /* const 082606f4 g 00000120 gBallSpriteTemplates */
CreateSprite = 0x08006df4|1;
SpriteCB_TestBallThrow = 0x08075620|1;
SpriteCB_ReleaseMonFromBall = 0x08075d14|1;
CreatePokeballSpriteToReleaseMon = 0x08076438|1;
objc_ball_instanciate2_entry_8022B28 = 0x08022B28|1;
sub_8076918    = 0x08076918|1;
LoadBallGfx    = 0x08076a78|1;
LoadCompressedPalette = 0x080a18f4|1;
FreeBallGfx    = 0x08076ae8|1;
ItemIdToBallId = 0x08170d84|1;
PokeBallOpenParticleAnimation    = 0x08171e50|1;
TimerBallOpenParticleAnimation   = 0x08171fc4|1;
DiveBallOpenParticleAnimation    = 0x081720c8|1;
SafariBallOpenParticleAnimation  = 0x081721cc|1;
UltraBallOpenParticleAnimation   = 0x081722cc|1;
GreatBallOpenParticleAnimation   = 0x081723d4|1;
RepeatBallOpenParticleAnimation  = 0x08172560|1;
MasterBallOpenParticleAnimation  = 0x081726bc|1;
PremierBallOpenParticleAnimation = 0x081727e8|1;
FreeBallTiles = 0x08172944|1;       /* 08172944 l 00000080 DestroyBallOpenAnimationParticle */

gSaveDataBuffer = 0x0203abbc;
gSaveBlock1ptr = 0x3005d8c;
gSaveBlock2ptr = 0x3005d90;
gSaveBlock3ptr = 0x3005D94;
gFirstSaveSector = 0x30061f0;
gSaveCounter = 0x3006200;
gFastSaveSection = 0x03006204;
gRamSaveSectionLocations = 0x3006220;
/* ------ */



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

SetVBlankCallback = 0x081c7600|1;
CopyToBgTilemapBuffer = 0x080022f0|1;
SetGpuReg = 0x080010b4|1;
CopyBgTilemapBufferToVram = 0x0800236c|1;
BeginNormalPaletteFade = 0x080a1ad4|1;
gPaletteFade = 0x02037fd4;
gTasks = 0x03005e00;
gMain = 0x030022c0;

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

PlayCry1 = 0x080a3274|1;
IsCryPlaying = 0x080a36ec|1;
CreateMonSprite_FieldMove = 0x080b5fe8|1;/*static u8*/
FreeResourcesAndDestroySprite = 0x080b6070|1;
SpriteCallbackDummy = 0x08007428|1;
gSprites = 0x02020630;

GetMonSpritePalStructFromOtIdPersonality = 0x0806e7cc|1;
IndexOfSpritePaletteTag = 0x08008804|1;
PreservePaletteInWeather = 0x080ac3f8|1;
CreateMonPicSprite_HandleDeoxys = 0x0818d7d8|1;
