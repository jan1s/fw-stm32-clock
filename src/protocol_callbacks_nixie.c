#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE

#include "protocol.h"
#include "nixie.h"
#include "nixie_clock.h"
#include <string.h>

void protocolMsgCallbackNixTyp(protocolMsgNixTyp_t *typ)
{
    //BKP_WriteBackupRegister(BKP_DR4, typ->type & 0x00FF);
    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_TYP);
}

void protocolMsgPollCallbackNixTyp(void)
{
    protocolMsgNixTyp_t typ;
    typ.type = 1;
    protocolMsgSendNixTyp(&typ);
}

void protocolMsgCallbackNixMod(protocolMsgNixMod_t *mod)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Write to the BKP Domain */
    BKP_WriteBackupRegister(BKP_DR4, mod->mode & 0xFF);

    /* Deny access to BKP Domain */
    PWR_BackupAccessCmd(DISABLE);

    nixieClockMode = mod->mode;
    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_MOD);
}

void protocolMsgPollCallbackNixMod(void)
{
    protocolMsgNixMod_t mod;
    mod.mode = nixieClockMode;
    protocolMsgSendNixMod(&mod);
}

#ifdef CFG_TYPE_NIXIE_4T
void protocolMsgCallbackNixTst(protocolMsgNixTst_t *tst)
{

    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_TST);
}

void protocolMsgCallbackNixR4T(protocolMsgNixR4T_t *r4t)
{
    /*
    fdisp_84x7_t d;
    memcpy(&d, r12, sizeof(fdisp_84x7_t));
    flipdot_set_84x7(&d);
    protocolReplyPacket(PROTOCOL_MSG_ID_FPD_R12);
    */
}
#endif

#ifdef CFG_TYPE_NIXIE_6T
void protocolMsgCallbackNixTst(protocolMsgNixTst_t *tst)
{

    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_TST);
}

void protocolMsgCallbackNixR6T(protocolMsgFpdR16_t *r6t)
{
    /*
    fdisp_112x16_t d;
    memcpy(&d, r16, sizeof(fdisp_112x16_t));
    flipdot_set_112x16(&d);
    protocolReplyPacket(PROTOCOL_MSG_ID_FPD_R16);
    */
}
#endif

#endif // CFG_TYPE_NIXIE
