//  Copyright 2021 Allen Lee
//
//  Author:  Allen Lee (alee@meoworkshop.org)
//  
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
//  For more information, please refer to https://opensource.org/licenses/mit-license.php
//
//  ------------------------------------------------------------------------------------------------  //

#include "BM_RTCModule.h"


#define BM_RTC_MODULE_LFXO_FREQ 32768
#define BM_RTC_MODULE_CC_CH     1

#define BM_RTC_UNLOCK_KEY       0xAEE8

volatile int32_t    _counter = 0;
volatile int32_t    _counterBeingModified = 0;


static BM_Error BM_RTCModule_validateHMSTime(const BM_RTCModule_TimeHMS hmsTime)
{
    if (hmsTime.hour < 0 || hmsTime.hour > 24)
        return BM_INVALID_TIME;

    if (hmsTime.minute < 0 || hmsTime.minute > 59)
        return BM_INVALID_TIME;

    if (hmsTime.second < 0 || hmsTime.second > 59)
        return BM_INVALID_TIME;

    return BM_NO_ERR;
}

static BM_Error BM_RTCModule_validateBCDTime(const BM_RTCModule_TimeBCD bcdTime)
{
    if (bcdTime.hourT < 0 || bcdTime.hourT > 2) return BM_INVALID_TIME;
    if (bcdTime.hourU < 0 || bcdTime.hourU > 9) return BM_INVALID_TIME;
    if (bcdTime.minuteT < 0 || bcdTime.minuteT > 5) return BM_INVALID_TIME;
    if (bcdTime.minuteU < 0 || bcdTime.minuteU > 9) return BM_INVALID_TIME;
    if (bcdTime.secondT < 0 || bcdTime.secondT > 5) return BM_INVALID_TIME;
    if (bcdTime.secondU < 0 || bcdTime.secondU > 9) return BM_INVALID_TIME;

    return BM_NO_ERR;
}


BM_Error BM_RTCModule_init()
{
    uint32_t lfxoFreq = CMU_ClockFreqGet(cmuClock_RTCC);
    if (lfxoFreq != BM_RTC_MODULE_LFXO_FREQ)
        return BM_RTC_INVALID_LFXO_FREQ;

    _counter = 0;

    RTCC_Init_TypeDef rtccInit = RTCC_INIT_DEFAULT;
    rtccInit.enable = false;
    rtccInit.presc = rtccCntPresc_32768;
    rtccInit.cntMode = rtccCntModeCalendar;
    rtccInit.cntWrapOnCCV1 = false;

    RTCC_CCChConf_TypeDef compare = RTCC_CH_INIT_COMPARE_DEFAULT;

    RTCC_Init(&rtccInit);

    //  Setup an initial time and date (should be changed through the functions BM_RTCModule_setTime/Date)
    RTCC_DateSet(0);
    RTCC_TimeSet(0x101500);

    //  Initialize interrupts to occur every minute (to update the counter)
    RTCC_ChannelInit(BM_RTC_MODULE_CC_CH, &compare);
    RTCC_ChannelTimeSet(BM_RTC_MODULE_CC_CH, 0x00000100);    //  Set interupt to trigger when the time is 00:01:00

    //  Set COMPMASK (we don't care about the month, day, hour or the 10s digit of the minutes)
    RTCC->CC[1].CTRL |= (20 << 12);

    RTCC_IntEnable(RTCC_IEN_CC1);
    NVIC_ClearPendingIRQ(RTCC_IRQn);
    NVIC_EnableIRQ(RTCC_IRQn);

    BM_ISR_RTCC_registerRTCCISR(BM_RTCModule_isrRoutine);

    RTCC_Enable(true);

    return BM_NO_ERR;
}


BM_Error BM_RTCModule_resetCounter()
{
    CORE_ATOMIC_SECTION(_counter = 0;);

    return BM_NO_ERR;
}


const int32_t BM_RTCModule_getCounterValue()
{
    return _counter;
}


