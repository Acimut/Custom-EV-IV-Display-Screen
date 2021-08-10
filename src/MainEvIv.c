#include "header.h"

extern void CB2_ShowEvIv(void);

/**
 * Punto de entrada
 * Inicio de la rutina
 * Función inicial
 * main function
 * como quieran llamarlo.
*/
void MainEvIv(void)
{
    SetVBlankCallback(NULL);
    //QuestLog_CutRecording();
    SetMainCallback2(CB2_ShowEvIv);
    ScriptContext2_Enable();
}
