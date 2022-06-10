#include "global.h"
#include "menu.h"
#include "task.h"
#include "constants/songs.h"
#include "bg.h"
#include "malloc.h"
#include "palette.h"
#include "sprite.h"
#include "window.h"
#include "string_util.h"
#include "sprite.h"

#include "party_menu.h"
#include "scanline_effect.h"
#include "gpu_regs.h"
#include "new_menu_helpers.h"
#include "field_effect.h"
#include "text_window.h"
#include "overworld.h"
#include "field_weather.h"
#include "trainer_pokemon_sprites.h"

#include "pokemon_summary_screen.h"

#include "main_eviv.h"

#define ESP //comment this to use the english text

// 1 = ACTIVADO, 0 = DESACTIVADO. Activa o desactiva el salto de sprite
// 1 = ON, 0 = OFF.  Activates or deactivates the sprite jump
#define SPRITE_JUMP             1

// 1 = DE DERECHA A IZQUIERDA, 0 = EN EL CENTRO -1 = DE IZQUIERDA A DERECHA.
// 1 = FROM RIGHT TO LEFT, 0 = IN THE CENTER -1 = FROM LEFT TO RIGHT.
#define SPRITE_JUMP_DIRECTION   1

// 1 = A LA DERECHA, 0 = A LA IZQUIERDA.
// 1 = RIGHT, 0 = LEFT.
#define SPRITE_VIEW_DIRECTION   0


//coordenada x del sprite pokémon, se mide en tiles de x8 pixeles
//x coordinate of the pokémon sprite, measured in tiles of x8 pixels
#define PICMON_X    18

//coordenada y del sprite pokémon, se mide en tiles de x8 pixeles
//y coordinate of the pokémon sprite, measured in tiles of x8 pixels
#define PICMON_Y     5

static void Task_EvIvInit(u8);
static u8 EvIvLoadGfx(void);
static void EvIvVblankHandler(void);
static void Task_WaitForExit(u8);
static void Task_EvIvReturnToOverworld(u8);
static void BufferMonData(struct Pokemon * mon);
static void ShowSprite(struct Pokemon *mon);
static void EvIvPrintText(struct Pokemon *mon);
static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y);
static void Task_ScriptShowMonPic(u8 taskId);
void HidePokemonPic2(u8 taskId);

static void PrintStat(u8 nature, u8 stat);
u8 GetDigitsDec(u32 num);
u8 GetDigitsHex(u32 num);
static u8 GetColorByNature(u8 nature, u8 stat);

const u32 gBgEvIvGfx[] = INCBIN_U32("graphics/bgEvIv.4bpp.lz");
const u32 gBgEvIvTilemap[] = INCBIN_U32("graphics/bgEvIv.bin.lz");
const u16 gBgEvIvPal[] = INCBIN_U16("graphics/bgEvIv.gbapal");

enum
{
    WIN_POKEMON_NAME,
    WIN_STATS,
    WIN_BOTTOM_BOX
};

extern const struct BaseStats *gBaseStatsPtr;

static const struct BgTemplate bg_Templates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 22,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 1,
    }, {
        .bg = 1,
        .charBaseIndex = 2,
        .mapBaseIndex = 19,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0,
    }
};

//window 0 = pokémon name
#define WINDOW0_WIDTH   29
#define WINDOW0_HEIGTH  2

//window 1 = stats
#define WINDOW1_WIDTH   17
#define WINDOW1_HEIGTH  11
#define WINDOW1_BASEBLOCK  (WINDOW0_WIDTH * WINDOW0_HEIGTH)

//window 2 = text in the bottom bar / texto en la barra inferior
#define WINDOW2_WIDTH   29
#define WINDOW2_HEIGTH  5
#define WINDOW2_BASEBLOCK WINDOW1_WIDTH * WINDOW1_HEIGTH + WINDOW1_BASEBLOCK

static const struct WindowTemplate windows_templates[] = {
    {//window 0 = pokémon name
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = WINDOW0_WIDTH,
        .height = WINDOW0_HEIGTH,
        .paletteNum = 15,
        .baseBlock = 0x000
    },{//window 1 = stats
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = WINDOW1_WIDTH,
        .height = WINDOW1_HEIGTH,
        .paletteNum = 15,
        .baseBlock = WINDOW1_BASEBLOCK
    },{//window 2 = text in the bottom bar / texto en la barra inferior
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = WINDOW2_WIDTH,
        .height = WINDOW2_HEIGTH,
        .paletteNum = 15,
        .baseBlock = WINDOW2_BASEBLOCK
    }, DUMMY_WIN_TEMPLATE
};




/**
 * Acimut
 * 2022-03-02
 * 
 * Es necesario un callback para retornar a la pantalla de Summary Screen.
*/


/*
void ShowPokemonSummaryScreen(
    struct Pokemon * party, 
    u8 cursorPos, 
    u8 lastIdx, 
    MainCallback savedCallback, 
    u8 mode);
*/

struct EvIv
{
    u8 state;
    u8 gfxStep;
    u8 callbackStep;
    bool8 return_summary_screen;

    u8 spriteTaskId;
    u8 cursorPos;               //CALLBACK SUMMARY SCREEN
    u8 lastIdx;                 //CALLBACK SUMMARY SCREEN
    u8 mode;                    //CALLBACK SUMMARY SCREEN
 
    u8 stats_ev[NUM_STATS];
    u8 stats_iv[NUM_STATS];
    u8 stats_bs[NUM_STATS];
    u16 totalStatsEV;
    u16 totalStatsIV;
    u16 totalStatsBS;

    struct Pokemon currentMon;

    union
    {
        struct Pokemon * mons;
        struct BoxPokemon * boxMons;
    } monList;                  //CALLBACK SUMMARY SCREEN

    MainCallback savedCallback; //CALLBACK SUMMARY SCREEN
    u16 tilemapBuffer[0x400];
};

struct PokeSS
{
    struct Pokemon * party;
    u8 cursorPos;
    u8 lastIdx;
    u8 mode;
    u8 taskId;
    MainCallback savedCallback;
};

extern struct PokeSS SS_backup;

extern struct EvIv *gEvIv;

