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

#include "BM_Controller_Sleep.h"

#define BM_CONTROLLER_SLEEP_TIME_YPOS 40


static BM_SystemState *_currentSystemState = NULL;

volatile int32_t BM_Controller_Sleep_wake = 0;



static int32_t BM_Controller_Sleep_mainLoop()
{
    while (*_currentSystemState == BM_STATE_SLEEP)
    {
        BM_RE_drawLightBackground();
        
        BM_RTCModule_TimeBCD time;
        BM_RTCModule_getCurrentTimeBCD(&time);

        BM_RE_drawTime(time, BM_CONTROLLER_SLEEP_TIME_YPOS);

        BM_RE_updateDisplay(false);

        EMU_EnterEM2(true);
        if (BM_Controller_Sleep_wake)
        {
            BM_Controller_Sleep_wake = 0;
            *(_currentSystemState) = BM_STATE_ACTIVE;
        }

    }

    return 1;
}


BM_Error BM_Controller_Sleep_init()
{

    return BM_NO_ERR;
}


BM_Error BM_Controller_Sleep_enter(BM_SystemState *systemState)
{
    BM_Power_setupForSleep();

    BM_Error error = BM_UserIO_setInterrupts(1);
    if (error != BM_NO_ERR)
        return error;

    
    _currentSystemState = systemState;
    BM_Controller_Sleep_wake = 0;

    BM_RE_init(FRAME_RATE);

    BM_Controller_Sleep_mainLoop();

    //  When exiting sleep mode, GPIO interrupts should be disabled
    //  Audio codec should be taken out of reset
    // BM_AE_setCodecActive(1);
    // BM_Power_exitSleep();
    BM_UserIO_setInterrupts(0);

    return BM_NO_ERR;
}