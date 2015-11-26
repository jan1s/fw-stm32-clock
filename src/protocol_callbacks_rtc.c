#include "platform_config.h"

#include "protocol.h"

#include "led.h"
#include "flipdot.h"
#include "rtc.h"
#include <time.h>
//#include "tz.h"

void protocolMsgPollCallbackTimUtc(void)
{
    time_t t = rtcGet();
    struct tm ts = *gmtime(&t);

    protocolMsgTimUtc_t utc;
    utc.nano = 0;
    utc.year = ts.tm_year + 1900;
    utc.month = ts.tm_mon - 1;
    utc.day = ts.tm_mday;
    utc.hour = ts.tm_hour;
    utc.min = ts.tm_min;
    utc.sec = ts.tm_sec;
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
    /*
    tzRule_t r;
    error_t e;

    e = tzGetSTD(&r);

    if (e)
    {
    	// answer with error
    }

    protocolMsgTimStd_t std;
    std.offset = r.offset;
    std.hour = r.hour;
    std.dow = r.dow;
    std.week = r.week;
    std.month = r.month;

    protocolMsgSendTimStd(&std);
    */
}

void protocolMsgPollCallbackTimDst(void)
{
    /*
    tzRule_t r;
    error_t e;

    e = tzGetDST(&r);

    if (e)
    {
    	// answer with error
    }

    protocolMsgTimDst_t dst;
    std.offset = r.offset;
    std.hour = r.hour;
    std.dow = r.dow;
    std.week = r.week;
    std.month = r.month;

    protocolMsgSendTimStd(&dst);
    */
}

void protocolMsgCallbackTimStd(protocolMsgTimStd_t *std)
{
    /*
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

    error_t e;
    tzRule_t r;

    e = tzCreateRule(std->offset, std->hour, std->dow, std->week, std->month, &r);
    if (e)
    {
    	// answer with error
    }

    tzSetSTD(&r);
    protocolMsgPollCallbackTimStd();
    */
}

void protocolMsgCallbackTimDst(protocolMsgTimDst_t *dst)
{
    /*
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

    error_t e;
    tzRule_t r;

    e = tzCreateRule(dst->offset, dst->hour, dst->dow, dst->week, dst->month, &r);
    if (e)
    {
    	// answer with error
    }

    tzSetDST(&r);
    protocolReplyPacket(PROTOCOL_MSG_ID_TIM_DST);
    */
}
