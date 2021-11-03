#include "include/constants/songs.h"

#include "main_eviv.h"

void Call_EvIv(void)
{
#ifdef FIRERED
//FIFERED
    PlaySE(SE_CARD_OPEN);
    QuestLog_CutRecording();
#else
//EMERALD
    PlaySE(SE_RG_CARD_OPEN);
#endif
    SetVBlankCallback(NULL);
    SetMainCallback2(CB2_ShowEvIv);
    ScriptContext2_Enable();
}
