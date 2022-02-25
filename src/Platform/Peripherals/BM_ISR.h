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
//
//  This file consolidates every ISR used in BitMasher to one central location
//  ISR functions for a given module can be written in its respective source but it MUST be registered with the register functions listed here!
//  Note that the registered function SHOULD be as short as possible!
//  <Peripheral>_IRQHandler() MUST NOT BE DEFINED IN ANY OTHER SOURCE FILE OTHER THAN HERE!

#ifndef BM_ISR_H_
#define BM_ISR_H_

#include "arm_math.h"
#include "em_ldma.h"
#include "em_timer.h"
#include "em_rtcc.h"
#include "em_gpio.h"

#include "BM_Common.h"
#include "BM_ErrorCodes.h"

BM_Error    BM_ISR_LDMA_registerAudioISR(void (*audioISR)(void));
BM_Error    BM_ISR_LDMA_registerDisplayISR(void (*displayISR)(void));

BM_Error    BM_ISR_RTCC_registerRTCCISR(void (*rtccISR)(void));

BM_Error    BM_ISR_Timer_registerTIMER0ISR(void (*timer0ISR)(void));
BM_Error    BM_ISR_Timer_registerTIMER1ISR(void (*timer1ISR)(void));
BM_Error    BM_ISR_Timer_registerWTIMER0ISR(void (*wtimer0ISR)(void));

BM_Error    BM_ISR_GPIO_registerUserIOISR(void (*userIOISR)(void));

#endif