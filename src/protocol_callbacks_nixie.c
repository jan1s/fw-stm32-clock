#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE

#include "protocol.h"
#include "nixie.h"
#include "nixie_clock.h"
#include <string.h>

void protocolMsgCallbackNixTyp(protocolMsgNixTyp_t *typ)
{
    nixieMapping_t m = typ->type;
    nixieStoreMapping(m);
    nixieSetMapping(m);
    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_TYP);
}

void protocolMsgPollCallbackNixTyp(void)
{
    protocolMsgNixTyp_t typ;
    typ.type = nixieGetMapping();
    protocolMsgSendNixTyp(&typ);
}

void protocolMsgCallbackNixMod(protocolMsgNixMod_t *mod)
{
    nclock_mode_t m = mod->mode;
    nixieClockStoreMode(m);
    nixieClockSetMode(m);
    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_MOD);
}

void protocolMsgPollCallbackNixMod(void)
{
    protocolMsgNixMod_t mod;
    mod.mode = nixieClockGetMode();
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
