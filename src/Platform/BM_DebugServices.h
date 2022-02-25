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

#ifndef SRC_BM_DEBUGSERVICES_H_
#define SRC_BM_DEBUGSERVICES_H_

#include "arm_math.h"

#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"

#include "BM_ErrorCodes.h"
#include "BM_ISR.h"

#define BM_DEBUG_PORT gpioPortC

typedef enum
{
  DBP0 = 7,
  DBP1 = 9,
  DBP2 = 10,
  DBP3 = 11,
}BM_DebugPin;

BM_Error  BM_DebugServices_init();
void      BM_DebugServices_set(BM_DebugPin debugPort);
void      BM_DebugServices_clear(BM_DebugPin debugPort);
void      BM_DebugServices_toggle(BM_DebugPin debugPort);
void      BM_DebugServices_startPerformanceTimer(int32_t startStop);
BM_Error  BM_DebugServices_getTimeElapsed(int32_t *out);

#ifndef HW_P1
BM_Error    BM_DebugServices_setAlertLED();
BM_Error    BM_DebugServices_clearAlertLED();
#endif


#endif /* SRC_BM_DEBUGSERVICES_H_ */
