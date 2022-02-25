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


#include "MW_Driver_LS013B7DH03.h"


#define MW_DRIVER_LD013V7DH03_VCOM_FREQ             60.f
#define MW_DRIVER_LD013V7DH03_LFXO_FREQ             32768.f
#define MW_DRIVER_LD013V7DH03_VCOM_TIMER_COMP0TOP   (MW_DRIVER_LD013V7DH03_LFXO_FREQ  / (MW_DRIVER_LD013V7DH03_VCOM_FREQ * 2))

#define MW_DRIVER_LD013V7DH03_SCK_FREQ              1000000

#define MW_DRIVER_LD013V7DH03_WRITE_CMD             0x01

#define MW_DRIVER_LD013V7DH03_FRAME_BUF_SIZE        2048
#define MW_DRIVER_LD013V7DH03_NUM_LINES             128
#define MW_DRIVER_LD013V7DH03_NUM_BYTES_PER_LINE    16
#define MW_DRIVER_LD013V7DH03_CMD_LENGTH            1

#define MW_DRIVER_LD013V7DH03_LINE_PKT_LENGTH       18
#define MW_DRIVER_LD013V7DH03_ADDR_OFFSET           1
#define MW_DRIVER_LD013V7DH03_DATA_OFFSET           2

#define MW_DRIVER_LD013V7DH03_ADDR_START            1

#define MW_DRIVER_LD013V7DH03_LDMA_CMD_INDEX        0
#define MW_DRIVER_LD013V7DH03_LDMA_ADDR_INDEX       1
#define MW_DRIVER_LD013V7DH03_LDMA_PIXELS_INDEX     2
#define MW_DRIVER_LD013V7DH03_LDMA_TRAILING_INDEX   3

#define MW_DRIVER_LD013V7DH03_TSSCS_TIME_S          0.000006f
#define MW_DRIVER_LD013V7DH03_THSCS_TIME_S          0.000002f

#define MW_DRIVER_LD013V7DH03_SET_CS_HIGH()         (GPIO_PinOutSet(params->spiCSPort, params->spiCSPin))
#define MW_DRIVER_LD013V7DH03_SET_CS_LOW()          (GPIO_PinOutClear(params->spiCSPort, params->spiCSPin))


static uint8_t              _CMD = MW_DRIVER_LD013V7DH03_WRITE_CMD;
static uint8_t              _TRAILING = 0x00;

static float32_t            _TSSCS_COUNTER_VAL;
static float32_t            _THSCS_COUNTER_VAL;


enum {
    SIGNALLING_TIMER_ISR_TYPE_BLOCKING = 0,
    SIGNALLING_TIMER_ISR_TYPE_ASYNC_START,
    SIGNALLING_TIMER_ISR_TYPE_ASYNC_END,
};




static void MW_LS013B7DH03_setupLETimer(MW_LS013B7DH03_Params *params)
{
    //  Initialize GPIO and LETIMER for COMEXT (VCOM) signal
    //  VCOM is used to remove any DC bias that forms over time in the display pixels
    //  https://www.sharpsde.com/fileadmin/products/Displays/2016_SDE_App_Note_for_Memory_LCD_programming_V1.3.pdf
    GPIO_PinModeSet(params->comExtPort,
                    params->comExtPin,
                    gpioModePushPull,
                    0);

    LETIMER_Init_TypeDef LETimerInit = LETIMER_INIT_DEFAULT;

    LETimerInit.enable = false;
    LETimerInit.comp0Top = true;
    LETimerInit.ufoa0 = letimerUFOAToggle;
    LETimerInit.repMode = letimerRepeatFree;
    LETimerInit.topValue = (uint32_t)MW_DRIVER_LD013V7DH03_VCOM_TIMER_COMP0TOP;

    //  In order for the pin to toggle, REP must not be 0
    LETIMER_RepeatSet(params->VCOMTimer, 0, 1);

    LETIMER_Init(params->VCOMTimer, &LETimerInit);

    //  Assign the right pins to the LETimer_OUT0
    params->VCOMTimer->ROUTEPEN |= params->VCOMLETimerOutputPEN;
    params->VCOMTimer->ROUTELOC0 |= params->comExtLoc;
}


/*
 *  The signalling timer is used to ensure that the timing between CS and SCK are adequate
 *  CS Rising Edge - SCK Rising Edge => 6 usec
 *  CS Falling Edge - LAST SCK Rising Edge => 2 usec
 */
