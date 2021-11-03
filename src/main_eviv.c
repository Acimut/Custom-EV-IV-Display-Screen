#include "include/global.h"
#include "include/menu.h"
#include "include/task.h"
#include "include/constants/fanfares.h"
#include "include/constants/songs.h"
#include "include/bg.h"
//#include "include/main.h"
#include "include/malloc.h"
#include "include/palette.h"
#include "include/sprite.h"
#include "include/window.h"
#include "include/string_util.h"
#include "include/sprite.h"

#include "main_eviv.h"

#define SPRITE_SALTO             1   //1 = ACTIVADO, 0 = DESACTIVADO, activa la animación de salto del sprite del pokémon.
#define SPRITE_DIRECCION_SALTO   1   //1 = DE DERECHA A IZQUIERDA, 0 = EN EL CENTRO -1 = DE IZQUIERDA A DERECHA, dirección a donde salta el pokémon.
#define SPRITE_DIRECCION_VISTA   0   //1 = A LA DERECHA, 0 = A LA IZQUIERDA, dirección a la que mira el sprite del pokémon.

#define PICMON_X    18          //coordenada x del sprite pokémon, se mide en tiles de x8 pixeles
#define PICMON_Y     5          //coordenada y del sprite pokémon, se mide en tiles de x8 pixeles

static void Task_EvIvInit(u8);
static u8 EvIvLoadGfx(void);
static void EvIvVblankHandler(void);
static void Task_WaitForExit(u8);
static void Task_EvIvReturnToOverworld(u8);
static void MostrarSprite(struct Pokemon *mon);
static void EvIvPrintText(struct Pokemon *mon);
//static void AgregarColorPorNaturaleza(u8 nature, u8 stat);
//static void AddNumInto_gStringVar4(u16 num);
static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y);
static void Task_ScriptShowMonPic(u8 taskId);
static void HidePokemonPic2(u8 taskId);

static void PrintStat(u8 nature, u8 stat);
static u8 GetDigits(u16 num);
static u8 GetColorByNature(u8 nature, u8 stat);

const u32 gBgEvIvGfx[] = INCBIN_U32("graphics/bgEvIv.4bpp.lz");
const u32 gBgEvIvTilemap[] = INCBIN_U32("graphics/bgEvIv.bin.lz");
const u16 gBgEvIvPal[] = INCBIN_U16("graphics/bgEvIv.gbapal");

enum
{
    WIN_NICK_POKEMON,
    WIN_STATS,
    WIN_BOTTOM_BOX
};

extern const struct BaseStats *gBaseStatsPtr;

