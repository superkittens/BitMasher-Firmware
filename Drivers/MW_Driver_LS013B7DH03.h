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

#include "em_cmu.h"
#include "em_letimer.h"
#include "em_timer.h"
#include "em_usart.h"
#include "em_gpio.h"
#include "em_ldma.h"
#include "em_cmu.h"
#include "arm_math.h"
#include "BM_DebugServices.h"


#define MW_DRIVER_LD013V7DH03_NUM_LDMA_STRUCTS 4


typedef struct
{
    //  COMEXT (VCOM) Pin Information
    GPIO_Port_TypeDef   comExtPort;
    uint32_t            comExtPin;
    uint32_t            comExtLoc;

    //  Timers for VCOM and Signal timing
    TIMER_TypeDef       *signallingTimer;
    IRQn_Type           signallingTimerIRQnType;
    CMU_Clock_TypeDef   signallingTimerClock;
    LETIMER_TypeDef     *VCOMTimer;
    uint32_t            VCOMLETimerOutputPEN;

    //  SPI Module, Ports and Pins
    USART_TypeDef       *usartModule;
    uint32_t            spiSCKLoc;
    uint32_t            spiCOPILoc;
    uint32_t            spiCSLoc;

    GPIO_Port_TypeDef   spiSCKPort;
    GPIO_Port_TypeDef   spiCOPIPort;
    GPIO_Port_TypeDef   spiCSPort;

    uint32_t            spiSCKPin;
    uint32_t            spiCOPIPin;
    uint32_t            spiCSPin;

    int32_t             ldmaChannel;

    int32_t             _isInitialized;
    int32_t             _signallingTimerISRType;
    volatile int32_t    _timerExpired;
    volatile int32_t    _pixelTransferInProgress;

    volatile LDMA_TransferCfg_t     _ldmaCfg;
    volatile LDMA_Descriptor_t      _ldmaDesc[MW_DRIVER_LD013V7DH03_NUM_LDMA_STRUCTS];

    volatile uint8_t*               _frameBufferRow;
    volatile uint8_t                _displayLine;
}MW_LS013B7DH03_Params;


int32_t     MW_LS013B7DH03_init(MW_LS013B7DH03_Params *params);
int32_t     MW_LS013B7DH03_start(MW_LS013B7DH03_Params *params);
int32_t     MW_LS013B7DH03_stop(MW_LS013B7DH03_Params *params);
int32_t     MW_LS013B7DH03_drawAsync(MW_LS013B7DH03_Params *params, uint8_t *frameBuffer);
int32_t     MW_LS013B7DH03_drawBlocking(MW_LS013B7DH03_Params *params, uint8_t *frameBuffer);
int32_t     MW_LS013B7DH03_clearScreen(MW_LS013B7DH03_Params *params);

void        MW_LS013B7DH03_ldmaISR(volatile MW_LS013B7DH03_Params *params);
void        MW_LS013B7DH03_signallingTimerISR(MW_LS013B7DH03_Params * const params);