#define gState              gEvIv->state            //Task_WaitForExit  | función que maneja los botones
#define gGfxStep            gEvIv->gfxStep          //EvIvLoadGfx       | carga gráficos y paletas
#define gCallbackStep       gEvIv->callbackStep     //Task_EvIvInit     | función inicializadora
#define gReturn             gEvIv->return_summary_screen
#define gSpriteTaskId       gEvIv->spriteTaskId
#define gStats_ev           gEvIv->stats_ev
#define gStats_iv           gEvIv->stats_iv
#define gStats_bs           gEvIv->stats_bs
#define gTotalStatsEV       gEvIv->totalStatsEV
#define gTotalStatsIV       gEvIv->totalStatsIV
#define gTotalStatsBS       gEvIv->totalStatsBS

//CALLBACK SUMMARY SCREEN
#define SS_party            gEvIv->monList          //Puntero al arreglo de pokémon.
#define SS_cursorPos        gEvIv->cursorPos        //Número del pokémon actual.
#define SS_lastIdx          gEvIv->lastIdx          //Cantidad de pokémon.
#define SS_callback         gEvIv->savedCallback    //Func. de retorno.
#define SS_mode             gEvIv->mode             //Modo.
#define gCurrentMon         gEvIv->currentMon       //buffer del pokémon actual


static void EvIvBgInit(void)
{
    ResetSpriteData();
    ResetPaletteFade();
    FreeAllSpritePalettes();
    ResetTasks();
    ScanlineEffect_Stop();
}