static const struct BgTemplate plantilaBG[] = {
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

//ventana 0 = nombre del pokémon
#define VENTANA0_ANCHO  29
#define VENTANA0_ALTO    2

//ventana 1 = estadísticas
#define VENTANA1_ANCHO  17
#define VENTANA1_ALTO   11
#define VENTANA1_BASEBLOCK  (VENTANA0_ANCHO * VENTANA0_ALTO)    //(width * height) + baseBlock del WindowTemplate anterior

//ventana 2 = texto en la barra inferior
#define VENTANA2_ANCHO  29
#define VENTANA2_ALTO    5
#define VENTANA2_BASEBLOCK VENTANA1_ANCHO * VENTANA1_ALTO + VENTANA1_BASEBLOCK  //(width * height) + baseBlock del WindowTemplate anterior

static const struct WindowTemplate plantilla_de_ventanas[] = {
    {//ventana 0 = nombre del pokémon
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = VENTANA0_ANCHO,
        .height = VENTANA0_ALTO,
        .paletteNum = 15,
        .baseBlock = 0x000
    },{//ventana 1 = estadísticas
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = VENTANA1_ANCHO,
        .height = VENTANA1_ALTO,
        .paletteNum = 15,
        .baseBlock = VENTANA1_BASEBLOCK
    },{//ventana 2 = texto en la barra inferior
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = VENTANA2_ANCHO,
        .height = VENTANA2_ALTO,
        .paletteNum = 15,
        .baseBlock = VENTANA2_BASEBLOCK
    }, DUMMY_WIN_TEMPLATE
};

struct EvIv
{
    u8 state;                   //usado en Task_WaitForExit
    u8 gfxStep;                 //usado en EvIvLoadGfx
    u8 callbackStep;            //usado en Task_EvIvInit
    u8 actualMon;               //usado para almacenar la posición del pokémon
    u8 spriteTaskId;            //usado para almacenar la taskId de la tarea que mueve al pokémon.
    u8 stats_ev[NUM_STATS];     //usado para imprimir las estadísticas de ev
    u8 stats_iv[NUM_STATS];     //usado para imprimir las estadísticas de iv
    u8 stats_bs[NUM_STATS];     //usado para imprimir las estadísticas de bs
    u16 totalStatsEV;           //usado para almacenar e imprimir el total de estadísticas
    u16 totalStatsIV;           //usado para almacenar e imprimir el total de estadísticas
    u16 totalStatsBS;           //usado para almacenar e imprimir el total de estadísticas
    u16 tilemapBuffer[0x400];   //buffer para el bg
};

extern struct EvIv *gEvIv;

#define gState              gEvIv->state
#define gGfxStep            gEvIv->gfxStep
#define gCallbackStep       gEvIv->callbackStep
#define gActualMon          gEvIv->actualMon
#define gSpriteTaskId       gEvIv->spriteTaskId
#define gStats_ev           gEvIv->stats_ev
#define gStats_iv           gEvIv->stats_iv
#define gStats_bs           gEvIv->stats_bs
#define gTotalStatsEV       gEvIv->totalStatsEV
#define gTotalStatsIV       gEvIv->totalStatsIV
#define gTotalStatsBS       gEvIv->totalStatsBS
//#define gTilemapBuffer      gEvIv->tilemapBuffer

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

void CB2_ShowEvIv(void)
{
    gEvIv = AllocZeroed(sizeof(*gEvIv));
    gState = 0;
    gGfxStep = 0;
    gCallbackStep = 0;
    gActualMon = 0;
    EvIvBgInit();
    CreateTask(Task_EvIvInit, 0);
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
        MostrarSprite(&gPlayerParty[gActualMon]);
        EvIvPrintText(&gPlayerParty[gActualMon]);
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
        if (JOY_REPT(DPAD_DOWN) && gPlayerPartyCount > 1)
        {
            //PlaySE(SE_SELECT);
            if (gActualMon == (gPlayerPartyCount - 1))
                gActualMon = 0;
            else
                gActualMon++;
            HidePokemonPic2(gSpriteTaskId);
            MostrarSprite(&gPlayerParty[gActualMon]);
            EvIvPrintText(&gPlayerParty[gActualMon]);
        }
        if (JOY_REPT(DPAD_UP) && gPlayerPartyCount > 1)
        {
            //PlaySE(SE_SELECT);
            if (gActualMon == 0)
                gActualMon = (gPlayerPartyCount - 1);
            else
                gActualMon--;
            HidePokemonPic2(gSpriteTaskId);
            MostrarSprite(&gPlayerParty[gActualMon]);
            EvIvPrintText(&gPlayerParty[gActualMon]);
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

static void Task_EvIvReturnToOverworld(u8 taskId)
{
    if (gPaletteFade.active)
        return;
    DestroyTask(taskId);
    FreeAllWindowBuffers();
    FREE_AND_SET_NULL(gEvIv);
#ifdef FIRERED
//FIRERED
        SetMainCallback2(CB2_ReturnToFieldFromDiploma);
#else
//EMERALD
        SetMainCallback2(CB2_ReturnToFieldFadeFromBlack);
#endif
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
    InitBgsFromTemplates(0, plantilaBG, 2);
    ResetBGPos();
    InitWindows(plantilla_de_ventanas);
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

#define SPECIES_EGG 412         //es el número de especie asignado al sprite del huevo.

static void MostrarSprite(struct Pokemon *mon)
{
    u16 species = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 isEgg    = GetMonData(mon, MON_DATA_IS_EGG, NULL);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

    //imprime el sprite del pokémon, si es un huevo no suena grito.
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

static void HidePokemonPic2(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    task->data[0]++;
}

static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y)
{
    u8 spriteId;

    spriteId = CreateMonSprite_FieldMove(species, otId, personality, 8 * x + 40, 8 * y + 40, FALSE);
    gSpriteTaskId = CreateTask(Task_ScriptShowMonPic, 80);

    gSprites[spriteId].hFlip = SPRITE_DIRECCION_VISTA;

    //Esta línea ajusta el sprite del pokémon 2 píxeles a la izquierda,
    //mejora la posición del sprite en el background por defecto.
    gSprites[spriteId].pos1.x -= 2;

#if SPRITE_SALTO == 1
    gSprites[spriteId].pos1.y -= 32;
    gSprites[spriteId].pos1.x += 48 * SPRITE_DIRECCION_SALTO;
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

#if SPRITE_SALTO == 1
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

#if SPRITE_SALTO == 1
        if (task->data[3] < 10)
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        else if (task->data[3] < 18)
        {
            gSprites[task->data[2]].pos1.y += 1;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 24)
        {
            gSprites[task->data[2]].pos1.y += 2;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 28)
        {
            gSprites[task->data[2]].pos1.y += 3;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 30)
        {
            gSprites[task->data[2]].pos1.y -= 3;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 32)
        {
            gSprites[task->data[2]].pos1.y -= 2;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 36)
        {
            gSprites[task->data[2]].pos1.y -= 1;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 40)
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        else if (task->data[3] < 44)
        {
            gSprites[task->data[2]].pos1.y += 1;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 46)
        {
            gSprites[task->data[2]].pos1.y += 2;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
        }
        else if (task->data[3] < 48)
        {
            gSprites[task->data[2]].pos1.y += 3;
            gSprites[task->data[2]].pos1.x -= 1 * SPRITE_DIRECCION_SALTO;
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

/**
 * Parámetro 'u8 color' en AddTextPrinterParameterized3 que
 * define el color que tendrá el texto a imprimir, usando los
 * colores de la paleta de la ventana de la siguiente manera:
 * static const u8 COLOR[3] = {FONDO, FUENTE, SOMBRA};
 * -----Colores PAL14 y PAL15-----
 * 0=transparente
 * 1=blanco         2=negro         3=gris
 * 4=rojo           5=rojo claro    6=verde
 * 7=verde claro    8=azul          9=azul claro
 * 
 * TEXT_COLOR_TRANSPARENT  = 0x0
 * TEXT_COLOR_WHITE        = 0x1
 * TEXT_COLOR_DARK_GREY    = 0x2
 * TEXT_COLOR_LIGHT_GREY   = 0x3
 * TEXT_COLOR_RED          = 0x4
 * TEXT_COLOR_LIGHT_RED    = 0x5
 * TEXT_COLOR_GREEN        = 0x6
 * TEXT_COLOR_LIGHT_GREEN  = 0x7
 * TEXT_COLOR_BLUE         = 0x8
 * TEXT_COLOR_LIGHT_BLUE   = 0x9
*/

//                                     fondo                    fuente                  sombra
static const u8 gColorTextoNegro[3] = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GREY,   TEXT_COLOR_LIGHT_GREY};
static const u8 gColorTextoGris[3]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_LIGHT_GREY,  TEXT_COLOR_DARK_GREY};

static const u8 gBlackTextColor[3]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GREY,   TEXT_COLOR_LIGHT_GREY};
static const u8 gBlueTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_BLUE,        TEXT_COLOR_LIGHT_GREY};
static const u8 gRedTextColor[3]    = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_RED,         TEXT_COLOR_LIGHT_GREY};
static const u8 gGrayTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_LIGHT_GREY,  TEXT_COLOR_DARK_GREY};

