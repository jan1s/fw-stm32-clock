#include "platform_config.h"

#ifdef CFG_TYPE_NIXIE

#include "protocol.h"
#include "nixie.h"
#include "nixie_clock.h"

#include "timer.h"
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
    for(uint8_t i = 0; i < 10; ++i)
    {
        nixieDisplay4t_t d;
        d.digits[3] = i;
        d.digits[2] = i;
        d.digits[1] = i;
        d.digits[0] = i;
        d.dots[0] = i % 2;
        d.dots[1] = (i + 1) % 2;
        nixieDisplay4t(&d);
        timer_sleep(100000);
    }

    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_TST);
}

void protocolMsgCallbackNixR4T(protocolMsgNixR4T_t *r4t)
{

}
#endif

#ifdef CFG_TYPE_NIXIE_6T
void protocolMsgCallbackNixTst(protocolMsgNixTst_t *tst)
{
    for(uint8_t i = 0; i < 10; ++i)
    {
        nixieDisplay4t_t d;
        d.digits[5] = i;
        d.digits[4] = i;
        d.digits[3] = i;
        d.digits[2] = i;
        d.digits[1] = i;
        d.digits[0] = i;
        d.dots[0] = i % 2;
        d.dots[1] = (i + 1) % 2;
        d.dots[2] = i % 2;
        d.dots[3] = (i + 1) % 2;
        nixieDisplay4t(&d);
        timer_sleep(100000);
    }

    protocolReplyPacket(PROTOCOL_MSG_ID_NIX_TST);
}

void protocolMsgCallbackNixR6T(protocolMsgFpdR16_t *r6t)
{

}
#endif

#endif // CFG_TYPE_NIXIE
