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


#include "BM_ClockTree.h"


static BM_Error BM_ClockTree_EnableCoreClocks()
{
    //  Start HFXO clock
    CMU_HFXOInit_TypeDef hfxoInit;
    hfxoInit.lowPowerMode = true;
    hfxoInit.autoStartEm01 = false;
    hfxoInit.autoSelEm01 = false;
    hfxoInit.autoStartSelOnRacWakeup = false;
    hfxoInit.ctuneStartup = _CMU_HFXOSTARTUPCTRL_CTUNE_DEFAULT;
    hfxoInit.ctuneSteadyState = 322U;            
    hfxoInit.regIshSteadyState = _CMU_HFXOSTEADYSTATECTRL_REGISH_DEFAULT;            
    hfxoInit.xoCoreBiasTrimStartup = 0x20;        
    hfxoInit.xoCoreBiasTrimSteadyState = 0x7;    
    hfxoInit.thresholdPeakDetect = 0x6;          
    hfxoInit.timeoutShuntOptimization = _CMU_HFXOTIMEOUTCTRL_SHUNTOPTTIMEOUT_DEFAULT;     
    hfxoInit.timeoutPeakDetect = 0xA;            
    hfxoInit.timeoutSteady = 0x4;               
    hfxoInit.timeoutStartup = _CMU_HFXOTIMEOUTCTRL_STARTUPTIMEOUT_DEFAULT;      
    hfxoInit.mode = cmuOscMode_Crystal;         

    CMU_HFXOInit(&hfxoInit);
    CMU_OscillatorEnable(cmuOsc_HFXO, true, true);
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
    // CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);

    return BM_NO_ERR;
}


static BM_Error BM_ClockTree_ConfigureLEClocks()
{
    //  Initialize LFXO used for I2S MCLK Generation and RTCC
    CMU_LFXOInit_TypeDef lfxoInit = CMU_LFXOINIT_DEFAULT;
    CMU_LFXOInit(&lfxoInit);

    CMU_ClockEnable(cmuClock_HFLE, true);
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
    CMU_ClockSelectSet(cmuClock_LFE, cmuSelect_LFXO);
    CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);

    CMU_ClockEnable(cmuClock_RTCC, true);

    return BM_NO_ERR;
}


static BM_Error BM_ClockTree_EnableLEPeripheralClocks()
{
    CMU_ClockEnable(cmuClock_LETIMER0, true);

    return BM_NO_ERR;
}


static BM_Error BM_ClockTree_EnablePeripheralClocks()
{
    CMU_ClockEnable(cmuClock_HFPER, true);

    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_LDMA, true);
    CMU_ClockEnable(cmuClock_USART1, true);     //  Reserved for display SPI
    CMU_ClockEnable(cmuClock_TIMER1, true);     //  Reserved for VSYNC timer
    CMU_ClockEnable(cmuClock_TIMER0, true);     //  Reserved for display signalling timer
    CMU_ClockEnable(cmuClock_WTIMER0, true);    //  Reserved for performance timer
    CMU_ClockEnable(cmuClock_TRNG0, true);

    return BM_NO_ERR;
}

/*
 *  Enable the system and peripheral clocks for BitMasher
 *  Clocks Needed:
 *      LETIMER0
 *      TIMER1
 * 
 *  Low Energy Peripheral Clocks use LFXO (32.768 kHz)
 */
BM_Error BM_ClockTree_EnableClocks()
{
    BM_ASSERT(BM_ClockTree_EnableCoreClocks());
    BM_ASSERT(BM_ClockTree_ConfigureLEClocks());
    BM_ASSERT(BM_ClockTree_EnablePeripheralClocks());
    BM_ASSERT(BM_ClockTree_EnableLEPeripheralClocks());

    return BM_NO_ERR;
}


/*
 *  Set the HF clock for low power (idle/sleep) modes
 *  HF bus is driven by HFRCO instead of HFXO
 *  Clock speed is set to 19 MHz (Allowing for VSCALE0 voltage scaling)
 * 
 *  Note that since the clock speed is changing, peripherals will be affected!
 *  Adjust peripheral settings as necessary before/after calling this function!
 */
BM_Error BM_ClockTree_enterLowPowerClocking()
{
    CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
    CMU_OscillatorEnable(cmuOsc_HFXO, false, false);

    return BM_NO_ERR;
}


/*
 *  Set the HF clock for full operation (active mode)
 *  HF bus is driven by HFXO
 *  Clock speed is set to 40 MHz
 * 
 *  Note that since the clock speed is changing, peripherals will be affected!
 *  Adjust peripheral settings as necessary before/after calling this function!
 */
BM_Error BM_ClockTree_enterFullPowerClocking()
{
    BM_ClockTree_EnableCoreClocks();

    return BM_NO_ERR;
}