const u8 * gTextColorByNature[] = 
{
    gBlackTextColor,
    gRedTextColor,
    gBlueTextColor
};


#define PS_X        4
#define BS_X        54
#define EV_X        BS_X + 30   //84
#define IV_X        EV_X + 30   //108

#define PS_Y        3
#define ATAQUE_Y    PS_Y + 14
#define DEFENSA_Y   ATAQUE_Y + 14
#define ATQ_ESP_Y   DEFENSA_Y + 14
#define DEF_ESP_Y   ATQ_ESP_Y + 14
#define VELOCID_Y   DEF_ESP_Y + 14

/*
Cada letra usa:
10 píxeles de alto
6  píxeles de ancho
+4 por salto de línea
6  píxeles por espacio (? cuando cambian la fuente no coincide.

           BS   EV  IV
         |-> 9x6 = 54px 
              |-> 5x6 = 30px + 54px = 84px
                   |-> 4x6 = 24px + 84px = 108px
+- - - - - - - - - - - +
|PS       160  255  16 |  PS_Y
|ATAQUE   110   10  30 |  ATAQUE_Y
|DEFENSA   65   20  31 |  DEFENSA_Y
|ATA.ESP.  65    1  20 |  ATQ_ESP_Y
|DEF.ESP. 110   80  31 |  DEF_ESP_Y
|VELOCID.  30   11  13 |  VELOCID_Y
+- - - - - - - - - - - +
 PS_X     BS_X EV_X IV_X

*/

