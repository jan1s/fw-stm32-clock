#include "platform_config.h"

#ifdef CFG_TYPE_FLIPDOT

#include "protocol.h"
#include "flipdot.h"
#include "flipdot_clock.h"
#include <string.h>

void protocolMsgCallbackFpdTyp(protocolMsgFpdTyp_t *typ)
{
    //BKP_WriteBackupRegister(BKP_DR4, typ->type & 0x00FF);
    protocolReplyPacket(PROTOCOL_MSG_ID_FPD_TYP);
}

void protocolMsgPollCallbackFpdTyp(void)
{
    protocolMsgFpdTyp_t typ;
    typ.type = 1;
    protocolMsgSendFpdTyp(&typ);
}

void protocolMsgCallbackFpdMod(protocolMsgFpdMod_t *mod)
{
	/* Enable PWR and BKP clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

	/* Allow access to BKP Domain */
	PWR_BackupAccessCmd(ENABLE);

	/* Write to the BKP Domain */
    BKP_WriteBackupRegister(BKP_DR4, mod->mode & 0xFF);

    /* Deny access to BKP Domain */
    PWR_BackupAccessCmd(DISABLE);

    flipdotClockMode = mod->mode;
    protocolReplyPacket(PROTOCOL_MSG_ID_FPD_MOD);
}

void protocolMsgPollCallbackFpdMod(void)
{
    protocolMsgFpdMod_t mod;
    mod.mode = flipdotClockMode;
    protocolMsgSendFpdMod(&mod);
}

void protocolMsgCallbackFpdTst(protocolMsgFpdTst_t *tst)
{
    if(tst->test == 42)
    {
        flipdot_wipe_84x7(0);
        flipdot_wipe_84x7(1);
    }
    protocolReplyPacket(PROTOCOL_MSG_ID_FPD_TST);
}

#ifdef CFG_TYPE_FLIPDOT_84X7
void protocolMsgCallbackFpdR12(protocolMsgFpdR12_t *r12)
{
    fdisp_84x7_t d;
    memcpy(&d, r12, sizeof(fdisp_84x7_t));
    flipdot_set_84x7(&d);
    protocolReplyPacket(PROTOCOL_MSG_ID_FPD_R12);
}
#endif

#ifdef CFG_TYPE_FLIPDOT_112X16
void protocolMsgCallbackFpdR16(protocolMsgFpdR16_t *r16)
{
    fdisp_112x16_t d;
    memcpy(&d, r16, sizeof(fdisp_112x16_t));
    //flipdot_set_112x16(&d);
    protocolReplyPacket(PROTOCOL_MSG_ID_FPD_R16);
}
#endif

#endif // CFG_TYPE_FLIPDOT