static void CB2_EvIv(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

/**
 * 2022-03-03
 * ACIMUT:
 * Callback summary screen
*/

#define FREE_AND_SET_NULL_IF_SET(ptr) \
{                                     \
    if (ptr != NULL)                  \
    {                                 \
        free(ptr);                    \
        (ptr) = NULL;                 \
    }                                 \
}

void CB2_ShowEvIv_SummaryScreen(void)
{
    if (!gPaletteFade.active)
        Show_EvIv(SS_backup.party,
                    SS_backup.cursorPos,
                    SS_backup.lastIdx,
                    SS_backup.savedCallback,
                    SS_backup.mode,
                    TRUE);
    //FREE_AND_SET_NULL_IF_SET(sMonSummaryScreen);
    //FREE_AND_SET_NULL_IF_SET(sMonSkillsPrinterXpos);
}

/**
 * 2022-03-03
 * ACIMUT:
 * Esta función configura el ev-iv summary screen por 
 * defecto, muestra nuestro equipo pokémon.
*/

void CB2_ShowEvIv_PlayerParty(void)
{
    Show_EvIv(gPlayerParty, 0, (gPlayerPartyCount - 1), NULL, PSS_MODE_NORMAL, FALSE);
}


/**
 * 2022-03-03
 * ACIMUT:
 * Esta función podrá ser llamada por la summary screen,
 * la cual está preparada con suficientes parámetros para
 * recibir argumentos desde la summary screen y así poder
 * volver a la summary screen.
 * 
 * Usar la ventana arriba a la derecha, donde pone:
 * EMERALD:     (A)CANCEL
 * FIRERED:     (+)PAGE (A)CANCEL
 * ESMERALDA:   (A)SALIR
 * ROJOFUEGO:   (+)PÁG. (A)SALIR
 * 
 * Poner en la segunda página:
 * EMERALD:     [(A)EV-IV] 
 * FIRERED:     [(+)PAGE (A)EV-IV]
 * ya que en esta página no se usa esta ventana.
 * #También hacer hook en esa parte para capturar el botón A.
 * 
 * ESMERALDA:
 * Ventana arriba a la derecha:
 * bg = 0
 * pal = 7
 * x = 176 = 0xB0 px = 22t        y = 0
 * tamaño = 64x16 px = 8x2
 * página 2 = POKEMON SKILLS
 * 
 * 
 * FIRERED:
 * Ventana arriba a la derecha:
 * bg = 1 & 2   //wtf!
 * pal = 7
 * x = 152 = 0x98 px = 19t        y = 0
 * tamaño = 88x16 px = 11x2
 * página 2 = HABIL. POKÉMON
 * 
 * -------TAREAS:--------
 * I:       Realizar hook a la función que maneja los botones.
 * Task_InputHandler_Info
 * II: -ok- Realizar hook a la función que carga la ventana X
 * III:-ok- Realizar una excepción a la función de retorno del ev-iv
 *          para que decida si retornar a la summary screen o al ow.
 * IV: -ok- Realizar las excepciones para los tipos de datos de los pokémon,
 *          así poder imprimir datos de Pokemon o BoxPokemon.
*/

void Show_EvIv(struct Pokemon * party, u8 cursorPos, u8 lastIdx, MainCallback savedCallback, u8 mode, bool8 return_summary_screen)
{
    gEvIv = AllocZeroed(sizeof(*gEvIv));

    if (gEvIv == NULL)
    {
        SetMainCallback2(savedCallback);
        return;
    }

    //FREE_AND_SET_NULL_IF_SET(sMonSummaryScreen);
    //FREE_AND_SET_NULL_IF_SET(sMonSkillsPrinterXpos);
    
    gState = 0;
    gGfxStep = 0;
    gCallbackStep = 0;
    
    SS_party.mons = party;
    SS_cursorPos = cursorPos;
    SS_lastIdx = lastIdx;
    SS_callback = savedCallback;
    SS_mode = mode;
    gReturn = return_summary_screen;

    BufferMonData(&gCurrentMon);

    EvIvBgInit();
    CreateTask(Task_EvIvInit, 80);
    SetMainCallback2(CB2_EvIv);
}

static void VCBC_EvIvOam(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void Task_EvIvInit(u8 taskId)
{
    switch (gCallbackStep)
    {
    case 0:
        SetVBlankCallback(NULL);
        break;
    case 1:
        EvIvVblankHandler();
        break;
    case 2:
        if (!EvIvLoadGfx())
        {
            return;
        }
        break;
    case 3:
        CopyToBgTilemapBuffer(1, gBgEvIvTilemap, 0, 0);
        break;
    case 4:
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        break;
    case 5:
        ShowSprite(&gCurrentMon);
        EvIvPrintText(&gCurrentMon);
        break;
    case 6:
        CopyBgTilemapBufferToVram(0);
        CopyBgTilemapBufferToVram(1);
        break;
    case 7:
        BeginNormalPaletteFade(0xFFFFFFFF, 0, 16, 0, RGB_BLACK);
        break;
    case 8:
        SetVBlankCallback(VCBC_EvIvOam);
        break;
    default:
        if (gPaletteFade.active)
        {
            break;
        }
        gTasks[taskId].func = Task_WaitForExit;
    }
    gCallbackStep++;
}

static void Task_WaitForExit(u8 taskId)
{
    switch (gState)
    {
    case 0:
        gState++;
        break;
    case 1:
        if (SS_lastIdx > 0)
        {
            if (JOY_REPT(DPAD_DOWN))
            {
                if (SS_cursorPos == (SS_lastIdx))
                    SS_cursorPos = 0;
                else
                    SS_cursorPos++;

                //si se trata de la caja de pokémon...
                if (SS_mode == PSS_MODE_BOX)
                {
                    //bucle usado en las cajas, donde hay pokémon vacíos.
                    while (GetBoxMonData(&SS_party.boxMons[SS_cursorPos], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
                    {
                        if (SS_cursorPos == (SS_lastIdx))
                            SS_cursorPos = 0;
                        else
                            SS_cursorPos++;

                    }
                }
                BufferMonData(&gCurrentMon);
                HidePokemonPic2(gSpriteTaskId);
                EvIvPrintText(&gCurrentMon);
                ShowSprite(&gCurrentMon);
            }
            else if (JOY_REPT(DPAD_UP))
            {
                if (SS_cursorPos == 0)
                    SS_cursorPos = (SS_lastIdx);
                else
                    SS_cursorPos--;
    
                if (SS_mode == PSS_MODE_BOX)
                {
                    while (GetBoxMonData(&SS_party.boxMons[SS_cursorPos], MON_DATA_SPECIES, NULL) == SPECIES_NONE)
                    {
                        if (SS_cursorPos == 0)
                            SS_cursorPos = (SS_lastIdx);
                        else
                            SS_cursorPos--;
                    }
                }
                BufferMonData(&gCurrentMon);
                HidePokemonPic2(gSpriteTaskId);
                EvIvPrintText(&gCurrentMon);
                ShowSprite(&gCurrentMon);
            }
        }

        if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
        {
#ifdef FIRERED
//FIRERED
            PlaySE(SE_CARD_FLIP);
#else
//EMERALD
            PlaySE(SE_RG_CARD_FLIP);
#endif
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
            gState++;
        }
        break;
    case 2:
        if (!IsCryPlaying())
            Task_EvIvReturnToOverworld(taskId);
        break;
    }
}

void CB2_SummaryScreen(void)
{
    //ResetTasks();//callback de pokesummary debería tenerlo.
    ShowPokemonSummaryScreen(SS_backup.party, SS_backup.cursorPos, SS_backup.lastIdx, SS_backup.savedCallback, SS_backup.mode);
}

//pokemon storage system
//revisar si se puede poner algo en la RAM para identificar cuál callback usar.

//0808ce60 g 00000068 Cb2_ReturnToPSS
extern void Cb2_ReturnToPSS(void);

//0804cf14 g 000005e4 CB2_ReturnToTradeMenuFromSummary
extern void CB2_ReturnToTradeMenuFromSummary(void);

//08122dbc l 00000044 CB2_ReturnToPartyMenuFromSummaryScreen
extern void CB2_ReturnToPartyMenuFromSummaryScreen(void);


static void Task_EvIvReturnToOverworld(u8 taskId)
{
    bool8 return_summary_screen = gReturn;

    if (gPaletteFade.active)
        return;

    HidePokemonPic2(gSpriteTaskId);//bugfix pokémon storage system
    DestroyTask(taskId);
    FreeAllWindowBuffers();
    

    //si retorna a la summary screen
    if (return_summary_screen)
    {
        //struct Pokemon * partyMon;
        //u8 mode, monIndex, maxMonIndex;

        //SS_backup.party = SS_party.mons;
        //SS_backup.cursorPos = SS_cursorPos;
        //SS_backup.lastIdx = SS_lastIdx;
        //SS_backup.savedCallback = SS_callback;
        //SS_backup.mode = SS_mode;

        //partyMon = SS_party.mons;
        //monIndex = SS_cursorPos;
        //maxMonIndex = SS_lastIdx;
        //mode = SS_mode;

        ShowPokemonSummaryScreen(SS_backup.party, SS_backup.cursorPos, SS_backup.lastIdx, SS_backup.savedCallback, SS_backup.mode);

        /*
        //trade menu
        if (SS_callback == &CB2_ReturnToTradeMenuFromSummary)
            ShowPokemonSummaryScreen(partyMon, monIndex, maxMonIndex, CB2_ReturnToTradeMenuFromSummary, mode);

        //pokemon storage system
        else if (SS_callback == &Cb2_ReturnToPSS)
            ShowPokemonSummaryScreen(partyMon, monIndex, maxMonIndex, Cb2_ReturnToPSS, mode);
        
        //party screen
        else if (SS_callback == &CB2_ReturnToPartyMenuFromSummaryScreen)
            ShowPokemonSummaryScreen(partyMon, monIndex, maxMonIndex, CB2_ReturnToPartyMenuFromSummaryScreen, mode);
        //overworld
        else
            ShowPokemonSummaryScreen(partyMon, monIndex, maxMonIndex, CB2_ReturnToField, mode);
        */

        //SetMainCallback2(SS_backup.savedCallback);
        //SetMainCallback2(CB2_SummaryScreen);
        //ShowPokemonSummaryScreen(SS_party.mons, SS_cursorPos, SS_lastIdx, SS_callback, SS_mode);
    }
    else
    {
#ifdef FIRERED
//FIRERED
        SetMainCallback2(CB2_ReturnToFieldFromDiploma);
#else
//EMERALD
        SetMainCallback2(CB2_ReturnToFieldFadeFromBlack);
#endif
    }
    
    //FREE_AND_SET_NULL(gEvIv);
    FREE_AND_SET_NULL_IF_SET(gEvIv);
}

static void BufferMonData(struct Pokemon * mon)
{
    if (SS_mode != PSS_MODE_BOX)
    {
        struct Pokemon * partyMons = SS_party.mons;
        *mon = partyMons[SS_cursorPos];
    }
    else
    {
        struct BoxPokemon * boxMons = SS_party.boxMons;
        BoxMonToMon(&boxMons[SS_cursorPos], mon);
    }
}

static void ResetBGPos(void)
{
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
}

static void ResetGpu(void)
{
    void *vram = (void *)VRAM;
    DmaClearLarge16(3, vram, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG3CNT, 0);
    SetGpuReg(REG_OFFSET_BG3HOFS, 0);
    SetGpuReg(REG_OFFSET_BG3VOFS, 0);
    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
}

static void EvIvVblankHandler(void)
{
    ResetGpu();
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, bg_Templates, 2);
    ResetBGPos();
    InitWindows(windows_templates);
    DeactivateAllTextPrinters();
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    SetBgTilemapBuffer(1, gEvIv->tilemapBuffer);
    ShowBg(0);
    ShowBg(1);
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 30, 20);
    FillBgTilemapBufferRect_Palette0(1, 0, 0, 0, 30, 20);
}

static u8 EvIvLoadGfx(void)
{
    switch (gGfxStep)
    {
    case 0:
        ResetTempTileDataBuffers();
        break;
    case 1:
        DecompressAndCopyTileDataToVram(1, gBgEvIvGfx, 0, 0, 0);
        break;
    case 2:
        if (!(FreeTempTileDataBuffersIfPossible() == 1))
        {
            break;
        }
        return 0;
    case 3:
        LoadPalette(gBgEvIvPal, 0, 0x20);
#ifdef FIRERED
//FIRERED
        LoadPalette(stdpal_get(0), 0xf0, 0x20);
#else
//EMERALD
        LoadPalette(GetTextWindowPalette(0), 0xf0, 0x20);
#endif
        break;
    default:
        return 1;
    }
    gGfxStep++;
    return 0;
}

static void ShowSprite(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 isEgg    = GetMonData(mon, MON_DATA_IS_EGG, NULL);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

    //imprime el sprite del pokémon, si es un huevo no suena grito.
    //Print the sprite of the pokémon, if it is an egg it does not sound a scream.
    if (!isEgg)
    {
        ShowPokemonPic2(species, otId, personality, PICMON_X, PICMON_Y);
#ifdef FIRERED
//FIRERED
        PlayCry7(species, 0);
#else
//EMERALD
        PlayCry1(species, 0);
#endif
    }else
        ShowPokemonPic2(SPECIES_EGG, 0, 0x8000, PICMON_X, PICMON_Y);
}

void HidePokemonPic2(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    task->data[0]++;
}

static u8 CreateMonSprite_Field(u16 species, u32 otId, u32 personality, s16 x, s16 y, u8 subpriority)
{
    const struct CompressedSpritePalette * spritePalette = GetMonSpritePalStructFromOtIdPersonality(species, otId, personality);
    u16 spriteId = CreateMonPicSprite_HandleDeoxys(species, otId, personality, 1, x, y, 0, spritePalette->tag);
    PreservePaletteInWeather(IndexOfSpritePaletteTag(spritePalette->tag) + 0x10);
    if (spriteId == 0xFFFF)
        return MAX_SPRITES;
    else
        return spriteId;
}

static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y)
{
    u8 spriteId;

    spriteId = CreateMonSprite_Field(species, otId, personality, 8 * x + 40, 8 * y + 40, FALSE);
    gSpriteTaskId = CreateTask(Task_ScriptShowMonPic, 80);

    gSprites[spriteId].hFlip = SPRITE_VIEW_DIRECTION;

    //Ajusta el sprite del pokémon 2 píxeles a la izquierda
    //Adjust the pokémon sprite 2 pixels to the left
    gSprites[spriteId].x -= 2;

#if SPRITE_JUMP == 1
    gSprites[spriteId].y -= 32;
    gSprites[spriteId].x += 48 * SPRITE_JUMP_DIRECTION;
#endif

    gTasks[gSpriteTaskId].data[0] = 0;
    gTasks[gSpriteTaskId].data[1] = species;
    gTasks[gSpriteTaskId].data[2] = spriteId;
    gTasks[gSpriteTaskId].data[3] = 0;
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.priority = 0;
}

static void Task_ScriptShowMonPic(u8 taskId)
{
    struct Task * task = &gTasks[taskId];

#if SPRITE_JUMP == 1
    task->data[3]++;
    if (task->data[3] > 0x7FF0)
        task->data[3] = 0xFF;
#endif

    switch (task->data[0])
    {
    case 0:
        task->data[0]++;
        break;
    case 1:

#if SPRITE_JUMP == 1
        u8 tmp_var;
        if (task->data[3] < 48)
        {
            if (task->data[3] < 10)
                tmp_var = 0;
            else if (task->data[3] < 18)                
                tmp_var = 1;
            else if (task->data[3] < 24)
                tmp_var = 2;
            else if (task->data[3] < 28)
                tmp_var = 3;
            else if (task->data[3] < 30)
                tmp_var = -3;
            else if (task->data[3] < 32)
                tmp_var = -2;
            else if (task->data[3] < 36)
                tmp_var = -1;
            else if (task->data[3] < 40)
                tmp_var = 0;
            else if (task->data[3] < 44)
                tmp_var = 1;
            else if (task->data[3] < 46)
                tmp_var = 2;
            else
                tmp_var = 3;

            gSprites[task->data[2]].y += tmp_var;
            gSprites[task->data[2]].x -= SPRITE_JUMP_DIRECTION;
        }
#endif

        break;
    case 2:
        FreeResourcesAndDestroySprite(&gSprites[task->data[2]], task->data[2]);
        task->data[0]++;
        break;
    case 3:
        DestroyTask(taskId);
        break;
    }
}


/*

+- - - - - - - - - - - +
|HP       160  255  16 |  HP_Y
|ATTACK   110   10  30 |  ATK_Y
|DEFENSE   65   20  31 |  DEF_Y
|SP.ATK.   65    1  20 |  SPATK_Y
|SP.DEF.  110   80  31 |  SPDEF_Y
|SPEED     30   11  13 |  SPEED_Y
+- - - - - - - - - - - +
|        |    |    |
 HP_X     BS_X EV_X IV_X

*/

#define HP_X        4
#define BS_X        54
#define EV_X        BS_X + 30
#define IV_X        EV_X + 30

#define HP_Y        3
#define ATK_Y       HP_Y + 14
#define DEF_Y       ATK_Y + 14
#define SPATK_Y     DEF_Y + 14
#define SPDEF_Y     SPATK_Y + 14
#define SPEED_Y     SPDEF_Y + 14



//                                     resaltado                fuente                  sombra
//                                     highlight                font                    shadow
static const u8 gBlackTextColor[3]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GRAY,   TEXT_COLOR_LIGHT_GRAY};
static const u8 gBlueTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_BLUE,        TEXT_COLOR_LIGHT_GRAY};
static const u8 gRedTextColor[3]    = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_RED,         TEXT_COLOR_LIGHT_GRAY};
static const u8 gGrayTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_LIGHT_GRAY,  TEXT_COLOR_DARK_GRAY};

