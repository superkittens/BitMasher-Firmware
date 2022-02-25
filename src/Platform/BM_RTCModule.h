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

#pragma once

#include "em_rtcc.h"
#include "em_cmu.h"
#include "em_core.h"
#include "BM_ErrorCodes.h"
#include "BM_ISR.h"
#include "arm_math.h"

typedef struct
{
    int32_t hour;
    int32_t minute;
    int32_t second;
}BM_RTCModule_TimeHMS;

typedef struct
{
    int32_t hourT;
    int32_t hourU;
    int32_t minuteT;
    int32_t minuteU;
    int32_t secondT;
    int32_t secondU;
}BM_RTCModule_TimeBCD;


BM_Error        BM_RTCModule_init();

BM_Error        BM_RTCModule_setupCounter();
BM_Error        BM_RTCModule_startCounter();
BM_Error        BM_RTCModule_resetCounter();
const int32_t   BM_RTCModule_getCounterValue();

BM_Error        BM_RTCModule_setTime(const BM_RTCModule_TimeHMS time);
BM_Error        BM_RTCModule_getCurrentTimeBCD(BM_RTCModule_TimeBCD * const outputTime);
BM_Error        BM_RTCModule_getCurrentTimeHMS(BM_RTCModule_TimeHMS * const outputTime);
BM_Error        BM_RTCModule_convertHMSToBCD(const BM_RTCModule_TimeHMS hmsTime, BM_RTCModule_TimeBCD * const outputBCDTime);
BM_Error        BM_RTCModule_convertBCDToHMS(const BM_RTCModule_TimeBCD bcdTime, BM_RTCModule_TimeHMS * const outputHMSTime);

void            BM_RTCModule_isrRoutine();