static void MW_LS013B7DH03_setupSignallingTimer(MW_LS013B7DH03_Params *params)
{
    TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

    timerInit.enable = false;
    timerInit.oneShot = true;
    timerInit.prescale = timerPrescale1;

    TIMER_Init(params->signallingTimer, &timerInit);

    //  Calculate the counter value needed for TSSCS (6 usec)
    uint32_t timerClkFreq = CMU_ClockFreqGet(params->signallingTimerClock);
    _TSSCS_COUNTER_VAL = MW_DRIVER_LD013V7DH03_TSSCS_TIME_S * (float32_t)timerClkFreq;
    _THSCS_COUNTER_VAL = MW_DRIVER_LD013V7DH03_THSCS_TIME_S * (float32_t)timerClkFreq;

    NVIC_EnableIRQ(params->signallingTimerIRQnType);
    TIMER_IntEnable(params->signallingTimer, TIMER_IF_OF);
}


static void MW_LS013B7DH03_setupSPI(MW_LS013B7DH03_Params *params)
{
    GPIO_PinModeSet(params->spiSCKPort,
                    params->spiSCKPin,
                    gpioModePushPull,
                    0);

    GPIO_PinModeSet(params->spiCOPIPort,
                    params->spiCOPIPin,
                    gpioModePushPull,
                    0);

    GPIO_PinModeSet(params->spiCSPort,
                    params->spiCSPin,
                    gpioModePushPull,
                    0);

    USART_InitSync_TypeDef config = USART_INITSYNC_DEFAULT;
    config.enable = usartDisable;
    config.master = true;
    config.baudrate = MW_DRIVER_LD013V7DH03_SCK_FREQ;
    config.autoCsEnable = false;
    config.clockMode = usartClockMode0;
    config.msbf = false;

    USART_InitSync(params->usartModule, &config);

    params->usartModule->ROUTELOC0 = params->spiSCKLoc | params->spiCOPILoc;
    params->usartModule->ROUTEPEN = USART_ROUTEPEN_CLKPEN | USART_ROUTEPEN_TXPEN;
}


void MW_LS013B7DH03_setupLDMA(MW_LS013B7DH03_Params *params)
{
    params->_ldmaCfg = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART1_TXEMPTY);
    
    for (int32_t i = 0; i < MW_DRIVER_LD013V7DH03_NUM_LDMA_STRUCTS; ++i)
    {
        params->_ldmaDesc[i] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(
                                                                                    &_CMD,
                                                                                    &USART1->TXDATA,
                                                                                    MW_DRIVER_LD013V7DH03_CMD_LENGTH,
                                                                                    1);

        params->_ldmaDesc[i].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
        params->_ldmaDesc[i].xfer.ignoreSrec = true;
        params->_ldmaDesc[i].xfer.size = ldmaCtrlSizeByte;
        params->_ldmaDesc[i].xfer.doneIfs = false;
    }

    params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_PIXELS_INDEX].xfer.xferCnt = MW_DRIVER_LD013V7DH03_NUM_BYTES_PER_LINE - 1;
    params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_ADDR_INDEX].xfer.srcAddr = &params->_displayLine;
    params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_TRAILING_INDEX].xfer.srcAddr = &_TRAILING;
    params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_TRAILING_INDEX].xfer.link = 0;
    params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_TRAILING_INDEX].xfer.linkAddr = 0;
    params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_TRAILING_INDEX].xfer.doneIfs = true;
}


static void MW_LS013V7DH03_startTimer(MW_LS013B7DH03_Params *params, uint32_t counterVal)
{
    params->_timerExpired = 0;
    TIMER_TopSet(params->signallingTimer, counterVal);
    TIMER_Enable(params->signallingTimer, true);
}



//  ============================================================    //
//  ============================================================    //



/*
 *  Initialize an instance of the Sharp LS013B7DH03 Display
 *  This driver uses the following peripherals for display operation:
 *      LETIMERx:   VCOM (external COM) signalling
 *                  The pin output feature of the LETIMER is used.  Therefore, the user must also assign appropriate pins and LOC
 *                  for LETIMER Output
 *      TIMERx:     Ensuring signal timing between SCK and CS lines
 *      LDMA:       For asynchronous display updates (pixel writes).  User must assign LDMA channels
 *      USARTx:     For SPI communication
 * 
 *  Exact peripherals are specified by the user by assigning appropriate values in the MW_LS013B7DH03_Params object
 * 
 *  Inputs:
 *      params:     Pointer to MW_LS013B7DH03_Params Instance
 * 
 *  Returns:
 *      0:          If initialization unsuceesfful
 *      1:          If initialization successful
 */
