#include "header.h"

static u8 EvIvLoadGfx(void);
static void EvIvVblankHandler(void);
static void Task_WaitForExit(u8);
static void Task_EvIvReturnToOverworld(u8);
static void EvIvPrintText(struct Pokemon *mon);

static void AgregarColorPorNaturaleza
(u8 naturaleza, u8 up1, u8 up2, u8 up3, u8 up4, u8 down1, u8 down2, u8 down3, u8 down4);
static void AgregarNumero(u16 num);
static void ShowPokemonPic(u16 species, u8 x, u8 y);
static void Task_ScriptShowMonPic(u8 taskId);
static void PicboxCancel2(void);

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

#define VENTANA_ANCHO   17
#define VENTANA_ALTO    11
#define VENTANA2_ANCHO  29
#define VENTANA2_ALTO    2

static const struct WindowTemplate plantilaWindow[] = {
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = VENTANA_ANCHO,
        .height = VENTANA_ALTO,
        .paletteNum = 15,
        .baseBlock = 0x000
    },{
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 2,
        .width = VENTANA2_ANCHO,
        .height = VENTANA2_ALTO,
        .paletteNum = 15,
        .baseBlock = VENTANA_ANCHO * VENTANA_ALTO //(width * height) + baseBlock del WindowTemplate anterior
    },{
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 15,
        .width = 29,
        .height = 5,
        .paletteNum = 15,
        .baseBlock = VENTANA2_ANCHO * VENTANA2_ALTO + (VENTANA_ANCHO * VENTANA_ALTO)
    }, DUMMY_WIN_TEMPLATE
};


