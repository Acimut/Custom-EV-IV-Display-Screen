#include "header.h"
#include "Textos.h"
#include "Graphics.h"

#define SALTO_SPRITE TRUE   //TRUE = activado, FALSE, desactivado (no implementado aún)
#define SPECIES_EGG 412     //es el número de especie asignado al sprite del huevo.
#define PICMON_X 18         //coordenada x del sprite pokémon, se mide en tiles (x8 pixeles) en el BG0
#define PICMON_Y  5         //coordenada y del sprite pokémon, se mide en tiles (x8 pixeles) en el BG0

static void Task_EvIvInit(u8);
static u8 EvIvLoadGfx(void);
static void EvIvVblankHandler(void);
static void Task_WaitForExit(u8);
static void Task_EvIvReturnToOverworld(u8);
static void EvIvPrintText(struct Pokemon *mon);
static void AgregarColorPorNaturaleza(u8 naturaleza, u8 stat);
static void AgregarNumero(u16 num);
static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y);
static void Task_ScriptShowMonPic(u8 taskId);
static void HidePokemonPic2(u8 taskId);

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

//window 0 = nombre del pokémon
#define VENTANA0_ANCHO  29
#define VENTANA0_ALTO    2

//window 1 = estadísticas
#define VENTANA1_ANCHO  17
#define VENTANA1_ALTO   11
#define VENTANA1_BASEBLOCK  (VENTANA0_ANCHO * VENTANA0_ALTO)    //(width * height) + baseBlock del WindowTemplate anterior

//window 2 = texto en la barra inferior
#define VENTANA2_ANCHO  29
#define VENTANA2_ALTO    5
#define VENTANA2_BASEBLOCK VENTANA1_ANCHO * VENTANA1_ALTO + VENTANA1_BASEBLOCK  //(width * height) + baseBlock del WindowTemplate anterior

static const struct WindowTemplate plantilaWindow[] = {
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = VENTANA0_ANCHO,
        .height = VENTANA0_ALTO,
        .paletteNum = 15,
        .baseBlock = 0x000
    },{
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = VENTANA1_ANCHO,
        .height = VENTANA1_ALTO,
        .paletteNum = 15,
        .baseBlock = VENTANA1_BASEBLOCK
    },{
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
    u8 spriteTaskId;
    u16 totalAtributos[3];      //usado para almacenar e imprimir el total de estadísticas
    u16 eclosion;               //usado para calcular los pasos aprox. de eclosión en un huevo (?
    u16 tilemapBuffer[0x400];   //buffer para el bg
};

extern struct EvIv *gEvIv;

#define gState              gEvIv->state
#define gGfxStep            gEvIv->gfxStep
#define gCallbackStep       gEvIv->callbackStep
#define gActualMon          gEvIv->actualMon
#define gSpriteTaskId       gEvIv->spriteTaskId
#define gTotalAtributos     gEvIv->totalAtributos
#define gEclosion           gEvIv->eclosion
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
    //gSpriteId = 0;
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
        PlaySE(SE_CARD3);
        gState++;
        break;
    case 1:
        if (JOY_REPT(DPAD_DOWN) && gPlayerPartyCount > 1)
        {
            PlaySE(SE_SELECT);
            if (gActualMon == (gPlayerPartyCount - 1))
                gActualMon = 0;
            else
                gActualMon++;
            HidePokemonPic2(gSpriteTaskId);
            EvIvPrintText(&gPlayerParty[gActualMon]);
        }
        if (JOY_REPT(DPAD_UP) && gPlayerPartyCount > 1)
        {
            PlaySE(SE_SELECT);
            if (gActualMon == 0)
                gActualMon = (gPlayerPartyCount - 1);
            else
                gActualMon--;
            HidePokemonPic2(gSpriteTaskId);
            EvIvPrintText(&gPlayerParty[gActualMon]);
        }
        if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_CARD1);
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
    SetMainCallback2(CB2_ReturnToFieldFromDiploma);
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
    InitWindows(plantilaWindow);
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
        LoadPalette(gBgEvIvPal, 0, 0x20);       //paleta 0
        LoadPalette(stdpal_get(0), 0xf0, 0x20); //paleta 15 por defecto
        break;
    default:
        return 1;
    }
    gGfxStep++;
    return 0;
}

