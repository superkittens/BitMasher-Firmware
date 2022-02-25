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


#include "BM_DebugServices.h"


static int32_t _timerOverflow;


/*
 *  The performance timer should NOT overflow
 *  If execution time lasts for more than 2^32 / fclk (about 108 seconds), this ISR will trigger
 *  and invalidate the execution time
 */
static void BM_DebugServices_WTimerOverflowISR()
{
  uint32_t flags = TIMER_IntGet(WTIMER0);
  TIMER_IntClear(WTIMER0, flags);

  _timerOverflow = 1;
  TIMER_Enable(WTIMER0, false);
}


BM_Error  BM_DebugServices_init()
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  
  GPIO_PinModeSet(BM_DEBUG_PORT, DBP0, gpioModePushPull, 0);
  GPIO_PinModeSet(BM_DEBUG_PORT, DBP1, gpioModePushPull, 0);
  GPIO_PinModeSet(BM_DEBUG_PORT, DBP2, gpioModePushPull, 0);
  GPIO_PinModeSet(BM_DEBUG_PORT, DBP3, gpioModePushPull, 0);

  //  Initialize performance timer
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;
  timerInit.enable = false;
  timerInit.oneShot = true;
  TIMER_Init(WTIMER0, &timerInit);

  _timerOverflow = 0;

  TIMER_IntEnable(WTIMER0, TIMER_IEN_OF);
  NVIC_EnableIRQ(WTIMER0_IRQn);

  BM_ISR_Timer_registerWTIMER0ISR(&BM_DebugServices_WTimerOverflowISR);

  return BM_NO_ERR;
}


void BM_DebugServices_set(BM_DebugPin debugPort)
{
  GPIO_PinOutSet(BM_DEBUG_PORT, debugPort);
}


void BM_DebugServices_clear(BM_DebugPin debugPort)
{
  GPIO_PinOutClear(BM_DEBUG_PORT, debugPort);
}


void BM_DebugServices_toggle(BM_DebugPin debugPort)
{
  GPIO_PinOutToggle(BM_DEBUG_PORT, debugPort);
}


/*
 *  Start a background timer that can measure execution time of a block of code
 *  Note that the timer (32-bit) has the potential to overflow which invalidates
 *  the measured time.
 * 
 *  Input Parameters:
 *    startStop:  0: Stop timer, 1: Start timer
 * 
 *  Returns:
 *    None
 */
void BM_DebugServices_startPerformanceTimer(int32_t startStop)
{
  if (startStop)
  {
    WTIMER0->CNT = 0;
    WTIMER0->TOP = 0xFFFFFFFF;
    _timerOverflow = 0;
    TIMER_Enable(WTIMER0, true);
  }
  else
    TIMER_Enable(WTIMER0, false);
}


/*
 *  Retrieve the elapsed time between startPerformanceTimer() calls in usec
 * 
 *  Input Parameters:
 *    out:  Pointer to variable that will hold the elapsed time
 * 
 *  Returns:
 *    BM_NO_ERR:          If nothing went wrong in the WTIMER0 count process
 *    BM_DEBUG_TIMER_OVF: If timer overflowed (out is set to -1) 
 */
BM_Error  BM_DebugServices_getTimeElapsed(int32_t *out)
{
  if (_timerOverflow)
  {
    *out = -1;
    return BM_DEBUG_TIMER_OVF;
  }

  const float32_t tickTime = 1.f / ((float32_t)CMU_ClockFreqGet(cmuClock_WTIMER0));
  const float32_t timeInUSec = (tickTime * (float32_t)WTIMER0->CNT) * 1000000.f;
  
  *out = (int32_t)timeInUSec;

  return BM_NO_ERR;
}


#ifndef HW_P1
BM_Error BM_DebugServices_setAlertLED()
{
  GPIO_PinModeSet(gpioPortF, 3, gpioModeWiredAnd, 0);
  return BM_NO_ERR;
}


BM_Error BM_DebugServices_clearAlertLED()
{
  GPIO_PinModeSet(gpioPortF, 3, gpioModeWiredAnd, 1);
  return BM_NO_ERR;
}
#endif