static void VCBC_EvIvOam(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void Task_EvIvInit(u8 taskId)
{
    switch (gEvIv->callbackStep)
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
        CopyToBgTilemapBuffer(1, gEvIvRAW, 0, 0);
        break;
    case 4:
        SetGpuReg(REG_OFFSET_BG1HOFS, 0);
        break;
    case 5:
        EvIvPrintText(&gPlayerParty[gEvIv->actualMon]);
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
    gEvIv->callbackStep++;
}

static void Task_WaitForExit(u8 taskId)
{
    switch (gEvIv->state)
    {
    case 0:
        PlaySE(SE_CARD3);
        gEvIv->state++;
        break;
    case 1:
        if (JOY_NEW(DPAD_DOWN) && gPlayerPartyCount > 1)
        {
            PlaySE(SE_SELECT);
            if (gEvIv->actualMon == (gPlayerPartyCount - 1))
                gEvIv->actualMon = 0;
            else
                gEvIv->actualMon++;
            PicboxCancel2();
            EvIvPrintText(&gPlayerParty[gEvIv->actualMon]);
        }
        if (JOY_NEW(DPAD_UP) && gPlayerPartyCount > 1)
        {
            PlaySE(SE_SELECT);
            if (gEvIv->actualMon == 0)
                gEvIv->actualMon = (gPlayerPartyCount - 1);
            else
                gEvIv->actualMon--;
            PicboxCancel2();
            EvIvPrintText(&gPlayerParty[gEvIv->actualMon]);
        }
        if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
        {
            PlaySE(SE_CARD1);
            BeginNormalPaletteFade(0xFFFFFFFF, 0, 0, 16, RGB_BLACK);
            gEvIv->state++;
        }
        break;
    case 2:
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

static void EvIvVblankHandler(void)
{
    void *vram = (void *)VRAM;
    DmaClearLarge16(3, vram, VRAM_SIZE, 0x1000);
    DmaClear32(3, (void *)OAM, OAM_SIZE);
    DmaClear16(3, (void *)PLTT, PLTT_SIZE);
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, plantilaBG, 2);
    ChangeBgX(0, 0, 0);
    ChangeBgY(0, 0, 0);
    ChangeBgX(1, 0, 0);
    ChangeBgY(1, 0, 0);
    ChangeBgX(2, 0, 0);
    ChangeBgY(2, 0, 0);
    ChangeBgX(3, 0, 0);
    ChangeBgY(3, 0, 0);
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
    switch (gEvIv->gfxStep)
    {
    case 0:
        ResetTempTileDataBuffers();
        break;
    case 1:
        DecompressAndCopyTileDataToVram(1, gEvIvTILE, 0, 0, 0);
        break;
    case 2:
        if (!(FreeTempTileDataBuffersIfPossible() == 1))
        {
            break;
        }
        return 0;
    case 3:
        LoadPalette(gEvIvPAL, 0, 0x20);
    default:
        return 1;
    }
    gEvIv->gfxStep++;
    return 0;
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
*/

static const ALIGNED(4) u8 gColorTextoNegro[3]  = {0, 2, 3};  //paleta 15 usada en plantilaWindow
static const ALIGNED(4) u8 gColorTextoGris[3]   = {0, 3, 2};  //paleta 15 usada en plantilaWindow

/**
 * COLOR_HIGHLIGHT_SHADOW = FC 04 @ takes 3 bytes
 * Cambio de color en línea de texto, FC 04 + 3 bytes.
 * Son bytes de texto que el juego interpreta como cambio de color
 * en el párrafo durante la impresión del texto, reemplaza el color
 * de la fuente del texto que le precede, usando los colores de la
 * paleta de la ventana, se configura de la siguiene manera:
 * static const u8 color[] = {0xFC, 0x04, fuente, fondo, sombra, 0xFF};
 * Es concatenado antes del texto a cambiar de color.
*/

static const u8 gColorNegro[]   = {0xFC, 0x04, 0x02, 0x00, 0x03, 0xFF};//colores basados en la paleta 15 usada en WindowTemplate
static const u8 gColorAzul[]    = {0xFC, 0x04, 0x08, 0x00, 0x03, 0xFF};
static const u8 gColorRojo[]    = {0xFC, 0x04, 0x04, 0x00, 0x03, 0xFF};

static const u8 gText_Espacios1[]   = { 0x00, 0xFF};        //' '
static const u8 gText_Espacios2[]   = { 0x00, 0x00, 0xFF};  //'  '
//static const u8 gText_Espacios3[] = {0x00, 0x00, 0x00, 0xFF};//'   '
//static const u8 gText_Espacios4[] = {0x00, 0x00, 0x00, 0x00, 0xFF};//'    '
static const u8 gText_Slash[]       = {0xBA, 0xFF};         // '/'
static const u8 gText_SaltoDeLinea[]    = {0xFE, 0xFF};     // '\n' = FE @ new line
static const u8 gText_CensurarHuevos[]  = {0x0, 0x0, 0xAC, 0x0, 0x0, 0x0, 0x0, 0xAC, 0x0, 0x0, 0x0, 0xAC, 0xFF};    //'? ? ?'

static const u8 gText_BsEvIv[]  = {0xBC, 0xCD, 0x00, 0x00, 0x00, 0xBF, 0xD0, 0x00, 0x00, 
                                   0xC3, 0xD0, 0xFF};                                       //'BS _ _EV _ IV'

static const u8 gText_Ps[]      = {0xCA, 0xCD, 0xFF};                                       //'PS'
static const u8 gText_Ataque[]  = {0xBB, 0xCE, 0xBB, 0xCB, 0xCF, 0xBF, 0xFF};               //'ATAQUE'
static const u8 gText_Defensa[] = {0xBE, 0xBF, 0xC0, 0xBF, 0xC8, 0xCD, 0xBB, 0xFF};         //'DEFENSA'
static const u8 gText_AtqEsp[]  = {0xBB, 0xCE, 0xCB, 0xAD, 0xBF, 0xCD, 0xCA, 0xAD, 0xFF};   //'ATQ.ESP.'
static const u8 gText_DefEsp[]  = {0xBE, 0xBF, 0xC0, 0xAD, 0xBF, 0xCD, 0xCA, 0xAD, 0xFF};   //'DEF.ESP.'
static const u8 gText_Velocid[] = {0xD0, 0xBF, 0xC6, 0xC9, 0xBD, 0xC3, 0xBE, 0xAD, 0xFF};   //'VELOCID.'

static const u8 gText_Total[]   = {0xCE, 0xC9, 0xCE, 0xBB, 0xC6, 0xF0, 0x00, 0xFF};         //'TOTAL: '

static const u8 gText_Tu[]      = {0xCE, 0xE9, 0x00, 0xFF};                                 //'Tu '
static const u8 gText_Es[]      = {0x00, 0xD9, 0xE7, 0x00, 0xFF};                           //' es '
static const u8 gText_Feliz[]   = {0x5B, 0x00, 0xDA, 0xD9, 0xE0, 0xDD, 0xEE, 0xAD, 0xFF};   //'% felíz.'

static const u8 gText_AunFalta[]= {0xBB, 0x27, 0xE2, 0x00, 0xDA, 0xD5, 0xE0, 0xE8, 0xD5, 0xB0, 0xFF};   //'Aún falta...'
static const u8 gText_Menos[]   = {0x52, 0xC7, 0xD9, 0xE2, 0xE3, 0xE7, 0x00, 0xD8, 0xD9, 
                                   0x00, 0xFF};                                             //'¡Menos de '
static const u8 gText_Pasos[]   = {0x00, 0xE4, 0xD5, 0xE7, 0xE3, 0xE7, 0x00, 0xE4, 0xD5,    
                                   0xE6, 0xD5, 0x00, 0xD9, 0xD7, 0xE0, 0xE3, 0xE7, 0xDD, 
                                   0xE3, 0xE2, 0xD5, 0xE6, 0xAB, 0xFF};                     //' pasos para eclosionar.'

#define SPECIES_EGG 412 //es el número de especie asignado al sprite del huevo.

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

    FillWindowPixelBuffer(0, 0);
    FillWindowPixelBuffer(1, 0);
    FillWindowPixelBuffer(2, 0);

    //imprimir numero pokémon actual/total equipo - ventana 1
    temp = gEvIv->actualMon + 1;                        //temp = número actual del pokémon.
    ConvertIntToDecimalStringN(gStringVar4,temp,2,1);   //se convierte ese número para imprimir en pantalla, se guarda en StringVar4
    StringAppend(gStringVar4,gText_Slash);              //Se concatena lo que tiene gStringVar4 con un '/'
    //total del equipo
    temp = gPlayerPartyCount;                           //temp = número total del equipo pokémon.
    ConvertIntToDecimalStringN(gStringVar1,temp,2,1);   //se convierte temp a un número para imprimir en pantalla, se guarda en gStringVar1
    StringAppend(gStringVar4,gStringVar1);              //Concatenar gStringVar1 con el resto de gStringVar4. 
                                                        //Ejemplo: si gStringVar4 = '1/' y gStringVar1 = '6', luego gStringVar4 = '1/6'
    AddTextPrinterParameterized3(1, 0x2, 0x2, 0x2, gColorTextoGris, 0, gStringVar4);//imprime en la ventana el texto concatenado.

    //imprime BS EV IV + nickname del pokemon actual - ventana 1
    StringCopy(gStringVar4,gText_BsEvIv);               //se puede usar StringCopy para limpiar gStringVar4 de textos anteriores.
    StringAppend(gStringVar4,gText_Espacios2);
    GetMonNickname(mon, gStringVar1);
    StringAppend(gStringVar4,gStringVar1);
    AddTextPrinterParameterized3(1, 0x2, 0x44, 0x2, gColorTextoGris, 0, gStringVar4);    

    //imprimir nombre de atributos - ventana 0
    StringCopy(gStringVar4,gText_Ps);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature,1,2,3,4,5,10,15,20);
    StringAppend(gStringVar4,gText_Ataque);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature,5,7,8,9,1,11,16,21);
    StringAppend(gStringVar4,gText_Defensa);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature,15,16,17,19,3,8,13,23);
    StringAppend(gStringVar4,gText_AtqEsp);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature,20,21,22,23,4,9,14,19);
    StringAppend(gStringVar4,gText_DefEsp);
    StringAppend(gStringVar4,gText_SaltoDeLinea);
    AgregarColorPorNaturaleza(nature,10,11,13,14,2,7,17,22);
    StringAppend(gStringVar4,gText_Velocid);
    AddTextPrinterParameterized3(0, 0x2, 0x4, 0x3, gColorTextoNegro, 0, gStringVar4);