const u8 gText_Slash[]           = _("/");
const u8 gText_SaltoDeLinea[]    = _("\n");
const u8 gText_CensurarHuevos[]  = _("{CLEAR_TO 12}?{CLEAR_TO 42}?{CLEAR_TO 66}?\n");

const u8 gText_BsEvIv[]  = _("BS{CLEAR_TO 30}EV{CLEAR_TO 54}IV");

const u8 gText_Ps[]      = _("PS\n");
const u8 gText_Ataque[]  = _("ATAQUE\n");
const u8 gText_Defensa[] = _("DEFENSA\n");
const u8 gText_AtqEsp[]  = _("ATQ.ESP.\n");
const u8 gText_DefEsp[]  = _("DEF.ESP.\n");
const u8 gText_Velocid[] = _("VELOCID.");

const u8 gText_Total[]   = _("TOTAL");

const u8 gText_Tu[]      = _("Tu ");
const u8 gText_Es[]      = _(" es ");
const u8 gText_Feliz[]   = _("% felíz");

const u8 gText_AunFalta[]= _(" Aún falta...");
const u8 gText_Menos[]   = _("¡Menos de ");
const u8 gText_Pasos[]   = _(" pasos para eclosionar!");

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
    gTotalStatsEV = 0;
    gTotalStatsIV = 0;
    gTotalStatsBS = 0;

    //obtiene las estadísticas del pokémon

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

    //suma los stat para los totales
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

    //imprime las ventanas con los textos en pantalla
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
}

/**
 * Datos de encabezado - ventana 0
 * Imprime el número del pokémon actual,
 * BS EV IV y nickname del pokémon actual.
*/

static void PrintWindow0(struct Pokemon *mon)
{
    u8 temp = 0;

    //imprimir numero pokémon actual/total equipo - ventana 0
    temp = gActualMon + 1;                              //temp = número actual del pokémon.
    ConvertIntToDecimalStringN(gStringVar4, temp, STR_CONV_MODE_LEFT_ALIGN, 1);//convierte temp en un número para imprimir en pantalla, se guarda en StringVar4 (gStringVar4 = "gActualMon")
    StringAppend(gStringVar4, gText_Slash);             //concatena lo que tiene gStringVar4 con un '/' (gStringVar4 = "gActualMon / ")
    temp = gPlayerPartyCount;                           //temp = número total del equipo pokémon.
    ConvertIntToDecimalStringN(gStringVar1, temp, STR_CONV_MODE_LEFT_ALIGN, 1);//convierte temp en un número para imprimir en pantalla, se guarda en gStringVar1
    StringAppend(gStringVar4, gStringVar1);             //Concatena gStringVar4 con gStringVar1 (gStringVar4 = "gActualMon / gPlayerPartyCount")
    AddTextPrinterParameterized3(WIN_NICK_POKEMON, 2, 2, 2, gGrayTextColor, 0, gStringVar4);//imprime en la ventana el texto concatenado en gStringVar4.

    //imprime BS EV IV - ventana 0
    AddTextPrinterParameterized3(WIN_NICK_POKEMON, 2, 0x44, 2, gGrayTextColor, 0, gText_BsEvIv);

    //imprime nickname del pokémon actual - ventana 0
    GetMonNickname(mon, gStringVar4);                   //obtiene el nickname del pokémon y lo guarda en gStringVar4
    AddTextPrinterParameterized3(WIN_NICK_POKEMON, 2, 0x90, 2, gGrayTextColor, 0, gStringVar4);
}

/**
 * Estadísticas - ventana 1
 * Si es un pokémon, imprime las estadísticas y hace otras operaciones.
 * Si es un huevo, solo imprime ? ? ?.
*/

