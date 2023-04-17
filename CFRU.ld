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

gEvIv = 0x03007490;

DestroySpriteAndFreeResources = 0x08007804|1;
GetTextWindowPalette = 0x08150408|1;
ListMenuLoadStdPalAt = 0x08107d38|1;
BlitMoveInfoIcon = 0x08107d68|1;
CopyWindowToVram = 0x08003f20|1;

PlaySE = 0x080722cc|1;
GetNature = 0x08042e9c|1;
gPlayerPartyCount = 0x02024029;
gPlayerParty = 0x02024284;
gBaseStatsPtr = 0x080001BC;
GetMonData = 0x0803fbe8|1;
GetSpeciesName = 0x08040fd0|1;
GetMonNickname = 0x081202e0|1;

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
gSaveBlock1Ptr = 0x03005008;
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

PlayCry_Normal = 0x08071df0|1;
PlayCry_Script = 0x08071f24|1;
IsCryPlaying = 0x08072214|1;
CreateMonSprite_FieldMove = 0x080839d4|1;/*static u8*/
FreeResourcesAndDestroySprite = 0x08083a5c|1;
SpriteCallbackDummy = 0x0800760c|1;
gSprites = 0x0202063c;

GetMonSpritePalStructFromOtIdPersonality = 0x08044180|1;
IndexOfSpritePaletteTag = 0x080089e8|1;
PreservePaletteInWeather = 0x0807b084|1;
CreateMonPicSprite_HandleDeoxys = 0x0810c1cc|1;

/*summary screen*/
sMonSummaryScreen     = 0x0203b140;/*EWRAM*/
sLastViewedMonIndex   = 0x0203b16c;/*EWRAM*/

GetBoxMonData       = 0x0803fd44|1;
BoxMonToMon         = 0x0803e774|1;
PokeSum_RemoveWindows    = 0x08138414|1;
ConvertIntToHexStringN   = 0x08008f18|1;

PokeSum_TryPlayMonCry = 0x0813b784|1;
Task_PokeSum_SwitchDisplayedPokemon = 0x0813b3f0|1;
IsPageFlipInput = 0x081347a4|1;
Task_PokeSum_FlipPages = 0x08134bac|1;
PokeSum_SeekToNextMon = 0x0813b120|1;
Task_FlipPages_FromInfo = 0x08134e84|1;
Task_DestroyResourcesOnExit = 0x08137e64|1;
MenuHelpers_CallLinkSomething = 0x080bf72c|1;
LinkRecvQueueLengthMoreThan2 = 0x0800b270|1;
FuncIsActiveTask = 0x08077650|1;
HideBg = 0x080019d0|1;
Overworld_LinkRecvQueueLengthMoreThan2 = 0x08058244|1;
CB2_SetUpPSS = 0x08135c34|1;
BufferSelectedMonData = 0x08138b8c|1;
PokeSum_DestroySprites = 0x0813af90|1;

PokeSum_PrintPageName = 0x08136da4|1;
PokeSum_PrintControlsString = 0x08136df0|1;
PrintMonLevelNickOnWindow2 = 0x08136e50|1;

SeekToNextMonInBox = 0x0808bde8|1;
IsMultiBattle = 0x0811fa20|1;
IsUpdateLinkStateCBActive = 0x0805642c|1;
gReceivedRemoteLinkPlayers = 0x03003f64;/*IWRAM*/

VarGet      = 0x0806e568|1;
VarSet      = 0x0806e584|1;
FlagSet     = 0x0806e680|1;
FlagClear   = 0x0806e6a8|1;
FlagGet     = 0x0806e6d0|1;

