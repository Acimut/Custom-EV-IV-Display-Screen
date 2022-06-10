/*

\-------------------------------------------------------------------------/
|\-----------------------------------------------------------------------/|
||\---------------------------------------------------------------------/||
|||         Custom EV - IV Display Screen (Terry's Edition)             |||
|||                                                                     |||
|||Esta es una Modificación del Custom EV - IV Display Screen de Acimut |||
|||la cual incluye algunos cambios esteticos que la versión original    |||
|||no posee.                                                            |||
|||                                                                     |||
|||Caracteristicas:                                                     |||
|||                                                                     |||
|||Versión de Acimut:                                                   |||
|||-Ve aqui(https://whackahack.com/foro/threads/                        |||
||| fr-rf-custom-ev-iv-display-screen.65588/)                           |||
||| para que las leas y lo apoyes xd.                                   |||
|||                                                                     |||
|||Versión de Tio Terry (Mia):                                          |||
|||-Reorganización de textos.                                           |||
|||-Naturalezas con nombre e indicadores con su respectivo color.       |||
|||-Nivel, Genero y Tipos del Pokémon.                                  |||
|||-Tipo de Poder Oculto segun la suma de Ivs.                          |||
|||-Pokeballs del equipo.                                               |||
||| (la Pokeball del Pokémon seleccionado se abrira).                   |||
|||-Fondo rediseñado.                                                   |||
||| (Configurado en 2 colores segun el genero del prota)                |||
|||-El Nombre y Nivel del Pokémon se tornaran amarillos si el Pokémon   |||
||| es Shiny.                                                           |||
|||-Compatible con Fire Red, Rojo Fuego y Emerald.                      |||
|||-Conserva el olor a Limón.                                           |||
|||                                                                     |||
|||No es un aporte a nivel FUAAAAA QUE WAPO Y KE LA CANCION, pero       |||
|||queria compartirlo y provar a ver que tal se me da v:                |||
|||                                                                     |||
|||                                                                     |||
|||                                                                     |||
|||                                                                     |||
|||                                                                     |||
|||                                                                     |||
|||Creditos:                                                            |||
|||Acimut - Repositorio base, paciencia y ayuda con la elaboración de   |||
|||         varias funciones.                                           |||
|||Tio_Terry - Modificaciónes esteticas.                                |||
|||ACE10 - Ideas y sugerencias.                                         |||
|||Tohru - Más ideas y sugerencias.                                     |||
|||Cefer - Más y más ideas y sugerencias.                               |||
|||                                                                     |||
|||                                                                     |||
|||                                                                     |||
|||                                                                     |||
|||Pido por favor que si lo usas, almenos intenta compilarlo xd,        |||
|||por ti mismo, si no puedes pide ayuda, todos en este lugar son       |||
|||amables y te la pueden brindar...                                    |||
|||                                                                     |||
|||Subido a: https://whackahack.com/foro/threads                        |||
|||                                                                     |||
|||Ver. 1.1 2022.                                                       |||
||/---------------------------------------------------------------------\||
|/-----------------------------------------------------------------------\|
/-------------------------------------------------------------------------\

*/


#include "include/global.h"
#include "include/menu.h"
#include "include/task.h"
#include "include/constants/songs.h"
#include "include/bg.h"
#include "include/malloc.h"
#include "include/palette.h"
#include "include/sprite.h"
#include "include/window.h"
#include "include/string_util.h"
#include "include/sprite.h"
#include "include/pokeball.h"
#include "include/battle_anim.h"
#include "include/party_menu.h"
#include "include/scanline_effect.h"
#include "include/gpu_regs.h"
#include "include/new_menu_helpers.h"
#include "include/field_effect.h"
#include "include/text_window.h"
#include "include/overworld.h"
#include "include/field_weather.h"
#include "include/trainer_pokemon_sprites.h"
#include "main_eviv.h"

//#define ESP //comment this to use the english text.


//escribe RF si usas Pokémon Edición Rojo Fuego.
//        EM si usas Pokémon Emerald Edition.
//        ES si usas Pokémon Edición Esmeralda.
//comenta si no usas ninguna de las anteriores.
//write RF if use Pokémon Edición Rojo Fuego.
//      EM if use Pokémon Emerald Edition.
//      ES if use Pokémon Edición Esmeralda.
//coment if you don't use any of the above.

//#define ES

// 1 = ACTIVADO, 0 = DESACTIVADO. Activa o desactiva el salto de sprite.
// 1 = ON, 0 = OFF.  Activates or deactivates the sprite jump.
#define SPRITE_JUMP             1