static void PrintWindow1(u8 nature, u8 isEgg)
{
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, PS_Y,      gTextColorByNature[GetColorByNature(nature, STAT_HP)],    0, gText_Ps);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, ATAQUE_Y,  gTextColorByNature[GetColorByNature(nature, STAT_ATK)],   0, gText_Ataque);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, DEFENSA_Y, gTextColorByNature[GetColorByNature(nature, STAT_DEF)],   0, gText_Defensa);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, ATQ_ESP_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPATK)], 0, gText_AtqEsp);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, DEF_ESP_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPDEF)], 0, gText_DefEsp);
    AddTextPrinterParameterized3(WIN_STATS, 2, PS_X, VELOCID_Y, gTextColorByNature[GetColorByNature(nature, STAT_SPEED)], 0, gText_Velocid);

    //si no es un huevo, imprime las estadísticas. - ventana 1
    if (!isEgg)
    {
        //gStringVar4[0] = EOS;//limpiamos gStringVar4
        
        PrintStat(nature, STAT_HP);
        PrintStat(nature, STAT_ATK);
        PrintStat(nature, STAT_DEF);
        PrintStat(nature, STAT_SPATK);
        PrintStat(nature, STAT_SPDEF);
        PrintStat(nature, STAT_SPEED);

    //si es un huevo. Imprime ? ? ? - ventana 1
    }else{
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, PS_Y,      gBlackTextColor, 0, gText_CensurarHuevos);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATAQUE_Y,  gBlackTextColor, 0, gText_CensurarHuevos);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEFENSA_Y, gBlackTextColor, 0, gText_CensurarHuevos);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATQ_ESP_Y, gBlackTextColor, 0, gText_CensurarHuevos);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEF_ESP_Y, gBlackTextColor, 0, gText_CensurarHuevos);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, VELOCID_Y, gBlackTextColor, 0, gText_CensurarHuevos);
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
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, PS_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, PS_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, PS_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_ATK:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATAQUE_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, ATAQUE_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, ATAQUE_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_DEF:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEFENSA_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, DEFENSA_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, DEFENSA_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPATK:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, ATQ_ESP_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, ATQ_ESP_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, ATQ_ESP_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPDEF:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, DEF_ESP_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, DEF_ESP_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, DEF_ESP_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    case STAT_SPEED:
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X, VELOCID_Y, gTextColorByNature[color], 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_STATS, 2, EV_X, VELOCID_Y, gTextColorByNature[color], 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_STATS, 2, IV_X, VELOCID_Y, gTextColorByNature[color], 0, gStringVar3);
        break;
    default:
        break;
    }
}

/**
 * Ventana 2
 * porcentaje de felicidad
 * ciclos del huevo
*/

static void PrintWindow2(u16 species, u8 isEgg, u8 friendship)
{
    u16 temp = 0;

    //si no es un huevo, imprime el total y el porcentaje de felicidad.
    if(!isEgg)
    {
        //agrega el texto total y la suma de los totales
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 14, 4, gBlackTextColor, 0, gText_Total);

        ConvertIntToDecimalStringN(gStringVar1, gTotalStatsBS, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar2, gTotalStatsEV, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar3, gTotalStatsIV, STR_CONV_MODE_RIGHT_ALIGN, 3);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, BS_X + 8, 4, gBlackTextColor, 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, EV_X + 8, 4, gBlackTextColor, 0, gStringVar2);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, IV_X + 2, 4, gBlackTextColor, 0, gStringVar3);

        //agrega 'Tu {EspeciePokémon} es x % feliz.'
        StringCopy(gStringVar4, gText_Tu);
        GetSpeciesName(gStringVar1, species);
        StringAppend(gStringVar4, gStringVar1);
        StringAppend(gStringVar4, gText_Es);
        temp = (friendship * 100) / 0xFF;
        
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar4, gStringVar2);
        
        StringAppend(gStringVar4, gText_Feliz);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 14, 18, gBlackTextColor, 0, gStringVar4);
        
    }else
    {
        //en caso contrario (si es un huevo)
        //agrega 'Aún falta...'
        StringCopy(gStringVar4, gText_AunFalta);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 2, 4, gBlackTextColor, 0, gStringVar4);

        //agrega' ¡Menos de x pasos para eclosionar!'
        StringCopy(gStringVar4, gText_Menos);
        temp = (friendship + 1) * 0xFF;
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, GetDigits(temp));
        StringAppend(gStringVar4, gStringVar2);
        StringAppend(gStringVar4, gText_Pasos);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 2, 18, gBlackTextColor, 0, gStringVar4);
    }
}

/**
 * Función que devuelve la cantidad de dígitos
 * de un número menor a 65.535
*/
static u8 GetDigits(u16 num)
{
    if (num < 10)
        return 1;
    else if (num < 100)
        return 2;
    else if (num < 1000)
        return 3;
    else if (num < 10000)
        return 4;
    else
        return 5;
}

/**
 * Esta función devuelve un número,
 * de acuerdo a la naturaleza.
 * 0 = neutro
 * 1 = rojo
 * 2 = azul
 * 
 * This return a value
 * used on color stat
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
