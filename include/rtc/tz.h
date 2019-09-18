#ifndef TZ_H_
#define TZ_H_

#include "platform_config.h"
#include "rtc_functions.h"

typedef enum
{
    TZ_WEEK_LAST = 0,
    TZ_WEEK_FIRST,
    TZ_WEEK_SECOND,
    TZ_WEEK_THIRD,
    TZ_WEEK_FOURTH
} tzWeek_t;

typedef struct tzRule_st
{
    uint8_t hour;
    uint8_t dow;
    uint8_t week;    /**< Zero-based, first day of week = Monday (use rtcWeekdays_t) */
    uint8_t month;   /**< One-based (use rtcMonths_t) */
    int16_t offset;
} tzRule_t;


void   tzInit( void );
void   tzCreateRule( uint16_t offset, uint8_t hour, rtcWeekdays_t dow, uint8_t week, rtcMonths_t month, tzRule_t *r );
void   tzSetSTD( tzRule_t *std );
void   tzGetSTD( tzRule_t *std );
void   tzStoreSTD( tzRule_t *std );
void   tzLoadSTD( tzRule_t *std );
void   tzSetDST( tzRule_t *dst );
void   tzGetDST( tzRule_t *dst );
void   tzStoreDST( tzRule_t *dst );
void   tzLoadDST( tzRule_t *dst );
void   tzCalcStartTimes( uint16_t year );
void   tzRuleToTime( tzRule_t *r, uint16_t year, rtcTime_t *t );
void   tzUTCToLocal( rtcTime_t *utc, rtcTime_t *local );
void   tzLocalToUTC( rtcTime_t *local, rtcTime_t *utc );

bool tzUTCIsDST( rtcTime_t *utc );
bool tzLocalIsDST( rtcTime_t *local );

#endif