// 1 = DE DERECHA A IZQUIERDA, 0 = EN EL CENTRO -1 = DE IZQUIERDA A DERECHA.
// 1 = FROM RIGHT TO LEFT, 0 = IN THE CENTER -1 = FROM LEFT TO RIGHT.
#define SPRITE_JUMP_DIRECTION   1

// 1 = A LA DERECHA, 0 = A LA IZQUIERDA.
// 1 = RIGHT, 0 = LEFT.
#define SPRITE_VIEW_DIRECTION   0


//coordenada x del sprite pokémon, se mide en tiles de x8 pixeles
//x coordinate of the pokémon sprite, measured in tiles of x8 pixels
#define PICMON_X    20

//coordenada y del sprite pokémon, se mide en tiles de x8 pixeles
//y coordinate of the pokémon sprite, measured in tiles of x8 pixels
#define PICMON_Y     6

static void Task_EvIvInit(u8);
static u8 EvIvLoadGfx(void);
static void EvIvVblankHandler(void);
static void Task_WaitForExit(u8);
static void Task_EvIvReturnToOverworld(u8);
static void ShowSprite(struct Pokemon *mon);
static void EvIvPrintText(struct Pokemon *mon);
static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y);
static void Task_ScriptShowMonPic(u8 taskId);
static void HidePokemonPic2(u8 taskId);

static void PrintStat(u8 nature, u8 stat);
static u8 GetDigits(u16 num);
static u8 GetColorByNature(u8 nature, u8 stat);
static void LoadBalls(void);

const u32 gBgEvIvGfx[] = INCBIN_U32("graphics/bgEvIv.4bpp.lz");
const u32 gBgEvIvTilemap[] = INCBIN_U32("graphics/bgEvIv.bin.lz");
const u16 gBgEvIvPal_B[] = INCBIN_U16("graphics/bgEvIv.gbapal");
const u16 gBgEvIvPal_G[] = INCBIN_U16("graphics/bgEvIv_G.gbapal");

//                      |0 Transparent  |1 White  |2 Gray   |3 Light grey |4 Red    |5 Dark Red    |6 Blue   |7 Dark Blue   |8 Yellow   |9 Dark Yellow
//                      |0 Trabsparente |1 Blaco  |2 Gris   |3 Gris Claro |4 Rojo   |5 Rojo Oscuro |6 Azul   |7 Azul Oscuro |8 Amarillo |9 Amarillo Oscuro
const u16 gPalText[] = {0x542E,         0x7FFF,   0x318C,   0x675A,       0x319E,   0x1CFE,        0x7EC4,   0x6600,        0x239E,     0x0611};


enum
{
    WIN_POKEMON_NAME,
    WIN_STATS,
    WIN_BOTTOM_BOX,
    WIN_HIDDEN_MOVE,
    WIN_TYPE
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
#define WINDOW0_WIDTH   30
#define WINDOW0_HEIGTH  5

//window 1 = stats
#define WINDOW1_WIDTH   20
#define WINDOW1_HEIGTH  11
#define WINDOW1_BASEBLOCK  (WINDOW0_WIDTH * WINDOW0_HEIGTH)

//window 2 = text in the bottom bar 
//window 2 = texto en la barra inferior
#define WINDOW2_WIDTH   30
#define WINDOW2_HEIGTH  5
#define WINDOW2_BASEBLOCK WINDOW1_WIDTH * WINDOW1_HEIGTH + WINDOW1_BASEBLOCK

//window 3 = Hidden Power
//window 3 = Poder Oculto
#define WINDOW3_WIDTH   5
#define WINDOW3_HEIGTH  2
#define WINDOW3_BASEBLOCK WINDOW2_WIDTH * WINDOW2_HEIGTH + WINDOW2_BASEBLOCK

//window 4 = types
//window 4 = tipos
#define WINDOW4_WIDTH   9
#define WINDOW4_HEIGTH  3
#define WINDOW4_BASEBLOCK WINDOW3_WIDTH * WINDOW3_HEIGTH + WINDOW3_BASEBLOCK

static const struct WindowTemplate windows_templates[] = {
    {//window 0 = pokémon name
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 0,
        .width = WINDOW0_WIDTH,
        .height = WINDOW0_HEIGTH,
        .paletteNum = 15,
        .baseBlock = 0x000
    },{//window 1 = stats
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 5,
        .width = WINDOW1_WIDTH,
        .height = WINDOW1_HEIGTH,
        .paletteNum = 15,
        .baseBlock = WINDOW1_BASEBLOCK
    },{//window 2 = text in the bottom bar 
       //window 2 = texto en la barra inferior
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 16,
        .width = WINDOW2_WIDTH,
        .height = WINDOW2_HEIGTH,
        .paletteNum = 15,
        .baseBlock = WINDOW2_BASEBLOCK
    },{//window 3 = Hidden Power
       //window 3 = Poder Oculto
        .bg = 0,
        .tilemapLeft = 15,
        .tilemapTop = 16,
        .width = WINDOW3_WIDTH,
        .height = WINDOW3_HEIGTH,
        .paletteNum = 12,
        .baseBlock = WINDOW3_BASEBLOCK
    },{//window 4 = types
       //window 4 = tipos
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 2,
        .width = WINDOW4_WIDTH,
        .height = WINDOW4_HEIGTH,
        .paletteNum = 12,
        .baseBlock = WINDOW4_BASEBLOCK
    },DUMMY_WIN_TEMPLATE
};

struct EvIv
{
    u8 state;
    u8 gfxStep;
    u8 callbackStep;
    u8 currentMon;
    u8 spriteTaskId;
    u8 stats_ev[NUM_STATS];
    u8 stats_iv[NUM_STATS];
    u8 stats_bs[NUM_STATS];
    u8 ballSpriteId[PARTY_SIZE];
    u8 level;
    u8 nat;
    u8 hidenmove;
    u8 varshiny;