static void MostrarSprite(u16 especie, u32 otId, u32 personality, u8 x, u8 y, u8 isEgg)
{
    //imprime el sprite del pokémon, si es un huevo no suena grito.
    if (!isEgg)
    {
        ShowPokemonPic2(especie, otId, personality, x, y);
        PlayCry7(especie, 0);
    }else
        ShowPokemonPic2(SPECIES_EGG, 0, 0x8000, x, y);
}

/**
 * Parámetro 'u8 color' en AddTextPrinterParameterized3 que
 * define el color que tendrá el texto a imprimir, usando los
 * colores de la paleta de la ventana de la siguiente manera:
 * static const ALIGNED(4) u8 COLOR[3] = {FONDO, FUENTE, SOMBRA};
 * -----Colores PAL14 y PAL15----- (0=transparente)
 * 1=blanco         2=negro         3=gris
 * 4=rojo           5=rojo claro    6=verde
 * 7=verde claro    8=azul          9=azul claro
 * 
#define TEXT_COLOR_TRANSPARENT  0x0
#define TEXT_COLOR_WHITE        0x1
#define TEXT_COLOR_DARK_GREY    0x2
#define TEXT_COLOR_LIGHT_GREY   0x3
#define TEXT_COLOR_RED          0x4
#define TEXT_COLOR_LIGHT_RED    0x5
#define TEXT_COLOR_GREEN        0x6
#define TEXT_COLOR_LIGHT_GREEN  0x7
#define TEXT_COLOR_BLUE         0x8
#define TEXT_COLOR_LIGHT_BLUE   0x9
*/


static const u8 gColorTextoNegro[3]  = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GREY, TEXT_COLOR_LIGHT_GREY};
static const u8 gColorTextoGris[3]   = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GREY, TEXT_COLOR_DARK_GREY};

/**
 * COLOR_HIGHLIGHT_SHADOW = FC 04 @ takes 3 bytes
 * Cambio de color en línea de texto, FC 04 + 3 bytes.
 * Son bytes de texto que el juego interpreta como cambio de color
 * en el párrafo durante la impresión del texto, reemplaza el color
 * de la fuente del texto que le precede, usando los colores de la
 * paleta de la ventana, se configura de la siguiene manera:
 * static const u8 color[] = {0xFC, 0x04, fuente, fondo, sombra, 0xFF};
 * Es concatenado antes del texto a cambiar de color.
 * Usa la paleta de la ventana.
*/

static const u8 gColorNegro[]   = {0xFC, 0x04, TEXT_COLOR_DARK_GREY, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GREY, EOS};
static const u8 gColorAzul[]    = {0xFC, 0x04, TEXT_COLOR_BLUE,      TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GREY, EOS};
static const u8 gColorRojo[]    = {0xFC, 0x04, TEXT_COLOR_RED,       TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GREY, EOS};

//imprimir numero pokémon actual/total equipo - ventana 0
static void ImprimirVentana0(struct Pokemon *mon)
{
    u8 temp;
    temp = gActualMon + 1;                        //temp = número actual del pokémon.
    ConvertIntToDecimalStringN(gStringVar4,temp,2,1);   //se convierte ese número para imprimir en pantalla, se guarda en StringVar4
    StringAppend(gStringVar4,gText_Slash);              //Se concatena lo que tiene gStringVar4 con un '/'
    //total del equipo
    temp = gPlayerPartyCount;                           //temp = número total del equipo pokémon.
    ConvertIntToDecimalStringN(gStringVar1,temp,2,1);   //se convierte temp a un número para imprimir en pantalla, se guarda en gStringVar1
    StringAppend(gStringVar4,gStringVar1);              //Concatenar gStringVar1 con el resto de gStringVar4. 
                                                        //Ejemplo: si gStringVar4 = '1/' y gStringVar1 = '6', luego gStringVar4 = '1/6'
    AddTextPrinterParameterized3(0, 0x2, 0x2, 0x2, gColorTextoGris, 0, gStringVar4);//imprime en la ventana el texto concatenado.

    //imprime BS EV IV + nickname del pokemon actual - ventana 0
    StringCopy(gStringVar4,gText_BsEvIv);               //se puede usar StringCopy para limpiar gStringVar4 de textos anteriores.
    StringAppend(gStringVar4,gText_Espacios2);
    GetMonNickname(mon, gStringVar1);
    StringAppend(gStringVar4,gStringVar1);
    AddTextPrinterParameterized3(0, 0x2, 0x44, 0x2, gColorTextoGris, 0, gStringVar4);
}