static const u8 *const gTextColorByNature[] = 
{
    gBlackTextColor,
    gRedTextColor,
    gBlueTextColor
};


const u8 gText_Slash[]      = _("/");
const u8 gText_CensorEgg[]  = _("{CLEAR_TO 12}?{CLEAR_TO 42}?{CLEAR_TO 66}?");

const u8 gText_BsEvIv[] = _("BS{CLEAR_TO 30}EV{CLEAR_TO 54}IV");

const u8 gText_Total[]  = _("TOTAL");

#ifdef ESP
//08419c7b g 00000007 gText_PokeSum_Controls_Page
const u8 gText_PokeSum_Controls_Page_EvIv[] = _("{DPAD_LEFTRIGHT}PÁG. {A_BUTTON}EV-IV");
const u8 gText_Hp[]     = _("PS");
const u8 gText_Atq[]    = _("ATAQUE");
const u8 gText_Def[]    = _("DEFENSA");
const u8 gText_SpAtk[]  = _("ATQ.ESP.");
const u8 gText_SpDef[]  = _("DEF.ESP.");
const u8 gText_Speed[]  = _("VELOCID.");

const u8 gText_Your[]   = _("Tu ");
const u8 gText_Is[]     = _(" es ");
const u8 gText_Happy[]  = _("% feliz.");

