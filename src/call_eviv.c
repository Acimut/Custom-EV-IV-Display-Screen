#include "constants/songs.h"

#include "main_eviv.h"

void EvIv_PlayerParty(void)
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
    SetMainCallback2(CB2_ShowEvIv_PlayerParty);
    ScriptContext2_Enable();
}