int32_t MW_LS013B7DH03_init(MW_LS013B7DH03_Params *params)
{
    if (params == NULL)
        return 0;

    MW_LS013B7DH03_setupLETimer(params);
    MW_LS013B7DH03_setupSignallingTimer(params);
    MW_LS013B7DH03_setupSPI(params);
    MW_LS013B7DH03_setupLDMA(params);

    params->_timerExpired = 0;
    params->_pixelTransferInProgress = 0;

    params->_isInitialized = 1;

    return 1;
}


/*
 *  Start the USART and LETIMER
 *  This function will begin the pulse train routed to the display's EXTCOM pin
 *  No data is sent to the display!
 *
 *  Inputs:
 *      params:     Pointer to MW_LS013B7DH03_Params instance
 * 
 *  Returns:
 *      0:          If unsucessful
 *      1:          If successful
 */
int32_t MW_LS013B7DH03_start(MW_LS013B7DH03_Params *params)
{
    if (params == NULL) return 0;
    if (params->_isInitialized == 0) return 0;

    LETIMER_Enable(LETIMER0, true);
    USART_Enable(params->usartModule, usartEnableTx);

    return 1;
}


/*
 *  Stop the USART and LETIMER
 *  Stop the pulse train routed to the display's EXTCOM pin
 *
 *  Inputs:
 *      params:     Pointer to MW_LS013B7DH03_Params instance
 * 
 *  Returns:
 *      0:          If unsucessful
 *      1:          If successful
 */
int32_t MW_LS013B7DH03_stop(MW_LS013B7DH03_Params *params)
{
    if (params == NULL) return 0;
    if (params->_isInitialized == 0) return 0;

    //  If the module is still writing to the display, wait for it to finish
    while(params->_pixelTransferInProgress);

    LETIMER_Enable(LETIMER0, false);
    USART_Enable(params->usartModule, usartDisable);

    return 1;
}


/*
 *  Write the frame buffer to the display asynchronously
 *  Note that writing to the frame buffer while transfer is taking place will result in race conditions
 *  It is recommended that a buffer of frame buffers be used when using this function
 *  As a transfer is taking place, a different frame buffer can be modified
 *  When MW_LS013B7DH03_drawAsync is called again, the new frame buffer can be passed in
 *
 *  Inputs:
 *      params:         Pointer to MW_LS013B7DH03_Params instance
 *      frameBuffer:    Pointer to frame buffer to write   
 * 
 *  Returns:
 *      0:          If unsucessful (Attempted to call thsi function while a transfer was taking place)
 *      1:          If successful
 */
int32_t MW_LS013B7DH03_drawAsync(MW_LS013B7DH03_Params *params, uint8_t *frameBuffer)
{
    if (params == NULL || frameBuffer == NULL) return 0;

    //  Check to see if a transfer is currently in progress
    //  If so, then we must return an error
    if (params->_pixelTransferInProgress) 
        return 0;

    params->_frameBufferRow = frameBuffer;
    params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_PIXELS_INDEX].xfer.srcAddr = (uint8_t*)params->_frameBufferRow;
    params->_displayLine = MW_DRIVER_LD013V7DH03_ADDR_START;

    LDMA->IFC |= (1 << params->ldmaChannel);

    params->_pixelTransferInProgress = 1;

    MW_DRIVER_LD013V7DH03_SET_CS_HIGH();
    params->_signallingTimerISRType = SIGNALLING_TIMER_ISR_TYPE_ASYNC_START;
    MW_LS013V7DH03_startTimer(params, _TSSCS_COUNTER_VAL);

    return 1;
}


/*
 *  Draw frame buffer to display
 *  This function will block until the entire display is updated
 *  For asynchronous transfers, use MW_LS013B7DH03_drawAsync()
 * 
 *  Note that it is NOT recommended to run both drawBlocking() and drawAsync() in a given code block!
 * 
 *  Inputs:
 *      params:         Pointer to MW_LS013B7DH03_Params instance
 *      frameBuffer:    Pointer to frame buffer to write   
 * 
 *  Returns:
 *      0:          If unsucessful (Attempted to call thsi function while a transfer was taking place)
 *      1:          If successful
 */