//const u8 gText_Less_Than[]  = _("¡Falta ");//old code
const u8 gText_Steps_to_hatching[]  = _("Pasos para eclosionar: ");
#else
// ENG
const u8 gText_PokeSum_Controls_Page_EvIv[] = _("{DPAD_LEFTRIGHT}PAGE {A_BUTTON}EV-IV");
const u8 gText_Hp[]     = _("HP");
const u8 gText_Atq[]    = _("ATTACK");
const u8 gText_Def[]    = _("DEFENSE");
const u8 gText_SpAtk[]  = _("SP.ATK.");
const u8 gText_SpDef[]  = _("SP.DEF.");
const u8 gText_Speed[]  = _("SPEED");

const u8 gText_Your[]   = _("Your ");
const u8 gText_Is[]     = _(" is ");
const u8 gText_Happy[]  = _("% happy.");

//const u8 gText_Less_Than[]  = _(" ... ");//old code
const u8 gText_Steps_to_hatching[]  = _("Steps to hatching: ");
#endif

static void PrintWindow0(struct Pokemon *mon);
static void PrintWindow1(u8 nature, u8 isEgg);
static void PrintWindow2(u16 species, u8 isEgg, u8 friendship);

static void EvIvPrintText(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 nature   = GetNature(mon);
    u8 isEgg    = GetMonData(mon, MON_DATA_IS_EGG, NULL);
    u8 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);

    //reinicia los totales.
    //reset the totals.
    gTotalStatsEV = 0;
    gTotalStatsIV = 0;
    gTotalStatsBS = 0;

    //obtiene las estadísticas del pokémon
    //get pokémon stats

    //STAT_HP
    gStats_bs[STAT_HP] = gBaseStatsPtr[species].baseHP;
    gStats_ev[STAT_HP] = GetMonData(mon,MON_DATA_HP_EV,NULL);
    gStats_iv[STAT_HP] = GetMonData(mon,MON_DATA_HP_IV,NULL);

    //STAT_ATK
    gStats_bs[STAT_ATK] = gBaseStatsPtr[species].baseAttack;
    gStats_ev[STAT_ATK] = GetMonData(mon,MON_DATA_ATK_EV,NULL);
    gStats_iv[STAT_ATK] = GetMonData(mon,MON_DATA_ATK_IV,NULL);

    //STAT_DEF
    gStats_bs[STAT_DEF] = gBaseStatsPtr[species].baseDefense;
    gStats_ev[STAT_DEF] = GetMonData(mon,MON_DATA_DEF_EV,NULL);
    gStats_iv[STAT_DEF] = GetMonData(mon,MON_DATA_DEF_IV,NULL);

    //STAT_SPATK
    gStats_bs[STAT_SPATK] = gBaseStatsPtr[species].baseSpAttack;
    gStats_ev[STAT_SPATK] = GetMonData(mon,MON_DATA_SPATK_EV,NULL);
    gStats_iv[STAT_SPATK] = GetMonData(mon,MON_DATA_SPATK_IV,NULL);
    
    //STAT_SPDEF
    gStats_bs[STAT_SPDEF] = gBaseStatsPtr[species].baseSpDefense;
    gStats_ev[STAT_SPDEF] = GetMonData(mon,MON_DATA_SPDEF_EV,NULL);
    gStats_iv[STAT_SPDEF] = GetMonData(mon,MON_DATA_SPDEF_IV,NULL);
    
    //STAT_SPEED
    gStats_bs[STAT_SPEED] = gBaseStatsPtr[species].baseSpeed;
    gStats_ev[STAT_SPEED] = GetMonData(mon,MON_DATA_SPEED_EV,NULL);
    gStats_iv[STAT_SPEED] = GetMonData(mon,MON_DATA_SPEED_IV,NULL);

    //realiza la suma de los totales
    //performs the sum of the totals
    for (int i = 0; i < NUM_STATS; i++)
    {
        gTotalStatsEV += gStats_ev[i];
        gTotalStatsIV += gStats_iv[i];
        gTotalStatsBS += gStats_bs[i];
    }

    FillWindowPixelBuffer(0, 0);
    FillWindowPixelBuffer(1, 0);
    FillWindowPixelBuffer(2, 0);

    PrintWindow0(mon);
    PrintWindow1(nature, isEgg);
    PrintWindow2(species, isEgg, friendship);

    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
}

