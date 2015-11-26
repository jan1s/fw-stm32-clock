//
//
//
//

#include "platform_config.h"

#ifdef CFG_TYPE_FLIPDOT

#include "flipdot.h"
#include "flipdot_clock.h"

#include <string.h>
#include <time.h>

#include "timer.h"

const char *wdays[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
const char *month[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

// ----------------------------------------------------------------------------

void flipdotClockInit()
{
#ifdef CFG_TYPE_FLIPDOT_84X7
    flipdotType = fdisp_type_84x7;
    flipdot_wipe_84x7(0);
    fdisp_84x7_t d;
    const uint8_t shallo[] = "   n3rdh3rd   ";
    flipdot_setstring_84x7(&d, shallo, sizeof(shallo));
    flipdot_set_84x7(&d);
#endif

#ifdef CFG_TYPE_FLIPDOT_112X16
    flipdot_wipe_112x16(0);
    fdisp_112x16_t d = {0};
    const uint8_t shallo[] = "Flipdot Clock";
    flipdot_setstring_112x16(&d, shallo, sizeof(shallo));
    flipdot_set_112x16(&d);
#endif

    timer_sleep(400000);
}

void flipdotClockShowTime(uint32_t t)
{
    struct tm ts = {0};
    ts = *gmtime(&t);

#ifdef CFG_TYPE_FLIPDOT_84X7
    char buffer[14] = {0};
    sprintf(buffer, "%2d.%s. %02d:%02d", ts.tm_mday, (uint8_t*)month[ts.tm_mon], ts.tm_hour, ts.tm_min);

    fdisp_84x7_t d;
    flipdot_setstring_84x7(&d, (uint8_t*)buffer, sizeof(buffer));
    flipdot_set_84x7(&d);
#endif

#ifdef CFG_TYPE_FLIPDOT_112X16

#endif
}

#endif // CFG_TYPE_FLIPDOT

// ----------------------------------------------------------------------------
