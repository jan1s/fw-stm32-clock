#include "platform_config.h"

#include "protocol.h"

#include "led.h"
#include "flipdot.h"
#include "rtc.h"
#include "tz.h"


void protocolMsgPollCallbackTimUtc(void)
{
    uint32_t epoch = rtcGet();

    rtcTime_t t;
    rtcCreateTimeFromEpoch(epoch, &t);

    protocolMsgTimUtc_t utc;
    utc.nano = 0;
    utc.year = t.years + 1900;
    utc.month = t.months;
    utc.day = t.days;
    utc.hour = t.hours;
    utc.min = t.minutes;
    utc.sec = t.seconds;
    utc.valid = true;

    protocolMsgSendTimUtc(&utc);
}

void protocolMsgCallbackTimUtc(protocolMsgTimUtc_t *utc)
{
    struct tm ts = {0};
    ts.tm_year = utc->year - 1900;
    ts.tm_mon = utc->month - 1;
    ts.tm_mday = utc->day;
    ts.tm_hour = utc->hour;
    ts.tm_min = utc->min;
    ts.tm_sec = utc->sec;

    uint32_t t = mktime(&ts);
    rtcSet(t);

    protocolReplyPacket(PROTOCOL_MSG_ID_TIM_UTC);
}

void protocolMsgPollCallbackTimStd(void)
{
    tzRule_t r;
    tzGetSTD(&r);

    protocolMsgTimStd_t std;
    std.offset = r.offset;
    std.hour = r.hour;
    std.dow = r.dow;
    std.week = r.week;
    std.month = r.month;

    protocolMsgSendTimStd(&std);
}

void protocolMsgPollCallbackTimDst(void)
{
    tzRule_t r;
    tzGetDST(&r);

    protocolMsgTimDst_t dst;
    dst.offset = r.offset;
    dst.hour = r.hour;
    dst.dow = r.dow;
    dst.week = r.week;
    dst.month = r.month;

    protocolMsgSendTimDst(&dst);
}

void protocolMsgCallbackTimStd(protocolMsgTimStd_t *std)
{
    if ((std->offset < -2000) || (std->offset > 2000))
    {
        // answer with error
    }
    if (std->hour > 23)
    {
        // answer with error
    }
    if (std->dow > 6)
    {
        // answer with error
    }
    if (std->week > 4)
    {
        // answer with error
    }
    if (std->month > 12)
    {
        // answer with error
    }

    tzRule_t r;
    tzCreateRule(std->offset, std->hour, std->dow, std->week, std->month, &r);

    tzStoreSTD(&r);
    tzSetSTD(&r);
    protocolReplyPacket(PROTOCOL_MSG_ID_TIM_STD);
}

void protocolMsgCallbackTimDst(protocolMsgTimDst_t *dst)
{
    if ((dst->offset < -2000) || (dst->offset > 2000))
    {
        // answer with error
    }
    if (dst->hour > 23)
    {
        // answer with error
    }
    if (dst->dow > 6)
    {
        // answer with error
    }
    if (dst->week > 4)
    {
        // answer with error
    }
    if (dst->month > 12)
    {
        // answer with error
    }

    tzRule_t r;
    tzCreateRule(dst->offset, dst->hour, dst->dow, dst->week, dst->month, &r);

    tzStoreDST(&r);
    tzSetDST(&r);
    protocolReplyPacket(PROTOCOL_MSG_ID_TIM_DST);
}