static void PrintWindow0(struct Pokemon *mon)
{
    u8 temp = 0;

    temp = SS_cursorPos + 1;
    ConvertIntToDecimalStringN(gStringVar4, temp, STR_CONV_MODE_LEFT_ALIGN, GetDigitsDec(temp));
    StringAppend(gStringVar4, gText_Slash);
    temp = SS_lastIdx + 1;
    ConvertIntToDecimalStringN(gStringVar1, temp, STR_CONV_MODE_LEFT_ALIGN, GetDigitsDec(temp));
    StringAppend(gStringVar4, gStringVar1);
    AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 2, 2, gGrayTextColor, 0, gStringVar4);

    AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 0x44, 2, gGrayTextColor, 0, gText_BsEvIv);

    GetMonNickname(mon, gStringVar4);
    AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 0x90, 2, gGrayTextColor, 0, gStringVar4);
}

static void PrintWindow1(u8 nature, u8 isEgg)
{
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, HP_Y,    gTextColorByNature[GetColorByNature(nature, STAT_HP)],    0, gText_Hp);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, ATK_Y,   gTextColorByNature[GetColorByNature(nature, STAT_ATK)],   0, gText_Atq);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, DEF_Y,   gTextColorByNature[GetColorByNature(nature, STAT_DEF)],   0, gText_Def);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, SPATK_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPATK)], 0, gText_SpAtk);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, SPDEF_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPDEF)], 0, gText_SpDef);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, SPEED_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPEED)], 0, gText_Speed);

    if (!isEgg)
    {
        PrintStat(nature, STAT_HP);
        PrintStat(nature, STAT_ATK);
        PrintStat(nature, STAT_DEF);
        PrintStat(nature, STAT_SPATK);
        PrintStat(nature, STAT_SPDEF);
        PrintStat(nature, STAT_SPEED);
    }else{
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, HP_Y,    gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATK_Y,   gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEF_Y,   gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPATK_Y, gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPDEF_Y, gBlackTextColor, 0, gText_CensorEgg);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPEED_Y, gBlackTextColor, 0, gText_CensorEgg);
    }
}

static void PrintStat(u8 nature, u8 stat)
{
    u8 color = GetColorByNature(nature, stat);

    ConvertIntToDecimalStringN(gStringVar1, gStats_bs[stat], STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar2, gStats_ev[stat], STR_CONV_MODE_RIGHT_ALIGN, 3);
    ConvertIntToDecimalStringN(gStringVar3, gStats_iv[stat], STR_CONV_MODE_RIGHT_ALIGN, 2);

    switch (stat)
    {
    case STAT_HP:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, HP_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, HP_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, HP_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_ATK:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATK_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, ATK_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, ATK_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_DEF:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEF_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, DEF_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, DEF_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPATK:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPATK_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, SPATK_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, SPATK_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPDEF:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPDEF_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, SPDEF_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, SPDEF_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPEED:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, SPEED_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, SPEED_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, SPEED_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    default:
        break;
    }
}

static void PrintWindow2(u16 species, u8 isEgg, u8 friendship)
{
    u32 temp = 0;

    if(!isEgg)
    {
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 14, 4, gBlackTextColor, 0, gText_Total);

        ConvertIntToDecimalStringN(gStringVar1, gTotalStatsBS, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar2, gTotalStatsEV, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar3, gTotalStatsIV, STR_CONV_MODE_RIGHT_ALIGN, 3);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, BS_X + 8, 4, gBlackTextColor, 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, EV_X + 8, 4, gBlackTextColor, 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, IV_X + 2, 4, gBlackTextColor, 0, gStringVar3);

        StringCopy(gStringVar4, gText_Your);
        GetSpeciesName(gStringVar1, species);
        StringAppend(gStringVar4, gStringVar1);
        StringAppend(gStringVar4, gText_Is);
        temp = (friendship * 100) / 0xFF;
        
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar4, gStringVar2);
        
        StringAppend(gStringVar4, gText_Happy);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 14, 18, gBlackTextColor, 0, gStringVar4);
        
    }else
    {
        temp = ((friendship + 1) * 0x100) - (gSaveBlock1Ptr->daycare.stepCounter + 1);
        if (gSaveBlock1Ptr->daycare.stepCounter == 0xFF)
            temp += 0x100;
        //StringCopy(gStringVar4, gText_Less_Than);//old code
        StringCopy(gStringVar4, gText_Steps_to_hatching);//new code
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, GetDigitsDec(temp));
        StringAppend(gStringVar4, gStringVar2);
        //StringAppend(gStringVar4, gText_Steps_to_hatching);//old code
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 2, 18, gBlackTextColor, 0, gStringVar4);
    }
    temp = SS_callback;
    //temp = SS_backup.savedCallback;
    ConvertIntToHexStringN(gStringVar4, temp, STR_CONV_MODE_LEFT_ALIGN, GetDigitsHex(temp));
    AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 0, 0x90, 2, gBlackTextColor, 0, gStringVar4);
}

/**
 * Devuelve el número de dígitos de un número
 * 
 * Returns the number of digits in a number
*/
u8 GetDigitsDec(u32 num)
{
    if (num < 10)
        return 1;
    else if (num < 100)
        return 2;
    else if (num < 1000)
        return 3;
    else if (num < 10000)
        return 4;
    else if (num < 100000)
        return 5;
    else if (num < 1000000)
        return 6;
    else if (num < 10000000)
        return 7;
    else if (num < 100000000)
        return 8;
    else if (num < 1000000000)
        return 9;
    else
        return 10;
    
    /*if (num > 10)
        return GetDigitsDec(num/10);
    else
        return 1;*/
}

u8 GetDigitsHex(u32 num)
{
    if (num < 0x10)
        return 1;
    else if (num < 0x100)
        return 2;
    else if (num < 0x1000)
        return 3;
    else if (num < 0x10000)
        return 4;
    else if (num < 0x100000)
        return 5;
    else if (num < 0x1000000)
        return 6;
    else if (num < 0x10000000)
        return 7;
    else
        return 8;
}


/**
 * Devuelve el número de dígitos de un entero con signo
 * 
 * Returns the number of digits in a signed int
*/