    u16 totalStatsEV;
    u16 totalStatsBS;
    u16 tilemapBuffer[0x400];  
    u16 gen;
    u16 typ1;
    u16 typ2;
    
};

extern struct EvIv *gEvIv;

#define gState              gEvIv->state
#define gGfxStep            gEvIv->gfxStep
#define gCallbackStep       gEvIv->callbackStep
#define gCurrentMon         gEvIv->currentMon
#define gSpriteTaskId       gEvIv->spriteTaskId
#define gStats_ev           gEvIv->stats_ev
#define gStats_iv           gEvIv->stats_iv
#define gStats_bs           gEvIv->stats_bs
#define gBallSpriteId       gEvIv->ballSpriteId
#define gLevelMon           gEvIv->level
#define gNature             gEvIv->nat
#define gHiddenMove         gEvIv->hidenmove
#define gVarShiny           gEvIv->varshiny

#define gTotalStatsEV       gEvIv->totalStatsEV
#define gTotalStatsBS       gEvIv->totalStatsBS
#define gGen                gEvIv->gen
#define gType1              gEvIv->typ1
#define gType2              gEvIv->typ2


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
    gCurrentMon = 0;
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
        LoadBalls();
        ShowSprite(&gPlayerParty[gCurrentMon]);
        EvIvPrintText(&gPlayerParty[gCurrentMon]);
        gSprites[gBallSpriteId[gCurrentMon]].animNum = 4;
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
        if (gPlayerPartyCount > 1)
        {
            if (JOY_REPT(DPAD_RIGHT))
            {
                gSprites[gBallSpriteId[gCurrentMon]].animNum = 3;

                if (gCurrentMon == (gPlayerPartyCount - 1))
                    gCurrentMon = 0;
                else
                    gCurrentMon++;

                gSprites[gBallSpriteId[gCurrentMon]].animNum = 4;

                HidePokemonPic2(gSpriteTaskId);
                EvIvPrintText(&gPlayerParty[gCurrentMon]);
                ShowSprite(&gPlayerParty[gCurrentMon]);
            }
            else  if (JOY_REPT(DPAD_LEFT))
            {
                gSprites[gBallSpriteId[gCurrentMon]].animNum = 3;

                if (gCurrentMon == 0)
                    gCurrentMon = (gPlayerPartyCount - 1);
                else
                    gCurrentMon--;

                gSprites[gBallSpriteId[gCurrentMon]].animNum = 4;

                HidePokemonPic2(gSpriteTaskId);
                EvIvPrintText(&gPlayerParty[gCurrentMon]);
                ShowSprite(&gPlayerParty[gCurrentMon]);
            }

            else if (JOY_REPT(DPAD_UP) || JOY_REPT(DPAD_DOWN))
            {
                if (gCurrentMon > 2)
                {
                    gSprites[gBallSpriteId[gCurrentMon]].animNum = 3;
                    gCurrentMon -= 3;
                    gSprites[gBallSpriteId[gCurrentMon]].animNum = 4;
                    HidePokemonPic2(gSpriteTaskId);
                    EvIvPrintText(&gPlayerParty[gCurrentMon]);
                    ShowSprite(&gPlayerParty[gCurrentMon]);
                }
                else if (gPlayerPartyCount > (gCurrentMon + 3))
                {
                    gSprites[gBallSpriteId[gCurrentMon]].animNum = 3;
                    gCurrentMon += 3;
                    gSprites[gBallSpriteId[gCurrentMon]].animNum = 4;
                    HidePokemonPic2(gSpriteTaskId);
                    EvIvPrintText(&gPlayerParty[gCurrentMon]);
                    ShowSprite(&gPlayerParty[gCurrentMon]);
                }
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
        CreateMoveTypeIcons();
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
        if (gSaveBlock2Ptr->playerGender == MALE)
        {   
            LoadPalette(gBgEvIvPal_B, 0, 0x20);
        }
        else
        {
            LoadPalette(gBgEvIvPal_G, 0, 0x20);
        }

#ifdef FIRERED
//FIRERED
        //LoadPalette(stdpal_get(0), 0xf0, 0x20);
        LoadPalette(gPalText, 0xF0, 0x14);

        #ifdef RF
        //Palette of Type Icons in Rojo Fuego(Change 0x08XXXXXX if necessary)
        //Paleta de Iconos de Tipos en Rojo Fuego(Cambiar 0x08XXXXXX si es necesario)
        const u16*gPalType=0x08E95CF0;
        #else
        //Palette of Type Icons in Fire Red(Change 0x08XXXXXX if necessary)
        //Paleta de Iconos de Tipos en Fire Red(Cambiar 0x08XXXXXX si es necesario)
        const u16*gPalType=0x08E95DBC;
        #endif

        LoadPalette(gPalType, 0xE0, 0x20);

#else
//EMERALD
        //LoadPalette(GetTextWindowPalette(0), 0xf0, 0x20);
        LoadPalette(gPalText, 0xF0, 0x14);

        #ifdef ES
        //Palette of Type Icons in Esmeralda(Change 0x08XXXXXX if necessary)
        //Paleta de Iconos de Tipos en Esmeralda(Cambiar 0x08XXXXXX si es necesario)
        const u32*gPalType=0x08D97B84;
        #else
        //Palette of Type Icons in Emerald(Change 0x08XXXXXX if necessary)
        //Paleta de Iconos de Tipos en Emerald(Cambiar 0x08XXXXXX si es necesario)
        const u32*gPalType=0x08D97B84;
        #endif

        LoadCompressedPalette(gPalType, 0xC0, 0x60);

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

    //Print the sprite of the pokémon, if it is an egg it does not sound a scream.
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


/**
 * 2022/03/21
 * ::ACIMUT::
 * 
 * 1. Obtener los tipos de balls de todo el equipo.
 * 2. Cargar los gráficos en la vram.
 * 3. Crear un sprite para cada ball y guardar el spriteId.
 * 4. Crear una función para cambiar la animación de la ball.
 * 
*/



#define BALL_X      188
#define BALL_Y      21
#define BALL_SIZE   16

static const u8 ball_cords[][2] = 
{
    {BALL_X, BALL_Y},
    {BALL_X + BALL_SIZE, BALL_Y},
    {BALL_X + (BALL_SIZE * 2), BALL_Y},
    {BALL_X, BALL_Y + BALL_SIZE},
    {BALL_X + BALL_SIZE, BALL_Y + BALL_SIZE},
    {BALL_X + (BALL_SIZE * 2), BALL_Y + BALL_SIZE}
};

static void LoadBalls(void)
{
    u8 i;
    u16 ballItemId;
    u8 ballId;
    u8 isEgg ;

    for (i = 0; i < gPlayerPartyCount; i++)
    {
        isEgg  = GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, NULL);
        if (!isEgg)
            ballItemId = GetMonData(&gPlayerParty[i], MON_DATA_POKEBALL, NULL);
        else
            ballItemId = 0;

        //Load the graphics
        //Cargamos los gráficos
        ballId = ItemIdToBallId(ballItemId);
        LoadBallGfx(ballId);

        //Save the spriteId
        //Guardamos los spriteId
        gBallSpriteId[i] = CreateSprite(&gBallSpriteTemplates[ballId], ball_cords[i][0], ball_cords[i][1], 0);
        gSprites[gBallSpriteId[i]].callback = SpriteCallbackDummy;
        gSprites[gBallSpriteId[i]].oam.priority = 0;

        //gSprites[gBallSpriteId[i]].invisible = TRUE;
    }
}

/**
 * Esta función destruye un sprite xd
 * unused.
*/
void EvIv_DestroySprite(u8 spriteId)
{
    DestroySpriteAndFreeResources(&gSprites[spriteId]);
}

static void HidePokemonPic2(u8 taskId)
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

    //Adjust the pokémon sprite 2 pixels to the left
    //Ajusta el sprite del pokémon 2 píxeles a la izquierda
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
        if (task->data[3] < 10)
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        else if (task->data[3] < 18)
        {
            gSprites[task->data[2]].y += 1;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 24)
        {
            gSprites[task->data[2]].y += 2;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 28)
        {
            gSprites[task->data[2]].y += 3;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 30)
        {
            gSprites[task->data[2]].y -= 3;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 32)
        {
            gSprites[task->data[2]].y -= 2;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 36)
        {
            gSprites[task->data[2]].y -= 1;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 40)
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        else if (task->data[3] < 44)
        {
            gSprites[task->data[2]].y += 1;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 46)
        {
            gSprites[task->data[2]].y += 2;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
        }
        else if (task->data[3] < 48)
        {
            gSprites[task->data[2]].y += 3;
            gSprites[task->data[2]].x -= 1 * SPRITE_JUMP_DIRECTION;
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
|PS       160  255  16 |  HP_Y
|ATAQUE   110   10  30 |  ATK_Y
|DEFENSA   65   20  31 |  DEF_Y
|ATA.ESP.  65    1  20 |  SPATK_Y
|DEF.ESP. 110   80  31 |  SPDEF_Y
|VELOCID.  30   11  13 |  SPEED_Y
+- - - - - - - - - - - +
|        |    |    |
 HP_X     BS_X EV_X IV_X

*/

#define HP_X        4
#define BS_X        60
#define EV_X        BS_X + 30
#define IV_X        EV_X + 36

#define HP_Y        1
#define ATK_Y       HP_Y + 14
#define DEF_Y       ATK_Y + 14
#define SPATK_Y     DEF_Y + 14
#define SPDEF_Y     SPATK_Y + 14
#define SPEED_Y     SPDEF_Y + 14



//                                     resaltado                color fuente            sombra
//                                     highlight                font color              shadow
static const u8 gBlackTextColor[3]  = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_DARK_GRAY,   TEXT_COLOR_LIGHT_GRAY};
static const u8 gBlueTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  6,                      7};
static const u8 gDarkBlueColor[3]   = {TEXT_COLOR_TRANSPARENT,  7,                      TEXT_COLOR_DARK_GRAY};
static const u8 gRedTextColor[3]    = {TEXT_COLOR_TRANSPARENT,  4,                      5};
static const u8 gDarkRedColor[3]    = {TEXT_COLOR_TRANSPARENT,  5,                      TEXT_COLOR_DARK_GRAY};
static const u8 gGrayTextColor[3]   = {TEXT_COLOR_TRANSPARENT,  TEXT_COLOR_LIGHT_GRAY,  TEXT_COLOR_DARK_GRAY};
static const u8 gShinyTextColor[3]  = {TEXT_COLOR_TRANSPARENT,  8,                      9};

static const u8 *const gTextColorByNature[] = 
{
    gBlackTextColor,
    gRedTextColor,
    gBlueTextColor
};

//Texts to display according to defined language (capitalize if necessary)
//Textos a mostrar segun idioma definido (capitalizar si es necesario)
const u8 gText_CensorEgg[]  = _("{CLEAR_TO 12}-{CLEAR_TO 42}-{CLEAR_TO 72}-");
const u8 gText_BsEvIv[] = _(" Bs{CLEAR_TO 30} Ev{CLEAR_TO 60} Iv");
const u8 gText_Total[]  = _("Total:");

const u8 gArrowU[]        = _("{0x79}");
const u8 gArrowD[]        = _("{0x7A}");

const u8 gMale[]            = _("♂");
const u8 gFemale[]          = _("♀");
const u8 gNoneG[]           = _("");

//Si quieres hacer que aparezcan en Mayusculas deberas escribirlos xd
#ifdef ESP
const u8 gHARDY[]        = _("Fuerte");
const u8 gLONELY[]       = _("Huraña");
const u8 gBRAVE[]        = _("Audaz");
const u8 gADAMANT[]      = _("Firme");
const u8 gNAUGHTY[]      = _("Pícara");
const u8 gBOLD[]         = _("Osada");
const u8 gDOCILE[]       = _("Dócil");
const u8 gRELAXED[]      = _("Plácida");
const u8 gIMPISH[]       = _("Agitada");
const u8 gLAX[]          = _("Floja");
const u8 gTIMID[]        = _("Miedosa");
const u8 gHASTY[]        = _("Activa");
const u8 gSERIOUS[]      = _("Seria");
const u8 gJOLLY[]        = _("Alegre");
const u8 gNAIVE[]        = _("Ingenua");
const u8 gMODEST[]       = _("Modesta");
const u8 gMILD[]         = _("Afable");
const u8 gQUIET[]        = _("Mansa");
const u8 gBASHFUL[]      = _("Tímida");
const u8 gRASH[]         = _("Alocada");
const u8 gCALM[]         = _("Serena");
const u8 gGENTLE[]       = _("Amable");
const u8 gSASSY[]        = _("Grosera");
const u8 gCAREFUL[]      = _("Cauta");
const u8 gQUIRKY[]       = _("Rara");

const u8 gText_Hp[]     = _("Ps");
const u8 gText_Atq[]    = _("Ataque");
const u8 gText_Def[]    = _("Defensa");
const u8 gText_SpAtk[]  = _("At. Esp.");
const u8 gText_SpDef[]  = _("De. Esp.");
const u8 gText_Speed[]  = _("Velocid.");

const u8 gText_Your[]   = _("Tu ");
const u8 gText_Is[]     = _(" es ");
const u8 gText_Happy[]  = _("% feliz.");
const u8 gLevel[]       = _("Nv. ");

const u8 gStepts[]      = _("Pasos restantes:");
const u8 gText_Less_Than[]  = _("¡Menos de ");
const u8 gText_Steps_to_hatching[]  = _(" pasos!");
#else
// ENG
//If you want to make them appear in uppercase you should write them xd
const u8 gHARDY[]        = _("Hardy");
const u8 gLONELY[]       = _("Lonely");
const u8 gBRAVE[]        = _("Brave");
const u8 gADAMANT[]      = _("Adamant");
const u8 gNAUGHTY[]      = _("Naughty");
const u8 gBOLD[]         = _("Bold");
const u8 gDOCILE[]       = _("Docile");
const u8 gRELAXED[]      = _("Relaxed");
const u8 gIMPISH[]       = _("Impish");
const u8 gLAX[]          = _("Lax");
const u8 gTIMID[]        = _("Timid");
const u8 gHASTY[]        = _("Hasty");
const u8 gSERIOUS[]      = _("Serious");
const u8 gJOLLY[]        = _("Jolly");
const u8 gNAIVE[]        = _("Naive");
const u8 gMODEST[]       = _("Modest");
const u8 gMILD[]         = _("Mild");
const u8 gQUIET[]        = _("Quiet");
const u8 gBASHFUL[]      = _("Bashful");
const u8 gRASH[]         = _("Rash");
const u8 gCALM[]         = _("Calm");
const u8 gGENTLE[]       = _("Gentle");
const u8 gSASSY[]        = _("Sassy");
const u8 gCAREFUL[]      = _("Careful");
const u8 gQUIRKY[]       = _("Quirky");

const u8 gText_Hp[]     = _("Hp");
const u8 gText_Atq[]    = _("Attack");
const u8 gText_Def[]    = _("Defense");
const u8 gText_SpAtk[]  = _("Sp. Atk.");
const u8 gText_SpDef[]  = _("Sp. Def.");
const u8 gText_Speed[]  = _("Speed");

const u8 gText_Your[]   = _("Your ");
const u8 gText_Is[]     = _(" is ");
const u8 gText_Happy[]  = _("% happy.");
const u8 gLevel[]  = _("Lv. ");

const u8 gStepts[]      = _("Steps to hatch:");
const u8 gText_Less_Than[]  = _("Less than ");
const u8 gText_Steps_to_hatching[]  = _(" steps!");
#endif

static void PrintWindow0(struct Pokemon *mon);
static void PrintWindow1(u8 nature, u8 isEgg);
static void PrintWindow2(u16 species, u8 isEgg, u8 friendship);
static void PrintWindow3(void);
static void PrintWindow4(void);

static void EvIvPrintText(struct Pokemon *mon)
{
    u16 species   = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 nature     = GetNature(mon);
    u8 isEgg      = GetMonData(mon, MON_DATA_IS_EGG, NULL);
    u8 friendship = GetMonData(mon, MON_DATA_FRIENDSHIP, NULL);



    //reset the totals.
    //reinicia los totales.
    gTotalStatsEV = 0;
    gTotalStatsBS = 0;

    //get pokémon stats
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

    //performs the sum of the totals
    //realiza la suma de los totales
    for (int i = 0; i < NUM_STATS; i++)
    {
        gTotalStatsEV += gStats_ev[i];
        gTotalStatsBS += gStats_bs[i];
    }

    //Get more data from the Pokémon
    //Obtiene mas datos del Pokémon
    gLevelMon = GetMonData(mon,MON_DATA_LEVEL,NULL);
    gGen      = GetMonGender(mon);

    gType1 = gBaseStatsPtr[species].type1;
    gType2 = gBaseStatsPtr[species].type2;

    if(!IsMonShiny(mon)){
        gVarShiny = 0;
    }else{
        gVarShiny = 1;
    }


    FillWindowPixelBuffer(0, 0);
    FillWindowPixelBuffer(1, 0);
    FillWindowPixelBuffer(2, 0);
    FillWindowPixelBuffer(3, 0);
    FillWindowPixelBuffer(4, 0);

    PrintWindow0(mon);
    PrintWindow1(nature, isEgg);
    PrintWindow2(species, isEgg, friendship);
    PrintWindow3();
    PrintWindow4();

    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
    PutWindowTilemap(3);
    PutWindowTilemap(4);

}

static void PrintWindow0(struct Pokemon *mon)
{
    gNature = GetNature(mon);

    AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, BS_X, 22, gGrayTextColor, 0, gText_BsEvIv);
    GetMonNickname(mon, gStringVar4);

