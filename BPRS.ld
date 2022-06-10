__aeabi_idiv = 0x081e32ec|1;
__aeabi_idivmod = 0x081e32ec|1;
__aeabi_uidiv = 0x081e32ec|1;
__aeabi_uidivmod = 0x081e32ec|1;
__divsi3 = 0x081e379c|1;
__udivsi3 = 0x081e3d90|1;
__umodsi3 = 0x081e3e08|1;
memcpy = 0x081E55FC|1;
memset = 0x081E565C|1;
strcmp = 0x081e56b0|1;
strcpy = 0x081e570c|1;
memmove = 0x081e55FC|1;

gEvIv = 0x0203FFF0;

DestroySpriteAndFreeResources = 0x08007770|1;
stdpal_get = 0x081501f8|1;

PlaySE = 0x08072304|1;
GetNature = 0x08042D88|1;
gPlayerPartyCount = 0x02024029;
gPlayerParty = 0x02024284;
gBaseStatsPtr = 0x080001BC;
GetMonData = 0x0803fad4|1;
GetSpeciesName = 0x08040EBC|1;
GetMonNickname = 0x08120440|1;

/* tio terry */
GetMonGender = 0x0803f60c|1;
BlitMoveInfoIcon = 0x08107F10|1;
IsMonShiny = 0x0804435c|1;
gPokeBallTiles = 0x0825bd34;         /* const 0825bd34 g 00000060 gBallSpriteSheets */
gPokeBallPals = 0x0825db94;          /* const 0825db94 g 00000060 gBallSpritePalettes */
gBallSpriteTemplates = 0x0825bebc;   /* const 0825bebc g 00000120 gBallSpriteTemplates */
CreateSprite = 0x08006ef8|1;
SpriteCB_TestBallThrow = 0x0804ACC8|1;
SpriteCB_ReleaseMonFromBall = 0x0804B35C|1;
CreatePokeballSpriteToReleaseMon = 0x0804B9FC|1;
objc_ball_instanciate2_entry_804BADC = 0x0804BADC|1;
sub_804BE88    = 0x0804BE88|1;
LoadBallGfx    = 0x0804BFE8|1;
FreeBallGfx    = 0x0804C058|1;
ItemIdToBallId = 0x080EF89C|1;
PokeBallOpenParticleAnimation    = 0x080F0A14|1;
TimerBallOpenParticleAnimation   = 0x080F0B88|1;
DiveBallOpenParticleAnimation    = 0x080F0C8C|1;
SafariBallOpenParticleAnimation  = 0x080F0D90|1;
UltraBallOpenParticleAnimation   = 0x080F0E90|1;
GreatBallOpenParticleAnimation   = 0x080F0F98|1;
RepeatBallOpenParticleAnimation  = 0x080F1124|1;
MasterBallOpenParticleAnimation  = 0x080F1280|1;
PremierBallOpenParticleAnimation = 0x080F13AC|1;
FreeBallTiles = 0x080F1508|1;       /* 080f1508 l 00000080 DestroyBallOpenAnimationParticle */

gSaveDataBuffer = 0x02039A38;
gSaveBlock1ptr = 0x3004F58;
gSaveBlock2ptr = 0x3004F5C;
gSaveBlock3ptr = 0x3005010;
gFirstSaveSector = 0x30052D0;
gSaveCounter = 0x30052E0;
gFastSaveSection = 0x30052E4;
gRamSaveSectionLocations = 0x3005300;
/* ------ */


QuestLog_CutRecording = 0x08112554|1;
SetMainCallback2 = 0x08000540|1;
ScriptContext2_Enable = 0x08069a04|1;
AllocZeroed = 0x08002b1c|1;
Free = 0x08002b30|1;
CreateTask = 0x08077454|1;
RunTasks = 0x080775b0|1;
AnimateSprites = 0x08006ac8|1;
BuildOamBuffer = 0x08006b14|1;
UpdatePaletteFade = 0x08070508|1;

ResetSpriteData = 0x08006a7c|1;
ResetPaletteFade = 0x08070560|1;
FreeAllSpritePalettes = 0x0800885c|1;
ResetTasks = 0x080773f4|1;
ScanlineEffect_Stop = 0x08087e9c|1;

SetVBlankCallback = 0x080006F0|1;
CopyToBgTilemapBuffer = 0x0800203c|1;
SetGpuReg = 0x08000a34|1;
CopyBgTilemapBufferToVram = 0x080020b8|1;
BeginNormalPaletteFade = 0x080705c0|1;
gPaletteFade = 0x02037ab8;
gTasks = 0x03004fe0;
gMain = 0x03003040;

ResetBgsAndClearDma3BusyFlags = 0x08001614|1;
InitBgsFromTemplates = 0x08001654|1;
ChangeBgX = 0x08001b8c|1;
ChangeBgY = 0x08001d04|1;
InitWindows = 0x08003a90|1;
DeactivateAllTextPrinters = 0x08002b94|1;
SetBgTilemapBuffer = 0x08001f9c|1;
ShowBg = 0x080019b8|1;
FillBgTilemapBufferRect_Palette0 = 0x08002450|1;

ResetTempTileDataBuffers = 0x080f6b84|1;
DecompressAndCopyTileDataToVram = 0x080f6bf4|1;
FreeTempTileDataBuffersIfPossible = 0x080f6ba8|1;
LoadPalette = 0x08070424|1;

FillWindowPixelBuffer = 0x080043c8|1;
AddTextPrinterParameterized3 = 0x0812e698|1;
PutWindowTilemap = 0x08003f0c|1;

LoadOam = 0x0800728C|1;
ProcessSpriteCopyRequests = 0x0800757C|1;
TransferPlttBuffer = 0x080704AC|1;

DestroyTask = 0x08077540|1;
FreeAllWindowBuffers = 0x08003e38|1;
CB2_ReturnToFieldFromDiploma = 0x080569f0|1;

ConvertIntToDecimalStringN = 0x08008de4|1;
StringAppend = 0x08008d10|1;
StringCopy = 0x08008cf0|1;
gStringVar1 = 0x02021cd0;
gStringVar2 = 0x02021cf0;
gStringVar3 = 0x02021d04;
gStringVar4 = 0x02021d18;

PlayCry7 = 0x08071F5C|1;
IsCryPlaying = 0x0807224C|1;
CreateMonSprite_FieldMove = 0x08083A0C|1;/*static u8*/
FreeResourcesAndDestroySprite = 0x08083A94|1;
SpriteCallbackDummy = 0x08007578|1;
gSprites = 0x0202063c;

GetMonSpritePalStructFromOtIdPersonality = 0x0804406C|1;
IndexOfSpritePaletteTag = 0x08008954|1;
PreservePaletteInWeather = 0x0807b0bc|1;
CreateMonPicSprite_HandleDeoxys = 0x0810c378|1;
