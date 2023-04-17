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

gEvIv = 0x03007490;

DestroySpriteAndFreeResources = 0x08007770|1;
GetTextWindowPalette = 0x081501f8|1;
ListMenuLoadStdPalAt = 0x08107EE0|1;
BlitMoveInfoIcon = 0x08107F10|1;
CopyWindowToVram = 0x08003E8C|1;

PlaySE = 0x08072304|1;
GetNature = 0x08042D88|1;
gPlayerPartyCount = 0x02024029;
gPlayerParty = 0x02024284;
gBaseStatsPtr = 0x080001BC;
GetMonData = 0x0803fad4|1;
GetSpeciesName = 0x08040EBC|1;
GetMonNickname = 0x08120440|1;

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
gSaveBlock1Ptr = 0x03004F58;
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

PlayCry_Normal = 0x08071E28|1;
PlayCry_Script = 0x08071F5C|1;
IsCryPlaying = 0x0807224C|1;
CreateMonSprite_FieldMove = 0x08083A0C|1;/*static u8*/
FreeResourcesAndDestroySprite = 0x08083A94|1;
SpriteCallbackDummy = 0x08007578|1;
gSprites = 0x0202063c;

GetMonSpritePalStructFromOtIdPersonality = 0x0804406C|1;
IndexOfSpritePaletteTag = 0x08008954|1;
PreservePaletteInWeather = 0x0807b0bc|1;
CreateMonPicSprite_HandleDeoxys = 0x0810c378|1;

/*summary screen*/
sMonSummaryScreen     = 0x0203b140;/*EWRAM*/
sLastViewedMonIndex   = 0x0203b16c;/*EWRAM*/

GetBoxMonData       = 0x0803FC30|1;
BoxMonToMon         = 0x0803E660|1;
PokeSum_RemoveWindows    = 0x081385DC|1;
ConvertIntToHexStringN   = 0x08008E84|1;

PokeSum_TryPlayMonCry = 0x0813B94C|1;
Task_PokeSum_SwitchDisplayedPokemon = 0x0813B5B8|1;
IsPageFlipInput = 0x0813496C|1;
Task_PokeSum_FlipPages = 0x08134D74|1;
PokeSum_SeekToNextMon = 0x0813B2E8|1;
Task_FlipPages_FromInfo = 0x0813504C|1;
Task_DestroyResourcesOnExit = 0x0813802C|1;
MenuHelpers_CallLinkSomething = 0x080BF9C4|1;
LinkRecvQueueLengthMoreThan2 = 0x0800B1DC|1;
FuncIsActiveTask = 0x08077688|1;
HideBg = 0x080019CC|1;
    Overworld_LinkRecvQueueLengthMoreThan2 = 0x08058338|1;
CB2_SetUpPSS = 0x08135DFC|1;
BufferSelectedMonData = 0x08138D54|1;
PokeSum_DestroySprites = 0x0813B158|1;

PokeSum_PrintPageName = 0x08136F6C|1;
PokeSum_PrintControlsString = 0x08136FB8|1;
PrintMonLevelNickOnWindow2 = 0x08137018|1;

SeekToNextMonInBox          = 0x0808BFC0|1;
IsMultiBattle               = 0x0811FB80|1;
IsUpdateLinkStateCBActive   = 0x08056520|1;
gReceivedRemoteLinkPlayers  = 0x03003EB4;/*IWRAM*/

VarGet      = 0x0806E5A0|1;
VarSet      = 0x0806E5BC|1;
FlagSet     = 0x0806E6B8|1;
FlagClear   = 0x0806E6E0|1;
FlagGet     = 0x0806E708|1;