    if (!IsMonShiny(mon)){
        AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 0, 2, gGrayTextColor, 0, gStringVar4);
    }else{
        AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 0, 2, gShinyTextColor, 0, gStringVar4);
    }
    
    

}

static void PrintWindow1(u8 nature, u8 isEgg)
{
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, HP_Y,    gBlackTextColor, 0, gText_Hp);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, ATK_Y,   gBlackTextColor, 0, gText_Atq);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, DEF_Y,   gBlackTextColor, 0, gText_Def);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, SPATK_Y, gBlackTextColor, 0, gText_SpAtk);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, SPDEF_Y, gBlackTextColor, 0, gText_SpDef);
    AddTextPrinterParameterized3(WIN_STATS, 2, HP_X, SPEED_Y, gBlackTextColor, 0, gText_Speed);

    if (!isEgg)
    {
        PrintStat(nature, STAT_HP);
        PrintStat(nature, STAT_ATK);
        PrintStat(nature, STAT_DEF);
        PrintStat(nature, STAT_SPATK);
        PrintStat(nature, STAT_SPDEF);
        PrintStat(nature, STAT_SPEED);

        
    if(gNature==NATURE_HARDY){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gHARDY);

    }else if(gNature==NATURE_LONELY){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gLONELY);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_BRAVE){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gBRAVE);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_ADAMANT){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gADAMANT);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_NAUGHTY){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gNAUGHTY);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_BOLD){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gBOLD);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_DOCILE){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gDOCILE);
    }

    else if(gNature==NATURE_RELAXED){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gRELAXED);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_IMPISH){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gIMPISH);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_LAX){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gLAX);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_TIMID){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gTIMID);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_HASTY){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gHASTY);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_SERIOUS){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gSERIOUS);
    }

    else if(gNature==NATURE_JOLLY){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gJOLLY);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_NAIVE){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gNAIVE);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_MODEST){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gMODEST);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_MILD){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gMILD);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_QUIET){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gQUIET);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_BASHFUL){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gBASHFUL);
    }

    else if(gNature==NATURE_RASH){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gRASH);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_CALM){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gCALM);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, ATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_GENTLE){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gGENTLE);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, DEF_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_SASSY){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gSASSY);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPEED_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_CAREFUL){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gCAREFUL);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPDEF_Y, gRedTextColor, 0, gArrowU);
        AddTextPrinterParameterized3(WIN_STATS, 2, BS_X-14, SPATK_Y, gBlueTextColor, 0, gArrowD);
    }

    else if(gNature==NATURE_QUIRKY){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 17, gGrayTextColor, 0, gQUIRKY);
    }

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
    u16 temp = 0;
    
    if(!isEgg){

    BlitMoveInfoIcon(WIN_TYPE, gType1 + 1, 1, 4);
    if (gType1 != gType2){
        BlitMoveInfoIcon(WIN_TYPE, gType2 + 1, 35, 4);
    }
            
    gHiddenMove = ((gStats_iv[STAT_HP] & 1)) |
                  ((gStats_iv[STAT_ATK] & 1) << 1) |
                  ((gStats_iv[STAT_DEF] & 1) << 2) |
                  ((gStats_iv[STAT_SPEED] & 1) << 3) |
                  ((gStats_iv[STAT_SPATK] & 1) << 4) |
                  ((gStats_iv[STAT_SPDEF] & 1) << 5);

    gHiddenMove = ((15 * gHiddenMove) / 63) + 1;
    if (gHiddenMove >= TYPE_MYSTERY){
        ++gHiddenMove;
    }     
        BlitMoveInfoIcon(WIN_HIDDEN_MOVE, gHiddenMove + 1, 3, 2);

        if (species != SPECIES_NIDORAN_M && species != SPECIES_NIDORAN_F)
        {
            switch (gGen)
            {
            //AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 66, 3, gBlueTextColor, 0, gMale);
            case MON_MALE:
            //else if(gGen==0xFE){
            AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 66, 3, gBlueTextColor,  0, gMale);
            break;
            case MON_FEMALE:
            //}else if(gGen==0xFF){
            AddTextPrinterParameterized3(WIN_POKEMON_NAME, 2, 66, 3, gRedTextColor, 0, gFemale);
            break;
            }
        }
        


        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 6, 1, gBlackTextColor, 0, gText_Total);

        ConvertIntToDecimalStringN(gStringVar1, gTotalStatsBS, STR_CONV_MODE_RIGHT_ALIGN, 3);
        ConvertIntToDecimalStringN(gStringVar2, gTotalStatsEV, STR_CONV_MODE_RIGHT_ALIGN, 3);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, BS_X, 1, gBlackTextColor, 0, gStringVar1);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, EV_X, 1, gBlackTextColor, 0, gStringVar2);

        ConvertIntToDecimalStringN(gStringVar4, gLevelMon, STR_CONV_MODE_LEFT_ALIGN, 3);

        if(gVarShiny==0){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 5, gGrayTextColor, 0, gLevel);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 208, 5, gGrayTextColor, 0, gStringVar4);
    }else if(gVarShiny==1){
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 187, 5, gShinyTextColor, 0, gLevel);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 208, 5, gShinyTextColor, 0, gStringVar4);
    }

        StringCopy(gStringVar4, gText_Your);
        GetSpeciesName(gStringVar1, species);
        StringAppend(gStringVar4, gStringVar1);
        StringAppend(gStringVar4, gText_Is);
        temp = (friendship * 100) / 0xFF;
        
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, 3);
        StringAppend(gStringVar4, gStringVar2);
        
        StringAppend(gStringVar4, gText_Happy);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 6, 17, gBlackTextColor, 0, gStringVar4);
        
        
    }else
    {
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 6, 1, gBlackTextColor, 0, gStepts);
        StringCopy(gStringVar4, gText_Less_Than);
        temp = (friendship + 1) * 0xFF;
        ConvertIntToDecimalStringN(gStringVar2, temp, STR_CONV_MODE_LEFT_ALIGN, GetDigits(temp));
        StringAppend(gStringVar4, gStringVar2);
        StringAppend(gStringVar4, gText_Steps_to_hatching);
        AddTextPrinterParameterized3(WIN_BOTTOM_BOX, 2, 6, 17, gBlackTextColor, 0, gStringVar4);
        AddTextPrinterParameterized3(WIN_HIDDEN_MOVE, 2, 3, 2, gBlackTextColor, 0, gNoneG);
    }
}

static void PrintWindow3(void)
{

        AddTextPrinterParameterized3(WIN_TYPE, 2, 1, 1, gGrayTextColor, 0, gNoneG);

}

static void PrintWindow4(void)
{

        AddTextPrinterParameterized3(WIN_HIDDEN_MOVE, 2, 1, 1, gGrayTextColor, 0, gNoneG);

}

/**
 * Returns the number of digits in a number less than 65536
 * 
 * Devuelve el número de dígitos de un número menor a 65536
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
 * This function returns a number, according to nature.
 * 0 = black
 * 1 = red
 * 2 = blue
 * 
 * Esta función devuelve un número, de acuerdo a la naturaleza.
 * 0 = negro
 * 1 = rojo
 * 2 = azul
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
