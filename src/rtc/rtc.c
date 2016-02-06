#include "platform_config.h"

#include "rtc/rtc.h"
#include "stm32f10x_conf.h"


typedef enum
{
    RTC_CFG_UNINITIALIZED = 0,
    RTC_CFG_INITIALIZED
} rtcConfig_t;


uint32_t rtcCounter = 0;
rtcConfig_t rtcConfig;

void RTC_IRQHandler(void);

void rtcConfiguration(void);
void rtcEnableInterrupt(void);
void rtcStoreConfiguration(rtcConfig_t cfg);
rtcConfig_t rtcLoadConfiguration(void);
void rtcSetConfiguration(rtcConfig_t cfg);
rtcConfig_t rtcGetConfiguration(void);

void rtcInit()
{
    rtcConfig = rtcLoadConfiguration();

    if (rtcConfig == RTC_CFG_INITIALIZED)
    {
        /* Enable calling the interrupt function */
        rtcEnableInterrupt();

        /* Check if the Power On Reset flag is set */
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {

        }
        /* Check if the Pin Reset flag is set */
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {

        }

        /* Wait for RTC registers synchronization */
        RTC_WaitForSynchro();

        /* Enable the RTC Second */
        RTC_ITConfig(RTC_IT_SEC, ENABLE);
        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }

    /* Clear reset flags */
    RCC_ClearFlag();
}

rtcConfig_t rtcLoadConfiguration(void)
{
    return BKP_ReadBackupRegister(BKP_DR1);
}

void rtcStoreConfiguration(rtcConfig_t cfg)
{
    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    BKP_WriteBackupRegister(BKP_DR1, cfg);

    /* Deny access to BKP Domain */
    PWR_BackupAccessCmd(DISABLE);
}

rtcConfig_t rtcGetConfiguration(void)
{
    return rtcConfig;
}

void rtcSetConfiguration(rtcConfig_t cfg)
{
    rtcConfig = cfg;
}


void rtcEnableInterrupt(void)
{
    /* NVIC configuration */
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void rtcConfiguration(void)
{
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    // BKP_DeInit();

    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);
    /* Wait till LSE is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}

uint32_t rtcGet(void)
{
    return rtcCounter;
}

void rtcSet(uint32_t t)
{
    /* Call the configuration method */
    rtcConfiguration();

    /* Write counter */
    RTC_SetCounter(t);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Store that the rtc was initialized */
    rtcStoreConfiguration(RTC_CFG_INITIALIZED);

    /* Reinitialize rtc */
    rtcInit();
}


/******************************************************************************/
/*            STM32F10x Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles RTC global interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);

        rtcCounter = RTC_GetCounter();

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
}