/*
static void ImprimirVentana1(u16 especie, u8 nature, u8 isEgg)
{
    ;
}

static void ImprimirVentana2(u16 especie, u8 nature, u8 isEgg)
{
    ;
}
*/

/**
 * Esta función imprime la infomración en pantalla.
 * Aprovechamos que gStringVar4 tiene espacio para 1000 carácteres,
 * para acomodar todo el texto antes de imprimirlo en las ventanas.
*/
static void EvIvPrintText(struct Pokemon *mon)
{
    u8 temp     = 0;
    u16 especie = GetMonData(mon, MON_DATA_SPECIES, NULL);
    u8 nature   = GetNature(mon);
    u8 isEgg    = GetMonData(mon, MON_DATA_IS_EGG, NULL);
    u32 otId = GetMonData(mon, MON_DATA_OT_ID, NULL);
    u32 personality = GetMonData(mon, MON_DATA_PERSONALITY, NULL);

    MostrarSprite(especie, otId, personality, PICMON_X, PICMON_Y, isEgg);

    FillWindowPixelBuffer(0, 0);
    FillWindowPixelBuffer(1, 0);
    FillWindowPixelBuffer(2, 0);

    ImprimirVentana0(mon);

    //imprimir nombre de estadísticas - ventana 1
    StringCopy(gStringVar4,gText_Ps);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature, STAT_ATK);
    StringAppend(gStringVar4,gText_Ataque);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature, STAT_DEF);
    StringAppend(gStringVar4,gText_Defensa);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature, STAT_SPATK);
    StringAppend(gStringVar4,gText_AtqEsp);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature, STAT_SPDEF);
    StringAppend(gStringVar4,gText_DefEsp);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature, STAT_SPEED);
    StringAppend(gStringVar4,gText_Velocid);
    AddTextPrinterParameterized3(1, 0x2, 0x4, 0x3, gColorTextoNegro, 0, gStringVar4);