/**
 * Estadísticas - ventana 0
 * Si es un pokémon, imprime las estadísticas y hace otras operaciones.
 * Si es un huevo, solo imprime ? ? ?.
*/
    //si no es un huevo, imprime las estadísticas y suma los totales. - ventana 0
    if (!isEgg)
    {
    //  línea PS
        temp = gBaseStats[especie].baseHP;
        gEvIv->totalAtributos[0] = temp;//BS
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
        gEvIv->totalAtributos[1] = temp;//EV
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        temp = GetMonData(mon,MON_DATA_HP_IV,NULL);
        gEvIv->totalAtributos[2] = temp;//IV
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea ATAQUE
        temp = gBaseStats[especie].baseAttack;
        gEvIv->totalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_ATK_EV,NULL);
        AgregarNumero(temp);
        gEvIv->totalAtributos[1] += temp;
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature,1,2,3,4,5,10,15,20);
        temp = GetMonData(mon,MON_DATA_ATK_IV,NULL);
        gEvIv->totalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea DEFENSA
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStats[especie].baseDefense;
        gEvIv->totalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_DEF_EV,NULL);
        gEvIv->totalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature,5,7,8,9,1,11,16,21);
        temp = GetMonData(mon,MON_DATA_DEF_IV,NULL);
        gEvIv->totalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea ATQ.ESP.
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStats[especie].baseSpAttack;
        gEvIv->totalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_SPATK_EV,NULL);
        gEvIv->totalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature,15,16,17,19,3,8,13,23);
        temp = GetMonData(mon,MON_DATA_SPATK_IV,NULL);
        gEvIv->totalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea DEF.ESP.
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStats[especie].baseSpDefense;
        gEvIv->totalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_SPDEF_EV,NULL);
        gEvIv->totalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature,20,21,22,23,4,9,14,19);
        temp = GetMonData(mon,MON_DATA_SPDEF_IV,NULL);
        gEvIv->totalAtributos[2] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_SaltoDeLinea);

    //  línea VELOCID.
        StringAppend(gStringVar4,gColorNegro);
        temp = gBaseStats[especie].baseSpeed;
        gEvIv->totalAtributos[0] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios2);
        temp = GetMonData(mon,MON_DATA_SPEED_EV,NULL);
        gEvIv->totalAtributos[1] += temp;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarColorPorNaturaleza(nature,10,11,13,14,2,7,17,22);
        temp = GetMonData(mon,MON_DATA_SPEED_IV,NULL);
        gEvIv->totalAtributos[2] += temp;
        AgregarNumero(temp);
        AddTextPrinterParameterized3(0, 0x2, 0x36, 0x3, gColorTextoNegro, 0, gStringVar4);

    //estadísticas si es un huevo. Imprime ? ? ? - ventana 0
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
        AddTextPrinterParameterized3(0, 0x2, 0x36, 0x3, gColorTextoNegro, 0, gStringVar4);
    }

    //Información de felicidad/ciclos del huevo - ventana 2
    StringCopy(gStringVar4,gText_Espacios2);
    StringAppend(gStringVar4,gText_Total);
    StringAppend(gStringVar4,gText_Espacios1);
    if(!isEgg)
    {
        //agrega los totales
        AgregarNumero(gEvIv->totalAtributos[0]);
        StringAppend(gStringVar4,gText_Espacios2);
        AgregarNumero(gEvIv->totalAtributos[1]);
        StringAppend(gStringVar4,gText_Espacios1);
        AgregarNumero(gEvIv->totalAtributos[2]);

        //agrega 'tu {EspeciePokémon} es x % feliz.'
        StringAppend(gStringVar4,gText_SaltoDeLinea);
        StringAppend(gStringVar4,gText_Tu);
        GetSpeciesName(gStringVar1, especie);
        StringAppend(gStringVar4,gStringVar1);
        StringAppend(gStringVar4,gText_Es);
        temp = (GetMonData(mon, MON_DATA_FRIENDSHIP, NULL) * 100) / 0xFF;
        AgregarNumero(temp);
        StringAppend(gStringVar4,gText_Feliz);
        
    }else
    {
        //agrega ? ? ? en el total si es un huevo
        StringAppend(gStringVar4,gText_CensurarHuevos);
        //agrega 'Aún falta... ¡Menos de x pasos para eclosionar!'
        StringAppend(gStringVar4,gText_Espacios2);
        StringAppend(gStringVar4,gText_AunFalta);
        StringAppend(gStringVar4,gText_SaltoDeLinea);
        StringAppend(gStringVar4,gText_Menos);
        gEvIv->eclosion = ((GetMonData(mon, MON_DATA_FRIENDSHIP, NULL) + 1) * 0xFF);
        if(gEvIv->eclosion < 10)
            ConvertIntToDecimalStringN(gStringVar2,gEvIv->eclosion,2,1);
        else if (gEvIv->eclosion < 100)
            ConvertIntToDecimalStringN(gStringVar2,gEvIv->eclosion,2,2);
        else if (gEvIv->eclosion < 1000)
            ConvertIntToDecimalStringN(gStringVar2,gEvIv->eclosion,2,3);
        else if (gEvIv->eclosion < 10000)
            ConvertIntToDecimalStringN(gStringVar2,gEvIv->eclosion,2,4);
        else
            ConvertIntToDecimalStringN(gStringVar2,gEvIv->eclosion,2,5);
        StringAppend(gStringVar4,gStringVar2);
        StringAppend(gStringVar4,gText_Pasos);
    }
    AddTextPrinterParameterized3(2, 0x2, 0x2, 0x4, gColorTextoNegro, 0, gStringVar4);

    //imprime las ventanas con los textos en pantalla
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);

    //imprime el sprite del pokémon, si es un huevo no suena grito.
    if (!isEgg)
    {
        ShowPokemonPic(especie, 18, 5);// x, y se mide en tiles (x8 pixeles) en el BG0
        PlayCry7(especie, 0);
    }else
        ShowPokemonPic(SPECIES_EGG, 18, 5);
}