/*
u8 GetDigitsDec_S(s32 num)
{
    if (num < 0)
        num * (-1);
    
    if (num < 10)
        return 1;
    else if (num < 100)
        return 2;
    else if (num < 1000)
        return 3;
    else if (num < 10000)
        return 4;
    else if (num < 100000)
        return 5;
    else if (num < 1000000)
        return 6;
    else if (num < 10000000)
        return 7;
    else if (num < 100000000)
        return 8;
    else if (num < 1000000000)
        return 9;
    else
        return 10;
}*/

/**
 * Esta función devuelve un número, de acuerdo a la naturaleza.
 * 0 = negro
 * 1 = rojo
 * 2 = azul
 * 
 * This function returns a number, according to nature.
 * 0 = black
 * 1 = red
 * 2 = blue
*/
static u8 GetColorByNature(u8 nature, u8 stat)
{
    switch (stat)
    {
    case STAT_ATK:
        switch (nature)
        {
        case NATURE_LONELY:
        case NATURE_BRAVE:
        case NATURE_ADAMANT:
        case NATURE_NAUGHTY:
            return 1;
        case NATURE_BOLD:
        case NATURE_TIMID:
        case NATURE_MODEST:
        case NATURE_CALM:
            return 2;
        
        default:
            return 0;
        }
    case STAT_DEF:
        switch (nature)
        {
        case NATURE_BOLD:
        case NATURE_RELAXED:
        case NATURE_IMPISH:
        case NATURE_LAX:
            return 1;
        case NATURE_LONELY:
        case NATURE_HASTY:
        case NATURE_MILD:
        case NATURE_GENTLE:
            return 2;
        
        default:
            return 0;
        }
    case STAT_SPATK:
        switch (nature)
        {
        case NATURE_MODEST:
        case NATURE_MILD:
        case NATURE_QUIET:
        case NATURE_RASH:
            return 1;
        case NATURE_ADAMANT:
        case NATURE_IMPISH:
        case NATURE_JOLLY:
        case NATURE_CAREFUL:
            return 2;
        
        default:
            return 0;
        }
    case STAT_SPDEF:
        switch (nature)
        {
        case NATURE_CALM:
        case NATURE_GENTLE:
        case NATURE_SASSY:
        case NATURE_CAREFUL:
            return 1;
        case NATURE_NAUGHTY:
        case NATURE_LAX:
        case NATURE_NAIVE:
        case NATURE_RASH:
            return 2;
        
        default:
            return 0;
        }
    case STAT_SPEED:
        switch (nature)
        {
        case NATURE_TIMID:
        case NATURE_HASTY:
        case NATURE_JOLLY:
        case NATURE_NAIVE:
            return 1;
        case NATURE_BRAVE:
        case NATURE_RELAXED:
        case NATURE_QUIET:
        case NATURE_SASSY:
            return 2;
        
        default:
            return 0;
        }
    case STAT_HP:
    default:
        return 0;
    }
}

/**
 * Acimut
 * 2022-02
 * Este código es el encargado de calcular el poder y tipo
 * del movimiento poder oculto.
 * 
 * La operación |= 0xC0 es para que el valor no se reconozca como
 * cero en algunas funciones, donde se usa & 0x3F para obtener el
 * valor original, los primeros 6 bits.
*/


/*
static void atkC1_hiddenpowercalc(void)
{
    s32 powerBits, typeBits;

    powerBits = ((gBattleMons[gBattlerAttacker].hpIV & 2) >> 1)
              | ((gBattleMons[gBattlerAttacker].attackIV & 2) << 0)
              | ((gBattleMons[gBattlerAttacker].defenseIV & 2) << 1)
              | ((gBattleMons[gBattlerAttacker].speedIV & 2) << 2)
              | ((gBattleMons[gBattlerAttacker].spAttackIV & 2) << 3)
              | ((gBattleMons[gBattlerAttacker].spDefenseIV & 2) << 4);
    typeBits  = ((gBattleMons[gBattlerAttacker].hpIV & 1) << 0)
              | ((gBattleMons[gBattlerAttacker].attackIV & 1) << 1)
              | ((gBattleMons[gBattlerAttacker].defenseIV & 1) << 2)
              | ((gBattleMons[gBattlerAttacker].speedIV & 1) << 3)
              | ((gBattleMons[gBattlerAttacker].spAttackIV & 1) << 4)
              | ((gBattleMons[gBattlerAttacker].spDefenseIV & 1) << 5);
    gDynamicBasePower = (40 * powerBits) / 63 + 30;
    gBattleStruct->dynamicMoveType = (15 * typeBits) / 63 + 1;
    if (gBattleStruct->dynamicMoveType >= TYPE_MYSTERY)
        ++gBattleStruct->dynamicMoveType;
    gBattleStruct->dynamicMoveType |= 0xC0;
    ++gBattlescriptCurrInstr;
}
*/

//08134840 l 0000036c Task_InputHandler_Info
//08134A52
void hoook_Task_InputHandler_Info(void)
{
    if (sMonSummaryScreen->curPageIndex == PSS_PAGE_INFO)
    {
        PlaySE(SE_SELECT);
        sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_FADEOUT;
    }
    else if (sMonSummaryScreen->curPageIndex == PSS_PAGE_SKILLS)
    {
#ifdef FIRERED
//FIFERED
        PlaySE(SE_CARD_OPEN);   
#else
//EMERALD
        PlaySE(SE_RG_CARD_OPEN);
#endif
        sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_FADEOUT;
        SetVBlankCallback(NULL);

        SS_backup.party = sMonSummaryScreen->monList.mons;
        SS_backup.cursorPos = sLastViewedMonIndex;
        SS_backup.lastIdx = sMonSummaryScreen->lastIndex;
        SS_backup.savedCallback = sMonSummaryScreen->savedCallback;
        SS_backup.mode = sMonSummaryScreen->mode;
        SetMainCallback2(CB2_ShowEvIv_SummaryScreen);

        //Show_EvIv(sMonSummaryScreen->monList.mons,
        //        sLastViewedMonIndex,
        //        sMonSummaryScreen->lastIndex,
        //        sMonSummaryScreen->savedCallback,
        //        sMonSummaryScreen->mode,
        //        TRUE);
    }
    else if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES)
    {
        PlaySE(SE_SELECT);
        sMonSummaryScreen->pageFlipDirection = 1;
        PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
        sMonSummaryScreen->curPageIndex++;
        sMonSummaryScreen->state3270 = PSS_STATE3270_FLIPPAGES;
    }
    //return;
}

