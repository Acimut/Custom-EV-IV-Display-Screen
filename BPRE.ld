__aeabi_idiv = 0x081e3b68|1;
__aeabi_idivmod = 0x081e3b68|1;
__aeabi_uidiv = 0x081e3b68|1;
__aeabi_uidivmod = 0x081e3b68|1;
__divsi3 = 0x081e4018|1;
__udivsi3 = 0x081e460c|1;
__umodsi3 = 0x081e4684|1;
memcpy = 0x081e5e78|1;
memset = 0x081e5ed8|1;
strcmp = 0x081e5f2c|1;
strcpy = 0x081e5f88|1;
memmove = 0x081e8e60|1;

gEvIv = 0x0203FFF0;

DestroySpriteAndFreeResources = 0x08007804|1;
stdpal_get = 0x08150408|1;

PlaySE = 0x080722cc|1;
GetNature = 0x08042e9c|1;
gPlayerPartyCount = 0x02024029;
gPlayerParty = 0x02024284;
gBaseStatsPtr = 0x080001BC;
GetMonData = 0x0803fbe8|1;
GetSpeciesName = 0x08040fd0|1;
GetMonNickname = 0x081202e0|1;

/* tio terry */
ListMenuLoadStdPalAt = 0x08107d38|1;
GetMonGender = 0x0803f720|1;
BlitMoveInfoIcon = 0x08107D68|1;
IsMonShiny = 0x08044470|1;
gPokeBallTiles = 0x0826056C;         /* const 0826056c g 00000060 gBallSpriteSheets */
gPokeBallPals = 0x082605CC;          /* const 082605cc g 00000060 gBallSpritePalettes */
gBallSpriteTemplates = 0x082606F4;   /* const 082606f4 g 00000120 gBallSpriteTemplates */
CreateSprite = 0x08006F8C|1;
SpriteCB_TestBallThrow = 0x0804ABD4|1;
SpriteCB_ReleaseMonFromBall = 0x0804B268|1;
CreatePokeballSpriteToReleaseMon = 0x0804B908|1;
objc_ball_instanciate2_entry_804B9E8 = 0x0804B9E8|1;
sub_804BD94    = 0x0804BD94|1;
LoadBallGfx    = 0x0804BEF4|1;
FreeBallGfx    = 0x0804BF64|1;
ItemIdToBallId = 0x080EF52C|1;
PokeBallOpenParticleAnimation    = 0x080F06A4|1;
TimerBallOpenParticleAnimation   = 0x080F0818|1;
DiveBallOpenParticleAnimation    = 0x080F091C|1;
SafariBallOpenParticleAnimation  = 0x080F0A20|1;
UltraBallOpenParticleAnimation   = 0x080F0B20|1;
GreatBallOpenParticleAnimation   = 0x080F0C28|1;
RepeatBallOpenParticleAnimation  = 0x080F0DB4|1;
MasterBallOpenParticleAnimation  = 0x080F0F10|1;
PremierBallOpenParticleAnimation = 0x080F103C|1;
FreeBallTiles = 0x080F1198|1;       /* 080f1198 l 00000080 DestroyBallOpenAnimationParticle */

gSaveDataBuffer = 0x02039A38;
gSaveBlock1ptr = 0x3005008;
gSaveBlock2ptr = 0x300500C;
gSaveBlock3ptr = 0x3005010;
gFirstSaveSector = 0x3005380;
gSaveCounter = 0x3005390;
gFastSaveSection = 0x3005394;
gRamSaveSectionLocations = 0x30053B0;
/* ------ */


QuestLog_CutRecording = 0x08112364|1;
SetMainCallback2 = 0x08000544|1;
ScriptContext2_Enable = 0x08069940|1;
AllocZeroed = 0x08002bb0|1;
Free = 0x08002bc4|1;
CreateTask = 0x0807741c|1;
RunTasks = 0x08077578|1;
AnimateSprites = 0x08006b5c|1;
BuildOamBuffer = 0x08006ba8|1;
UpdatePaletteFade = 0x080704d0|1;

ResetSpriteData = 0x08006b10|1;
ResetPaletteFade = 0x08070528|1;
FreeAllSpritePalettes = 0x080088f0|1;
ResetTasks = 0x080773bc|1;
ScanlineEffect_Stop = 0x08087e64|1;

SetVBlankCallback = 0x080006f4|1;
CopyToBgTilemapBuffer = 0x08002040|1;
SetGpuReg = 0x08000a38|1;
CopyBgTilemapBufferToVram = 0x080020bc|1;
BeginNormalPaletteFade = 0x08070588|1;
gPaletteFade = 0x02037ab8;
gTasks = 0x03005090;
gMain = 0x030030f0;

ResetBgsAndClearDma3BusyFlags = 0x08001618|1;
InitBgsFromTemplates = 0x08001658|1;
ChangeBgX = 0x08001b90|1;
ChangeBgY = 0x08001d08|1;
InitWindows = 0x08003b24|1;
DeactivateAllTextPrinters = 0x08002c28|1;
SetBgTilemapBuffer = 0x08001fa0|1;
ShowBg = 0x080019bc|1;
FillBgTilemapBufferRect_Palette0 = 0x08002454|1;

ResetTempTileDataBuffers = 0x080f6808|1;
DecompressAndCopyTileDataToVram = 0x080f6878|1;
FreeTempTileDataBuffersIfPossible = 0x080f682c|1;
LoadPalette = 0x080703ec|1;

FillWindowPixelBuffer = 0x0800445c|1;
AddTextPrinterParameterized3 = 0x0812e51c|1;
PutWindowTilemap = 0x08003fa0|1;

LoadOam = 0x08007320|1;
ProcessSpriteCopyRequests = 0x08007610|1;
TransferPlttBuffer = 0x08070474|1;

DestroyTask = 0x08077508|1;
FreeAllWindowBuffers = 0x08003ecc|1;
CB2_ReturnToFieldFromDiploma = 0x080568fc|1;

ConvertIntToDecimalStringN = 0x08008e78|1;
StringAppend = 0x08008da4|1;
StringCopy = 0x08008d84|1;
gStringVar1 = 0x02021cd0;
gStringVar2 = 0x02021cf0;
gStringVar3 = 0x02021d04;
gStringVar4 = 0x02021d18;

PlayCry7 = 0x08071f24|1;
IsCryPlaying = 0x08072214|1;
CreateMonSprite_FieldMove = 0x080839d4|1;/*static u8*/
FreeResourcesAndDestroySprite = 0x08083a5c|1;
SpriteCallbackDummy = 0x0800760c|1;
gSprites = 0x0202063c;

GetMonSpritePalStructFromOtIdPersonality = 0x08044180|1;
IndexOfSpritePaletteTag = 0x080089e8|1;
PreservePaletteInWeather = 0x0807b084|1;
CreateMonPicSprite_HandleDeoxys = 0x0810c1cc|1;