/**
 * Estadísticas - ventana 1
 * Si es un pokémon, imprime las estadísticas y hace otras operaciones.
 * Si es un huevo, solo imprime ? ? ?.
*/
    //si no es un huevo, imprime las estadísticas y suma los totales. - ventana 1
    if (!isEgg)
    {
    //  línea PS
        temp = gBaseStatsPtr[especie].baseHP;
        gTotalAtributos[0] = temp;//BS
        if (temp < 10)
        {
            StringCopy(gStringVar4,gText_Espacios2);
            ConvertIntToDecimalStringN(gStringVar2,temp,2,1);//1 digito
            StringAppend(gStringVar4,gStringVar2);
        }
        else if (temp < 100)
        {
            StringCopy(gStringVar4,gText_Espacios1);
            ConvertIntToDecimalStringN(gStringVar2,temp,2,2);//2 digitos
            StringAppend(gStringVar4,gStringVar2);
        }
        else
            ConvertIntToDecimalStringN(gStringVar4,temp,2,3);//3 dígitos

        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_HP_EV,NULL);
        gTotalAtributos[1] = temp;//EV
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        temp = GetMonData(mon,MON_DATA_HP_IV,NULL);
        gTotalAtributos[2] = temp;//IV
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea ATAQUE
        temp = gBaseStatsPtr[especie].baseAttack;
        gTotalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_ATK_EV,NULL);
        AgregarNumero(temp);
        gTotalAtributos[1] += temp;
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature, STAT_ATK);
        temp = GetMonData(mon,MON_DATA_ATK_IV,NULL);
        gTotalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea DEFENSA
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStatsPtr[especie].baseDefense;
        gTotalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_DEF_EV,NULL);
        gTotalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature, STAT_DEF);
        temp = GetMonData(mon,MON_DATA_DEF_IV,NULL);
        gTotalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea ATQ.ESP.
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStatsPtr[especie].baseSpAttack;
        gTotalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_SPATK_EV,NULL);
        gTotalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature, STAT_SPATK);
        temp = GetMonData(mon,MON_DATA_SPATK_IV,NULL);
        gTotalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea DEF.ESP.
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStatsPtr[especie].baseSpDefense;
        gTotalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_SPDEF_EV,NULL);
        gTotalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature, STAT_SPDEF);
        temp = GetMonData(mon,MON_DATA_SPDEF_IV,NULL);
        gTotalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea VELOCID.
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStatsPtr[especie].baseSpeed;
        gTotalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_SPEED_EV,NULL);
        gTotalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature, STAT_SPEED);
        temp = GetMonData(mon,MON_DATA_SPEED_IV,NULL);
        gTotalAtributos[2] += temp;
        AgregarNumero(temp);
        AddTextPrinterParameterized3(1, 0x2, 0x36, 0x3, gColorTextoNegro, 0, gStringVar4);

    //estadísticas si es un huevo. Imprime ? ? ? - ventana 1
    }else{
        StringCopy(gStringVar4,gText_CensurarHuevos);   //ps.
        StringAppend(gStringVar4,gText_SaltoDeLinea);
        StringAppend(gStringVar4,gText_CensurarHuevos); //atq.
        StringAppend(gStringVar4,gText_SaltoDeLinea);
        StringAppend(gStringVar4,gText_CensurarHuevos); //def.
        StringAppend(gStringVar4,gText_SaltoDeLinea);
        StringAppend(gStringVar4,gText_CensurarHuevos); //a.e.
        StringAppend(gStringVar4,gText_SaltoDeLinea);
        StringAppend(gStringVar4,gText_CensurarHuevos); //d.e.
        StringAppend(gStringVar4,gText_SaltoDeLinea);
        StringAppend(gStringVar4,gText_CensurarHuevos); //vel.
        AddTextPrinterParameterized3(1, 0x2, 0x36, 0x3, gColorTextoNegro, 0, gStringVar4);
    }

    //Información de felicidad/ciclos del huevo - ventana 2
    StringCopy(gStringVar4,gText_Espacios2);
    StringAppend(gStringVar4,gText_Total);
    StringAppend(gStringVar4,gText_Espacios1);
    StringAppend(gStringVar4,gText_Espacios2);
    if(!isEgg)
    {
        //agrega los totales
        AgregarNumero(gTotalAtributos[0]);
        StringAppend(gStringVar4,gText_Espacios2);
        AgregarNumero(gTotalAtributos[1]);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarNumero(gTotalAtributos[2]);

        //agrega 'tu {EspeciePokémon} es x % feliz.'
        StringAppend(gStringVar4, gText_SaltoDeLinea);
        StringAppend(gStringVar4, gText_Tu);
        GetSpeciesName(gStringVar1, especie);
        StringAppend(gStringVar4, gStringVar1);
        StringAppend(gStringVar4, gText_Es);
        temp = (GetMonData(mon, MON_DATA_FRIENDSHIP, NULL) * 100) / 0xFF;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Feliz);
        
    }else
    {
        //agrega ? ? ? en el total si es un huevo
        StringAppend(gStringVar4, gText_CensurarHuevos);
        //agrega 'Aún falta... ¡Menos de x pasos para eclosionar!'
        StringAppend(gStringVar4, gText_Espacios2);
        StringAppend(gStringVar4, gText_AunFalta);
        StringAppend(gStringVar4, gText_SaltoDeLinea);
        StringAppend(gStringVar4, gText_Menos);
        gEclosion = ((GetMonData(mon, MON_DATA_FRIENDSHIP, NULL) + 1) * 0xFF);
        if(gEclosion < 10)
            ConvertIntToDecimalStringN(gStringVar2,gEclosion,2,1);
        else if (gEclosion < 100)
            ConvertIntToDecimalStringN(gStringVar2,gEclosion,2,2);
        else if (gEclosion < 1000)
            ConvertIntToDecimalStringN(gStringVar2,gEclosion,2,3);
        else if (gEclosion < 10000)
            ConvertIntToDecimalStringN(gStringVar2,gEclosion,2,4);
        else
            ConvertIntToDecimalStringN(gStringVar2,gEclosion,2,5);
        StringAppend(gStringVar4,gStringVar2);
        StringAppend(gStringVar4,gText_Pasos);
    }
    AddTextPrinterParameterized3(2, 0x2, 0x2, 0x4, gColorTextoNegro, 0, gStringVar4);

    //imprime las ventanas con los textos en pantalla
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
}