static void PicboxCancel2(void)
{
    u8 taskId = FindTaskIdByFunc(Task_ScriptShowMonPic);
    struct Task * task;
    if (taskId != 0xFF)
    {
        task = &gTasks[taskId];
        switch (task->data[0])
        {
        case 0:
        case 1:
        case 2:
            FreeResourcesAndDestroySprite(&gSprites[task->data[2]], task->data[2]);

            DestroyTask(taskId);
            break;
        case 3:

            DestroyTask(taskId);
            break;
        }
    }
}

static void Task_ScriptShowMonPic(u8 taskId)
{
    struct Task * task = &gTasks[taskId];
    switch (task->data[0])
    {
    case 0:
        task->data[0]++;
        break;
    case 1:
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

static void ShowPokemonPic(u16 species, u8 x, u8 y)
{
    u8 spriteId;
    u8 taskId;

    spriteId = CreateMonSprite_PicBox(species, 8 * x + 40, 8 * y + 40, FALSE);
    taskId = CreateTask(Task_ScriptShowMonPic, 80);

    gTasks[taskId].data[0] = 0;
    gTasks[taskId].data[1] = species;
    gTasks[taskId].data[2] = spriteId;
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.priority = 0;
}

static void AgregarColorPorNaturaleza
(u8 naturaleza, u8 up1, u8 up2, u8 up3, u8 up4, u8 down1, u8 down2, u8 down3, u8 down4)
{
    if (naturaleza == up1 || naturaleza == up2 || naturaleza == up3 || naturaleza == up4)
        StringCopy(gStringVar3,gColorRojo);
    else if (naturaleza == down1 || naturaleza == down2 || naturaleza == down3 || naturaleza == down4)
        StringCopy(gStringVar3,gColorAzul);
    else
        StringCopy(gStringVar3,gColorNegro);
    StringAppend(gStringVar4,gStringVar3);
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