int32_t MW_LS013B7DH03_drawBlocking(MW_LS013B7DH03_Params *params, uint8_t *frameBuffer)
{
    if (params == NULL|| frameBuffer == NULL) return 0;
    if (params->_isInitialized == 0) return 0;
    if (params->_pixelTransferInProgress) return 0;

    params->_pixelTransferInProgress = 1;

    //  Transmit data
    //  Drawing all pixels will have the data structure:
    //  |CMD||ADDR1||DATA0|...|DATA15||TRAILING BYTE||ADDR2||DATA0|...|DATA15||TRAILING BYTE|...|ADDR128||DATA0|...|DATA15||TRAILING BYTE||TRAILING BYTE|
    MW_DRIVER_LD013V7DH03_SET_CS_HIGH();

    //  A 6 usec delay is needed between the rising edge of CS and SCK for guaranteed performance
    params->_signallingTimerISRType = SIGNALLING_TIMER_ISR_TYPE_BLOCKING;
    MW_LS013V7DH03_startTimer(params, _TSSCS_COUNTER_VAL);

    while(!params->_timerExpired);

    USART_SpiTransfer(params->usartModule, MW_DRIVER_LD013V7DH03_WRITE_CMD);

    //  Loop draws a single line
    for (int32_t i = 0; i < MW_DRIVER_LD013V7DH03_NUM_LINES; ++i)
    {
        USART_SpiTransfer(params->usartModule, i + 1);      //  Send address (line number)
        for (int32_t j = 0; j < MW_DRIVER_LD013V7DH03_NUM_BYTES_PER_LINE; ++j)
            USART_SpiTransfer(params->usartModule, frameBuffer[(i * MW_DRIVER_LD013V7DH03_NUM_BYTES_PER_LINE) + j]);

        USART_SpiTransfer(params->usartModule, 0x00);
    }

    //  Last trailing bytes
    USART_SpiTransfer(params->usartModule, 0x00);

    MW_LS013V7DH03_startTimer(params, _THSCS_COUNTER_VAL);

    while(!params->_timerExpired);

    MW_DRIVER_LD013V7DH03_SET_CS_LOW();

    params->_pixelTransferInProgress = 0;
}


/*
 *  Callback function for LDMA ISR
 *  This function must be registered to the LDMA ISR so that when the interrupt is triggered, this function will run
 * 
 *  Inputs:
 *      params:         Pointer to MW_LS013B7DH03_Params instance
 * 
 *  Returns:
 *      None
 */
void MW_LS013B7DH03_ldmaISR(volatile MW_LS013B7DH03_Params *params)
{
    LDMA->IFC |= (1 << params->ldmaChannel);

    if (params == NULL) return;

    if (params->_displayLine <= MW_DRIVER_LD013V7DH03_NUM_LINES)
    {
        params->_displayLine++;
        params->_frameBufferRow += MW_DRIVER_LD013V7DH03_NUM_BYTES_PER_LINE;
        params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_PIXELS_INDEX].xfer.srcAddr = (uint8_t*)(params->_frameBufferRow);

        LDMA_StartTransfer(params->ldmaChannel, &params->_ldmaCfg, &params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_ADDR_INDEX]);
    }

    else if (params->_displayLine == MW_DRIVER_LD013V7DH03_NUM_LINES + 1)
    {
        params->_displayLine++;
        LDMA_StartTransfer(params->ldmaChannel, &params->_ldmaCfg, &params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_TRAILING_INDEX]);
    }

    else
    {
        LDMA_StopTransfer(params->ldmaChannel);
        params->_signallingTimerISRType = SIGNALLING_TIMER_ISR_TYPE_ASYNC_END;
        MW_LS013V7DH03_startTimer(params, _THSCS_COUNTER_VAL);
    }
}


/*
 *  Callback function for signalling timer
 *  This function must be registered to the TIMER ISR so that when the interrupt is triggered, this function will run
 * 
 *  Inputs:
 *      params:         Pointer to MW_LS013B7DH03_Params instance
 * 
 *  Returns:
 *      None
 */
void MW_LS013B7DH03_signallingTimerISR(MW_LS013B7DH03_Params * const params)
{
    TIMER_IntClear(params->signallingTimer, TIMER_IFC_OF);

    if (params->_signallingTimerISRType == SIGNALLING_TIMER_ISR_TYPE_BLOCKING)
    {
        params->_timerExpired = 1;
        TIMER_Enable(params->signallingTimer, false);
    }

    if (params->_signallingTimerISRType == SIGNALLING_TIMER_ISR_TYPE_ASYNC_START)
    {
        LDMA_StartTransfer( params->ldmaChannel, 
                        (void*)&params->_ldmaCfg,
                        (void*)&params->_ldmaDesc[MW_DRIVER_LD013V7DH03_LDMA_CMD_INDEX]);
    }

    if (params->_signallingTimerISRType == SIGNALLING_TIMER_ISR_TYPE_ASYNC_END)
    {
        MW_DRIVER_LD013V7DH03_SET_CS_LOW();
        params->_pixelTransferInProgress = 0;
    }
}