static void HidePokemonPic2(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    task->data[0]++;
    //FreeResourcesAndDestroySprite(&gSprites[task->data[2]], task->data[2]);
    //DestroyTask(taskId);
}

static void Task_ScriptShowMonPic(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    task->data[3]++;
    if (task->data[3] > 0x7FF0)
        task->data[3] = 65;
    switch (task->data[0])
    {
    case 0:
        task->data[0]++;
        break;
    case 1:
        if (task->data[3] < 10)
            gSprites[task->data[2]].pos1.x -= 1;//10f x
        else if (task->data[3] < 18)
        {
            gSprites[task->data[2]].pos1.y += 1;//8f down 1 = 8p
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 24)
        {
            gSprites[task->data[2]].pos1.y += 2;//6f down 2 = 12p
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 28)
        {
            gSprites[task->data[2]].pos1.y += 3;//4f down 3 = 12p   _32_
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 30)
        {
            gSprites[task->data[2]].pos1.y -= 3;//2f up 3 = -6
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 34)
        {
            gSprites[task->data[2]].pos1.y -= 2;//4f up 2 = -8
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 40)
        {
            gSprites[task->data[2]].pos1.y -= 1;//6f up 1 = -6      _20_
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 48)
            gSprites[task->data[2]].pos1.x -= 1;//8f x              _8_
        else if (task->data[3] < 54)
        {
            gSprites[task->data[2]].pos1.y += 1;//6f down 1 = 6p
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 58)
        {
            gSprites[task->data[2]].pos1.y += 2;//4f down 2 = 8p
            gSprites[task->data[2]].pos1.x -= 1;
        }
        else if (task->data[3] < 60)
        {
            gSprites[task->data[2]].pos1.y += 3;//2f down 3 = 6p   _20_
            gSprites[task->data[2]].pos1.x -= 1;
        }
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

static void ShowPokemonPic2(u16 species, u32 otId, u32 personality, u8 x, u8 y)
{
    u8 spriteId;
    //u8 taskId;

    //spriteId = CreateMonSprite_PicBox(species, 8 * x + 40, 8 * y + 40, FALSE);
    spriteId = CreateMonSprite_FieldMove(species, otId, personality, 8 * x + 40, 8 * y + 40, FALSE);
    gSpriteTaskId = CreateTask(Task_ScriptShowMonPic, 80);

    gSprites[spriteId].pos1.y -= 32;
    gSprites[spriteId].pos1.x += 60;

    gTasks[gSpriteTaskId].data[0] = 0;
    gTasks[gSpriteTaskId].data[1] = species;
    gTasks[gSpriteTaskId].data[2] = spriteId;
    gTasks[gSpriteTaskId].data[3] = 0;
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.priority = 0;
}

//Esta función convierte un número a texto, si es menor a 100 rellena con espacios para alinear los números a la derecha.
static void AgregarNumero(u16 num)
{
    if (num < 10)
    {
        StringCopy(gStringVar3,gText_Espacios2);
        StringAppend(gStringVar4,gStringVar3);
        ConvertIntToDecimalStringN(gStringVar2,num,2,1);
    }
    else if (num < 100)
    {
        StringCopy(gStringVar3,gText_Espacios1);
        StringAppend(gStringVar4,gStringVar3);
        ConvertIntToDecimalStringN(gStringVar2,num,2,2);
    }
    else if (num < 1000)
        ConvertIntToDecimalStringN(gStringVar2,num,2,3);
    else if (num < 10000)
        ConvertIntToDecimalStringN(gStringVar2,num,2,4);
    else
        ConvertIntToDecimalStringN(gStringVar2,num,2,5);
    StringAppend(gStringVar4,gStringVar2);
}

static void AgregarColorPorNaturaleza(u8 naturaleza, u8 stat)
{
    switch (stat)
    {
    case STAT_ATK:
        switch (naturaleza)
        {
        case NATURE_LONELY:
        case NATURE_BRAVE:
        case NATURE_ADAMANT:
        case NATURE_NAUGHTY:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case NATURE_BOLD:
        case NATURE_TIMID:
        case NATURE_MODEST:
        case NATURE_CALM:
            StringCopy(gStringVar3,gColorAzul);
            break;
        
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case STAT_DEF:
        switch (naturaleza)
        {
        case NATURE_BOLD:
        case NATURE_RELAXED:
        case NATURE_IMPISH:
        case NATURE_LAX:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case NATURE_LONELY:
        case NATURE_HASTY:
        case NATURE_MILD:
        case NATURE_GENTLE:
            StringCopy(gStringVar3,gColorAzul);
            break;
        
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case STAT_SPATK:
        switch (naturaleza)
        {
        case NATURE_MODEST:
        case NATURE_MILD:
        case NATURE_QUIET:
        case NATURE_RASH:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case NATURE_ADAMANT:
        case NATURE_IMPISH:
        case NATURE_JOLLY:
        case NATURE_CAREFUL:
            StringCopy(gStringVar3,gColorAzul);
            break;
        
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case STAT_SPDEF:
        switch (naturaleza)
        {
        case NATURE_CALM:
        case NATURE_GENTLE:
        case NATURE_SASSY:
        case NATURE_CAREFUL:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case NATURE_NAUGHTY:
        case NATURE_LAX:
        case NATURE_NAIVE:
        case NATURE_RASH:
            StringCopy(gStringVar3,gColorAzul);
            break;
        
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case STAT_SPEED:
        switch (naturaleza)
        {
        case NATURE_TIMID:
        case NATURE_HASTY:
        case NATURE_JOLLY:
        case NATURE_NAIVE:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case NATURE_BRAVE:
        case NATURE_RELAXED:
        case NATURE_QUIET:
        case NATURE_SASSY:
            StringCopy(gStringVar3,gColorAzul);
            break;
        
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    default:
        StringCopy(gStringVar3,gColorNegro);
        break;
    }
    StringAppend(gStringVar4,gStringVar3);
}

//----------------------------------------------------------------------//
//          EL SIGUIENTE CÓDIGO DA EL MISMO RESULTADO                   //
//          QUE EL ANTERIOR, PERO ES MÁS LARGO LOL                      //
//----------------------------------------------------------------------//
/*
static void AgregarColorPorNaturaleza2(u8 naturaleza, u8 stat)
{
    switch (naturaleza)
    {
    case NATURE_LONELY:
        switch (stat)
        {
        case STAT_ATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_DEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_BRAVE:
        switch (stat)
        {
        case STAT_ATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPEED:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_ADAMANT:
        switch (stat)
        {
        case STAT_ATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_NAUGHTY:
        switch (stat)
        {
        case STAT_ATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_BOLD:
        switch (stat)
        {
        case STAT_DEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_ATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_RELAXED:
        switch (stat)
        {
        case STAT_DEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPEED:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_IMPISH:
        switch (stat)
        {
        case STAT_DEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_LAX:
        switch (stat)
        {
        case STAT_DEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_TIMID:
        switch (stat)
        {
        case STAT_SPEED:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_ATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_HASTY:
        switch (stat)
        {
        case STAT_SPEED:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_DEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_JOLLY:
        switch (stat)
        {
        case STAT_SPEED:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_NAIVE:
        switch (stat)
        {
        case STAT_SPEED:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_MODEST:
        switch (stat)
        {
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_ATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_MILD:
        switch (stat)
        {
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_DEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_QUIET:
        switch (stat)
        {
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_DEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_RASH:
        switch (stat)
        {
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_CALM:
        switch (stat)
        {
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_ATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_GENTLE:
        switch (stat)
        {
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_DEF:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_SASSY:
        switch (stat)
        {
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPEED:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    case NATURE_CAREFUL:
        switch (stat)
        {
        case STAT_SPDEF:
            StringCopy(gStringVar3,gColorRojo);
            break;
        case STAT_SPATK:
            StringCopy(gStringVar3,gColorAzul);
            break;
        default:
            StringCopy(gStringVar3,gColorNegro);
            break;
        }
        break;
    default:
        StringCopy(gStringVar3,gColorNegro);
        break;
    }
    StringAppend(gStringVar4,gStringVar3);
}
*/
