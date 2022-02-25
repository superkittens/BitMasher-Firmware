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

#include "BM_ISR.h"

static void (*_audioISR)(void);
static void (*_displayISR)(void);

static void (*_rtccISR)(void);

static void (*_timer0ISR)(void);
static void (*_timer1ISR)(void);
static void (*_wtimer0ISR)(void);

static void (*_userIOISR)(void);


BM_Error BM_ISR_LDMA_registerAudioISR(void (*audioISR)(void))
{
    if (audioISR == NULL)
        return BM_NULLPTR;

    _audioISR = audioISR;

    return BM_NO_ERR;
}


BM_Error BM_ISR_LDMA_registerDisplayISR(void (*displayISR)(void))
{
    if (displayISR == NULL)
        return BM_NULLPTR;

    _displayISR = displayISR;

    return BM_NO_ERR;
}


BM_Error BM_ISR_RTCC_registerRTCCISR(void (*rtccISR)(void))
{
    if (rtccISR == NULL)
        return BM_NULLPTR;

    _rtccISR = rtccISR;

    return BM_NO_ERR;
}


BM_Error BM_ISR_Timer_registerTIMER0ISR(void (*timer0ISR)(void))
{
    if (timer0ISR == NULL)
        return BM_NULLPTR;

    _timer0ISR = timer0ISR;

    return BM_NO_ERR;
}


BM_Error BM_ISR_Timer_registerTIMER1ISR(void (*timer1ISR)(void))
{
    if (timer1ISR == NULL)
        return BM_NULLPTR;

    _timer1ISR = timer1ISR;

    return BM_NO_ERR;
}


BM_Error BM_ISR_Timer_registerWTIMER0ISR(void (*wtimer0ISR)(void))
{
    if (wtimer0ISR == NULL)
        return BM_NO_ERR;

    _wtimer0ISR = wtimer0ISR;

    return BM_NO_ERR;
}


BM_Error BM_ISR_GPIO_registerUserIOISR(void (*userIOISR)(void))
{
    if (userIOISR == NULL)
        return BM_NULLPTR;

    _userIOISR = userIOISR;

    return BM_NO_ERR;
}





//  IRQHandler Definitions
//  ------------------------------------------------------------------------------------------------  //

void LDMA_IRQHandler()
{
    uint32_t ldmaFlag = LDMA->IF;

    if (ldmaFlag & (1 << 0))
        _audioISR();
    
    if (ldmaFlag & (1 << 1))
        _audioISR();

    if (ldmaFlag & (1 << BM_LDMA_CH_DISP))
        _displayISR();
}


void RTCC_IRQHandler()
{
    _rtccISR();
}


void TIMER0_IRQHandler()
{
    _timer0ISR();
}


void TIMER1_IRQHandler()
{
    _timer1ISR();
}


void WTIMER0_IRQHandler()
{
    _wtimer0ISR();
}


void GPIO_EVEN_IRQHandler()
{
    _userIOISR();
}


void GPIO_ODD_IRQHandler()
{
    _userIOISR();
}