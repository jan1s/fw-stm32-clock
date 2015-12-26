#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "platform_config.h"
#include <stdbool.h>

#define PROTOCOL_SYNC_0 0xB5
#define PROTOCOL_SYNC_1 0x62
#define PROTOCOL_HEADER_SIZE 0x06
#define PROTOCOL_PAYLOAD_SIZE 0x5FF

#pragma pack(1)
typedef struct
{
    uint8_t sync[2];
    uint16_t msgId;
    uint16_t payloadLength;
    uint8_t payload[PROTOCOL_PAYLOAD_SIZE];
    uint16_t checksum;
} protocolPacket_t;

void protocolInit(void);
void protocolPoll(void);

void protocolSendChar(uint8_t c);
void protocolSendPacket(protocolPacket_t *packet);
void protocolReplyPacket(uint16_t msgId);
bool protocolGetPacket(protocolPacket_t *packet);
bool protocolCheckPacket(const protocolPacket_t *packet);
bool protocolEvaluatePacket(protocolPacket_t *packet);
uint16_t protocolCalculateChecksum(const protocolPacket_t *packet);


#define PROTOCOL_MSG_ID_CFG_SAV 0x0101
#define PROTOCOL_MSG_ID_CFG_MSG 0x0102

#define PROTOCOL_MSG_ID_TIM_UTC 0x0201
#define PROTOCOL_MSG_ID_TIM_STD 0x0202
#define PROTOCOL_MSG_ID_TIM_DST 0x0203
#define PROTOCOL_MSG_ID_TIM_SRC 0x0204

#define PROTOCOL_MSG_ID_NIX_TYP 0x0801
#define PROTOCOL_MSG_ID_NIX_MOD 0x0802
#define PROTOCOL_MSG_ID_NIX_TST 0x0803
#define PROTOCOL_MSG_ID_NIX_RAW 0x0804

#define PROTOCOL_MSG_ID_FPD_TYP 0x0901
#define PROTOCOL_MSG_ID_FPD_MOD 0x0902
#define PROTOCOL_MSG_ID_FPD_TST 0x0903
#define PROTOCOL_MSG_ID_FPD_R12 0x0904
#define PROTOCOL_MSG_ID_FPD_R16 0x0905

typedef struct
{
    uint32_t save;
    uint32_t load;
    uint32_t clear;
} protocolMsgCfgSav_t;

typedef struct
{
    int32_t nano;
    int16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t valid;
} protocolMsgTimUtc_t;

typedef struct
{
    uint16_t offset;
    uint8_t hour;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
} protocolMsgTimStd_t;

typedef struct
{
    uint16_t offset;
    uint8_t hour;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
} protocolMsgTimDst_t;

typedef struct
{
    uint32_t src;
    uint8_t mode;
} protocolMsgTimSrc_t;


typedef struct
{
    uint8_t type;
} protocolMsgNixTyp_t;

typedef struct
{
    uint8_t mode;
} protocolMsgNixMod_t;

typedef struct
{
    uint8_t test;
} protocolMsgNixTst_t;

typedef struct
{
    uint8_t raw[6];
} protocolMsgNixR4T_t;

typedef struct
{
    uint8_t raw[8];
} protocolMsgNixR6T_t;


typedef struct
{
    uint8_t type;
} protocolMsgFpdTyp_t;

typedef struct
{
    uint8_t mode;
} protocolMsgFpdMod_t;

typedef struct
{
    uint8_t test;
} protocolMsgFpdTst_t;

typedef struct
{
    uint8_t raw[84];
} protocolMsgFpdR12_t;

typedef struct
{
    uint16_t raw[112];
} protocolMsgFpdR16_t;



void protocolMsgSendTimUtc(protocolMsgTimUtc_t *msg);
void protocolMsgSendTimStd(protocolMsgTimStd_t *msg);
void protocolMsgSendTimDst(protocolMsgTimDst_t *msg);
void protocolMsgSendTimSrc(protocolMsgTimSrc_t *msg);

void protocolMsgPollCallbackTimUtc(void);
void protocolMsgCallbackTimUtc(protocolMsgTimUtc_t *utc);
void protocolMsgPollCallbackTimStd(void);
void protocolMsgCallbackTimStd(protocolMsgTimStd_t *std);
void protocolMsgPollCallbackTimDst(void);
void protocolMsgCallbackTimDst(protocolMsgTimDst_t *dst);
void protocolMsgPollCallbackTimSrc(void);
void protocolMsgCallbackTimSrc(protocolMsgTimSrc_t *src);


void protocolMsgSendNixTyp(protocolMsgNixTyp_t *msg);
void protocolMsgSendNixMod(protocolMsgNixMod_t *msg);
void protocolMsgSendNixTst(protocolMsgNixTst_t *msg);
void protocolMsgSendNixR4T(protocolMsgNixR4T_t *msg);
void protocolMsgSendNixR6T(protocolMsgNixR6T_t *msg);

void protocolMsgPollCallbackNixTyp(void);
void protocolMsgCallbackNixTyp(protocolMsgNixTyp_t *typ);
void protocolMsgPollCallbackNixMod(void);
void protocolMsgCallbackNixMod(protocolMsgNixMod_t *mod);
void protocolMsgPollCallbackNixTst(void);
void protocolMsgCallbackNixTst(protocolMsgNixTst_t *tst);
void protocolMsgPollCallbackNixR4T(void);
void protocolMsgCallbackNixR4T(protocolMsgNixR4T_t *r4);
void protocolMsgPollCallbackNixR6T(void);
void protocolMsgCallbackNixR6T(protocolMsgNixR6T_t *r6);


void protocolMsgSendFpdTyp(protocolMsgFpdTyp_t *msg);
void protocolMsgSendFpdMod(protocolMsgFpdMod_t *msg);
void protocolMsgSendFpdTst(protocolMsgFpdTst_t *msg);
void protocolMsgSendFpdR12(protocolMsgFpdR12_t *msg);
void protocolMsgSendFpdR16(protocolMsgFpdR16_t *msg);

void protocolMsgCallbackFpdTyp(protocolMsgFpdTyp_t *msg);
void protocolMsgCallbackFpdMod(protocolMsgFpdMod_t *msg);
void protocolMsgCallbackFpdTst(protocolMsgFpdTst_t *msg);
void protocolMsgCallbackFpdR12(protocolMsgFpdR12_t *msg);
void protocolMsgCallbackFpdR16(protocolMsgFpdR16_t *msg);

#endif
