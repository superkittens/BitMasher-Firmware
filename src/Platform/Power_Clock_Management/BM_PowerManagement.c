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

#include "BM_PowerManagement.h"


BM_Error BM_Power_init()
{
    GPIO_PinModeSet(gpioPortD, 8, gpioModePushPull, 0);
    BM_Power_enable3V0(1);

    return BM_NO_ERR;
}


BM_Error BM_Power_enable3V0(const int32_t enable)
{
    if ((enable > 1) || (enable < 0))
        return BM_INVALID_PARAM;
    
    if (enable)
        GPIO_PinOutSet(gpioPortD, 8);

    else
        GPIO_PinOutClear(gpioPortD, 8);
    
    return BM_NO_ERR;
}


BM_Error BM_Power_enterFullPower()
{
    BM_RE_stopRenderEngine();
    EMU_VScaleEM01(emuVScaleEM01_HighPerformance, true);
    BM_ClockTree_enterFullPowerClocking();
    BM_AE_setAudioBypass(0);
    BM_AE_startAudioEngine();
    BM_RE_startRenderEngine();
}


BM_Error BM_Power_enterIdlePower()
{
    BM_AE_setAudioBypass(1);
    BM_AE_stopAudioEngine();
    BM_AE_clearBuffers();
    BM_RE_stopRenderEngine();
    BM_ClockTree_enterLowPowerClocking();
    EMU_VScaleEM01(emuVScaleEM01_LowPower, true);

    BM_RE_startRenderEngine();
}


BM_Error BM_Power_setupForSleep()
{
    BM_AE_setCodecActive(0);
    BM_AE_stopAudioEngine();
    BM_AE_clearBuffers();
    // BM_ClockTree_enterLowPowerClocking();
    // EMU_VScaleEM01(emuVScaleEM01_LowPower, true);
}


BM_Error BM_Power_exitSleep()
{
    EMU_VScaleEM01(emuVScaleEM01_HighPerformance, true);
    BM_ClockTree_enterFullPowerClocking();
    BM_AE_setCodecActive(1);
}