BM_Error BM_RTCModule_setTime(const BM_RTCModule_TimeHMS hmsTime)
{
    BM_RTCModule_TimeBCD bcdTime;
    BM_Error error = BM_RTCModule_convertHMSToBCD(hmsTime, &bcdTime);
    if (error != BM_NO_ERR)
        return error;
    
    //  Pack the BCD time into a uint32_t variable that we can upload to the MCU register
    const uint32_t BCD_TIME =   bcdTime.hourT << 20 |
                                bcdTime.hourU << 16 |
                                bcdTime.minuteT << 12 |
                                bcdTime.minuteU << 8 |
                                bcdTime.secondT << 4 |
                                bcdTime.secondU;

    //  Disable RTCC interrupts for now
    NVIC_DisableIRQ(RTCC_IRQn);

    //  Unlock the RTCC_TIME register so we can write to it
    // RTCC_Unlock();
    RTCC_TimeSet(BCD_TIME);
    // RTCC_Lock();

    //  Set the counter interrupt value
    uint32_t currentTime = RTCC_TimeGet();
    currentTime &= 0x00000FFF;
    currentTime = currentTime >> 8;

    uint32_t nextInterruptTime = currentTime + 1;
    nextInterruptTime = nextInterruptTime << 8;

    if (currentTime == 9)
        nextInterruptTime = 0;

    RTCC_ChannelTimeSet(BM_RTC_MODULE_CC_CH, nextInterruptTime);

    NVIC_EnableIRQ(RTCC_IRQn);

    return BM_NO_ERR;
}


BM_Error BM_RTCModule_getCurrentTimeBCD(BM_RTCModule_TimeBCD * const outputTime)
{
    if (outputTime == NULL)
        return BM_NULLPTR;

    const uint32_t time = RTCC_TimeGet();

    //  The value returned from RTCC_TimeGet will have some gaps
    //  To pack the T/U values, we will need to do a series of bit shifts and masking
    //  Refer to the EFM32 reference manual (RTCC_TIME register) for the actual mask and shift values
    outputTime->hourT = (time & (0x03 << 20)) >> 20;
    outputTime->hourU = (time & (0x0F << 16)) >> 16;
    outputTime->minuteT = (time & (0x07 << 12)) >> 12;
    outputTime->minuteU = (time & (0x0F << 8)) >> 8;
    outputTime->secondT = (time & ( 0x07 << 4)) >> 4;
    outputTime->secondU = time & 0x0F;

    return BM_NO_ERR;
}


BM_Error BM_RTCModule_getCurrentTimeHMS(BM_RTCModule_TimeHMS * const outputTime)
{
    if (outputTime == NULL)
        return BM_NULLPTR;

    BM_RTCModule_TimeBCD bcdTime;
    BM_Error error = BM_RTCModule_getCurrentTimeBCD(&bcdTime);
    if (error != BM_NO_ERR)
        return error;

    error = BM_RTCModule_convertBCDToHMS(bcdTime, outputTime);
    if (error != BM_NO_ERR)
        return error;

    return BM_NO_ERR;
}


BM_Error BM_RTCModule_convertHMSToBCD(const BM_RTCModule_TimeHMS hmsTime, BM_RTCModule_TimeBCD * const outputBCDTime)
{
    if (outputBCDTime == NULL)
        return BM_NULLPTR;

    if (BM_RTCModule_validateHMSTime(hmsTime) != BM_NO_ERR)
        return BM_INVALID_TIME;

    outputBCDTime->hourT = hmsTime.hour / 10;
    outputBCDTime->hourU = hmsTime.hour % 10;
    outputBCDTime->minuteT = hmsTime.minute / 10;
    outputBCDTime->minuteU = hmsTime.minute % 10;
    outputBCDTime->secondT = hmsTime.second / 10;
    outputBCDTime->secondU = hmsTime.second % 10;

    return BM_NO_ERR;
}


BM_Error BM_RTCModule_convertBCDToHMS(const BM_RTCModule_TimeBCD bcdTime, BM_RTCModule_TimeHMS * const outputHMSTime)
{
    if (outputHMSTime == NULL)
        return BM_NULLPTR;

    if (BM_RTCModule_validateBCDTime(bcdTime) != BM_NO_ERR)
        return BM_INVALID_TIME;

    outputHMSTime->hour = (bcdTime.hourT * 10) + bcdTime.hourU;
    outputHMSTime->minute = (bcdTime.minuteT * 10) + bcdTime.minuteU;
    outputHMSTime->second = (bcdTime.secondT * 10) + bcdTime.secondU;

    return BM_NO_ERR;
}


//  ISR routine for counter (executes every minute)
void BM_RTCModule_isrRoutine()
{
    RTCC_IntClear(RTCC_IFC_CC1);
    
    //  TODO:  Make sure this is thread safe
    _counter++;

    //  Set the next time that the interrupt should trigger
    uint32_t currentTime = RTCC_TimeGet();
    currentTime &= 0x00000FFF;
    currentTime = currentTime >> 8;

    uint32_t nextInterruptTime = currentTime + 1;
    nextInterruptTime = nextInterruptTime << 8;

    if (currentTime == 9)
        nextInterruptTime = 0;

    RTCC_ChannelTimeSet(BM_RTC_MODULE_CC_CH, nextInterruptTime);
}