#include "menu_helpers.h"
#include "link.h"

//static 
extern void PokeSum_TryPlayMonCry(void);
extern void Task_PokeSum_SwitchDisplayedPokemon(u8 taskId);
extern bool32 IsPageFlipInput(u8 direction);
extern void Task_PokeSum_FlipPages(u8 taskId);
extern void PokeSum_SeekToNextMon(u8 taskId, s8 direction);
extern void Task_FlipPages_FromInfo(u8 taskId);
extern void Task_DestroyResourcesOnExit(u8 taskId);

//static 
void New_Task_InputHandler_Info(u8 taskId)
{
    switch (sMonSummaryScreen->state3270) {
    case PSS_STATE3270_FADEIN:
        BeginNormalPaletteFade(0xffffffff, 0, 16, 0, 0);
        sMonSummaryScreen->state3270 = PSS_STATE3270_PLAYCRY;
        break;
    case PSS_STATE3270_PLAYCRY:
        if (!gPaletteFade.active)
        {
            PokeSum_TryPlayMonCry();
            sMonSummaryScreen->state3270 = PSS_STATE3270_HANDLEINPUT;
            return;
        }

        sMonSummaryScreen->state3270 = PSS_STATE3270_PLAYCRY;
        break;
    case PSS_STATE3270_HANDLEINPUT:
        if (MenuHelpers_CallLinkSomething() == TRUE)
            return;
        else if (LinkRecvQueueLengthMoreThan2() == TRUE)
            return;
        else if (FuncIsActiveTask(Task_PokeSum_SwitchDisplayedPokemon))
            return;

        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            if (IsPageFlipInput(1) == TRUE)
            {
                if (FuncIsActiveTask(Task_PokeSum_FlipPages))
                {
                    sMonSummaryScreen->lastPageFlipDirection = 1;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex < PSS_PAGE_MOVES)
                {
                    PlaySE(SE_SELECT);
                    HideBg(0);
                    sMonSummaryScreen->pageFlipDirection = 1;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex++;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_FLIPPAGES;
                }
                return;
            }
            else if (IsPageFlipInput(0) == TRUE)
            {
                if (FuncIsActiveTask(Task_PokeSum_FlipPages))
                {
                    sMonSummaryScreen->lastPageFlipDirection = 0;
                    return;
                }
                else if (sMonSummaryScreen->curPageIndex > PSS_PAGE_INFO)
                {
                    PlaySE(SE_SELECT);
                    HideBg(0);
                    sMonSummaryScreen->pageFlipDirection = 0;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex--;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_FLIPPAGES;
                }
                return;
            }
        }

        if ((!FuncIsActiveTask(Task_PokeSum_FlipPages)) || FuncIsActiveTask(Task_PokeSum_SwitchDisplayedPokemon))
        {
            if (JOY_NEW(DPAD_UP))
            {
                PokeSum_SeekToNextMon(taskId, -1);
                return;
            }
            else if (JOY_NEW(DPAD_DOWN))
            {
                PokeSum_SeekToNextMon(taskId, 1);
                return;
            }
            else if (JOY_NEW(A_BUTTON))
            {
                if (sMonSummaryScreen->curPageIndex == PSS_PAGE_INFO)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_FADEOUT;
                }
                else if (sMonSummaryScreen->curPageIndex == PSS_PAGE_SKILLS)
                {
#ifdef FIRERED
//FIFERED
                    PlaySE(SE_CARD_OPEN);   
#else
//EMERALD
                    PlaySE(SE_RG_CARD_OPEN);
#endif
                    SS_backup.party = sMonSummaryScreen->monList.mons;
                    SS_backup.cursorPos = sLastViewedMonIndex;
                    SS_backup.lastIdx = sMonSummaryScreen->lastIndex;
                    SS_backup.savedCallback = sMonSummaryScreen->savedCallback;
                    SS_backup.mode = sMonSummaryScreen->mode;
                    SS_backup.taskId = taskId;

                    SetVBlankCallback(NULL);//revisar
                    SetMainCallback2(CB2_ShowEvIv_SummaryScreen);
                    //ScriptContext2_Enable();//revisar

                    sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_FADEOUT;
                }
                else if (sMonSummaryScreen->curPageIndex == PSS_PAGE_MOVES)
                {
                    PlaySE(SE_SELECT);
                    sMonSummaryScreen->pageFlipDirection = 1;
                    PokeSum_RemoveWindows(sMonSummaryScreen->curPageIndex);
                    sMonSummaryScreen->curPageIndex++;
                    sMonSummaryScreen->state3270 = PSS_STATE3270_FLIPPAGES;
                }
                return;
            }
            else if (JOY_NEW(B_BUTTON))
            {
                sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_FADEOUT;
            }
        }
        break;
    case PSS_STATE3270_FLIPPAGES:
        if (sMonSummaryScreen->curPageIndex != PSS_PAGE_MOVES_INFO)
        {
            CreateTask(Task_PokeSum_FlipPages, 0);
            sMonSummaryScreen->state3270 = PSS_STATE3270_HANDLEINPUT;
        }
        else
        {
            gTasks[sMonSummaryScreen->inputHandlerTaskId].func = Task_FlipPages_FromInfo;
            sMonSummaryScreen->state3270 = PSS_STATE3270_HANDLEINPUT;
        }
        break;
    case PSS_STATE3270_ATEXIT_FADEOUT:
        BeginNormalPaletteFade(0xffffffff, 0, 0, 16, 0);
        sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_WAITLINKDELAY;
        break;
    case PSS_STATE3270_ATEXIT_WAITLINKDELAY:
        if (Overworld_LinkRecvQueueLengthMoreThan2() == TRUE)
            return;
        else if (LinkRecvQueueLengthMoreThan2() == TRUE)
            return;

        sMonSummaryScreen->state3270 = PSS_STATE3270_ATEXIT_WAITFADE;
        break;
    default:
        if (!gPaletteFade.active)
            Task_DestroyResourcesOnExit(taskId);

        break;
    }
}