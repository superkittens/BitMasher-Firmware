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

#include "MW_Driver_CS42L52.h"


#define MAX_BASS_GAIN       12.5f
#define MIN_BASS_GAIN       -12.5f
#define MAX_TREBLE_GAIN     12.5f
#define MIN_TREBLE_GAIN     -12.5f

//  Dummy variables to store right channel values if mono is used
static int16_t _rxValue = 0;

static int32_t startI2CTransfer(MW_CS42L52_Params *codecParams, uint16_t readWrite, uint8_t *txBuffer, int32_t numTXBytes, uint8_t *rxBuffer, int32_t numRXBytes)
{
    if (txBuffer == NULL || rxBuffer == NULL)
        return 0;

    I2C_TransferSeq_TypeDef i2cTransfer;

    i2cTransfer.addr = MW_DRIVER_CS42L52_I2C_ADDR;
    i2cTransfer.flags = readWrite;
    i2cTransfer.buf[0].data = txBuffer;
    i2cTransfer.buf[0].len = numTXBytes;
    i2cTransfer.buf[1].data = rxBuffer;
    i2cTransfer.buf[1].len = numRXBytes;

    I2C_TransferInit(codecParams->i2cModule, &i2cTransfer);

    while (I2C_Transfer(codecParams->i2cModule) == i2cTransferInProgress);

    return 1;
}


// static void startI2CTransferAsync()


/*
 *  Setup I2S lines between the microcontroller and the codec
 *  The microcontroller is setup to drive the codec
 */
static int32_t MW_Driver_CS42L52_setupI2S(MW_CS42L52_Params *codecParams)
{
    if (codecParams == NULL)
        return 0;

    CMU_ClockEnable(cmuClock_USART3, true);

    GPIO_PinModeSet(codecParams->i2sSDINPort, codecParams->i2sSDINPin, gpioModePushPull, 0);
    GPIO_PinModeSet(codecParams->i2sSDOUTPort, codecParams->i2sSDOUTPin, gpioModeInputPull, 0);
    GPIO_PinModeSet(codecParams->i2sSCKPort, codecParams->i2sSCKPin, gpioModeInputPull, 0);
    GPIO_PinModeSet(codecParams->i2sLRCKPort, codecParams->i2sLRCKPin, gpioModeInputPull, 0);

    USART_InitI2s_TypeDef init = USART_INITI2S_DEFAULT;

    init.sync.databits = usartDatabits16;
    init.delay = false;
    init.sync.enable = usartDisable;
    init.sync.autoTx = true;
    init.format = usartI2sFormatW32D16;
    init.justify = usartI2sJustifyLeft;
    init.dmaSplit = true;
    init.sync.baudrate = (uint32_t)MW_DRIVER_CS42L52_FS * 64;
    init.sync.master = false;

    USART_InitI2s(USART3, &init);

    USART3->CTRL |= USART_CTRL_CSINV;
    USART3->CTRLX |= 1;
    //USART3->CTRL |= USART_CTRL_MSBF;

    USART3->ROUTEPEN = USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_CSPEN | USART_ROUTEPEN_CLKPEN;
    USART3->ROUTELOC0 = codecParams->i2sSDOUTLoc |
                        codecParams->i2sSDINLoc |
                        codecParams->i2sLRCKLoc |
                        codecParams->i2sSCKLoc;

    //    NVIC_ClearPendingIRQ(USART3_RX_IRQn);
    //    NVIC_ClearPendingIRQ(USART3_TX_IRQn);
    //    NVIC_EnableIRQ(USART3_RX_IRQn);
    //    NVIC_EnableIRQ(USART3_TX_IRQn);
    //
    //    USART3->IEN = 0xFC;

    return 1;
}

/*
 *  Start the MCLK for the codec
 *  MCLK is driven by the EFM32PG12 CMU unit and leverages the HFRCO oscillator with the DPLL module
 *  to generate the 12 MHz clock.
 *  The MCLK clock is routed to CLKOUT pin defined by the user
 * 
 *  Inputs:
 *      codecParams:    MW_CS42L52_Params instance that contains which pin the MCLK should output to
 * 
 *  Returns:
 *      0 if initialization unsuccessful
 *      1 otherwise
 */
static int32_t MW_Driver_CS42L52_startMCLK(MW_CS42L52_Params *codecParams)
{
    if (codecParams == NULL)
        return 0;

    //  Setup DPLL using HFXO
    //  We could use this but the LFXO actually has a multiple that works nicely with the 12.288 MHz MCLK frequency
    //  that the codec can accept so we will use that instead
    //  CMU_DPLLInit_TypeDef dpllInit;;

    //  dpllInit.frequency = 12000000;
    //  dpllInit.n = 899;
    //  dpllInit.m = 2999;
    //  dpllInit.ssInterval = 0;
    //  dpllInit.ssAmplitude = 0;
    //  dpllInit.refClk = cmuDPLLClkSel_Hfxo;
    //  dpllInit.edgeSel = cmuDPLLEdgeSel_Fall;
    //  dpllInit.lockMode = cmuDPLLLockMode_Freq;
    //  dpllInit.autoRecover = true;

    // Set MCLK frequency to 12.288 MHz by scaling the LFXO (32.768 kHz)
   CMU_DPLLInit_TypeDef dpllInit = CMU_DPLL_LFXO_TO_40MHZ;
   dpllInit.frequency = 12288000;
   dpllInit.n = 374;
   dpllInit.m = 0;

    if (!CMU_DPLLLock(&dpllInit))
        return 0;

    //  Setup clock output pin
    GPIO_PinModeSet(codecParams->MCLKPort, codecParams->MCLKPin, gpioModePushPull, 0);
    CMU->CTRL = (CMU->CTRL & ~_CMU_CTRL_CLKOUTSEL0_MASK) | CMU_CTRL_CLKOUTSEL0_HFRCOQ;
    CMU->ROUTELOC0 = CMU->ROUTELOC0 | codecParams->MCLKLoc;
    CMU->ROUTEPEN = CMU->ROUTEPEN | 0x00000001;

    return 1;
}


static void MW_Driver_CS42L52_setupCodecI2C(MW_CS42L52_Params *codecParams)
{
    if (codecParams->i2cModule == I2C0)
        CMU_ClockEnable(cmuClock_I2C0, true);

    if (codecParams->i2cModule == I2C1)
        CMU_ClockEnable(cmuClock_I2C1, true);

    CMU_ClockEnable(cmuClock_GPIO, true);

    //  Initialize I2C for codec control
    I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
    i2cInit.freq = MW_DRIVER_CS42L52_I2C_FSCK;
    i2cInit.clhr = i2cClockHLRStandard;

    //  Setup GPIOs for I2C lines
    GPIO_PinModeSet(codecParams->i2cSDAPort, codecParams->i2cSDAPin, gpioModeWiredAnd, 1);
    GPIO_PinModeSet(codecParams->i2cSCLPort, codecParams->i2cSCLPin, gpioModeWiredAnd, 1);

    //  Map GPIO pins in the I2C config registers
    codecParams->i2cModule->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
    codecParams->i2cModule->ROUTELOC0 = (codecParams->i2cModule->ROUTELOC0 & (~_I2C_ROUTELOC0_SDALOC_MASK)) | codecParams->i2cSDALoc;
    codecParams->i2cModule->ROUTELOC0 = (codecParams->i2cModule->ROUTELOC0 & (~_I2C_ROUTELOC0_SCLLOC_MASK)) | codecParams->i2cSCLLoc;

    I2C_Init(codecParams->i2cModule, &i2cInit);
}


static void MW_Driver_CS42L52_runCodecInitializationRoutine(MW_CS42L52_Params *codecParams)
{
    //  Start MCLK
    MW_Driver_CS42L52_startMCLK(codecParams);

    uint8_t txBuffer[2];
    uint8_t rxBuffer[2];

    //  First, set the codec PDN bit in the Power Control 1 (addr: 0x02) register
    txBuffer[0] = 0x02;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_READ, txBuffer, 1, rxBuffer, 1);

    txBuffer[0] = 0x02;
    txBuffer[1] = 0x01;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  SPKR off HP on
    txBuffer[0] = 0x04;
    txBuffer[1] = 0xAF;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  Set sampling frequency and control bits
    //  fs = 32 kHz
    txBuffer[0] = 0x05;
    txBuffer[1] = 0x32;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  Set to master mode (0x83 for 16 bit DAC value)
    txBuffer[0] = 0x06;
    txBuffer[1] = 0x80;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  Retimed SCK and loopback (set to 0x20.  Normal 0x00);
    txBuffer[0] = 0x07;
    txBuffer[1] = 0x00;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  Use ADC1A
    txBuffer[0] = 0x08;
    txBuffer[1] = 0x00;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  Use ADC1B
    txBuffer[0] = 0x09;
    txBuffer[1] = 0x00;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  Start Codec
    txBuffer[0] = 0x02;
    txBuffer[1] = 0x00;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);
}

/*
 *  Setup LDMA descriptors for audio data transfer into and out of the codec
 */
static int32_t MW_Driver_CS42L52_setupLDMA(MW_CS42L52_Params *codecParams)
{
  if (codecParams == NULL)
      return 0;

  if (codecParams->LDMALeftBuffers == NULL || codecParams->numBuffers <= 0)
      return 0;

  if (codecParams->ldmaLeftTxDescriptors == NULL || codecParams->ldmaLeftRxDescriptors == NULL)
      return 0;

//   CMU_ClockEnable(cmuClock_LDMA, true);

//   LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
//   ldmaInit.ldmaInitCtrlNumFixed = 0;

//   LDMA_Init(&ldmaInit);

//   LDMA->DBGHALT |= 0x000000FF;


   //  Setup left channel LDMA configurations
   //  Setup RX LDMA
   codecParams->ldmaLeftRxTransferCfg = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART3_RXDATAV);
   codecParams->ldmaRightRxTransferCfg = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART3_RXDATAVRIGHT);

   for (int32_t i = 0; i < codecParams->numBuffers - 1; ++i)
   {
       codecParams->ldmaLeftRxDescriptors[i] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(
                                                                   &USART3->RXDOUBLE,
                                                                   (uint8_t*)(codecParams->LDMALeftBuffers + (i * codecParams->bufferSize)),
                                                                   codecParams->bufferSize,
                                                                   1
                                                                   );

       codecParams->ldmaLeftRxDescriptors[i].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
       codecParams->ldmaLeftRxDescriptors[i].xfer.ignoreSrec = true;
       codecParams->ldmaLeftRxDescriptors[i].xfer.size = ldmaCtrlSizeHalf;
       codecParams->ldmaLeftRxDescriptors[i].xfer.doneIfs = true;

       //  Setup LDMA RX configuration if stereo is specified
       if (!codecParams->isMono)
       {
           codecParams->ldmaRightRxDescriptors[i] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(
                                                                   &USART3->RXDOUBLE,
                                                                   (uint8_t*)(codecParams->LDMARightBuffers + (i * codecParams->bufferSize)),
                                                                   codecParams->bufferSize,
                                                                   1
                                                                   );

           codecParams->ldmaRightRxDescriptors[i].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
           codecParams->ldmaRightRxDescriptors[i].xfer.ignoreSrec = true;
           codecParams->ldmaRightRxDescriptors[i].xfer.size = ldmaCtrlSizeHalf;
           codecParams->ldmaRightRxDescriptors[i].xfer.doneIfs = true;

       }
   }


   codecParams->ldmaLeftRxDescriptors[codecParams->numBuffers - 1] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(
                                                                                       &USART3->RXDOUBLE,
                                                                                       (uint8_t*)(codecParams->LDMALeftBuffers + (codecParams->bufferSize * (codecParams->numBuffers - 1))),
                                                                                       codecParams->bufferSize,
                                                                                       -(codecParams->numBuffers - 1)
                                                                                       );

   codecParams->ldmaLeftRxDescriptors[codecParams->numBuffers - 1].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
   codecParams->ldmaLeftRxDescriptors[codecParams->numBuffers - 1].xfer.ignoreSrec = true;
   codecParams->ldmaLeftRxDescriptors[codecParams->numBuffers - 1].xfer.size = ldmaCtrlSizeHalf;
   codecParams->ldmaLeftRxDescriptors[codecParams->numBuffers - 1].xfer.doneIfs = true;

   if (!codecParams->isMono)
   {
       codecParams->ldmaRightRxDescriptors[codecParams->numBuffers - 1] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(
                                                                                       &USART3->RXDOUBLE,
                                                                                       (uint8_t*)(codecParams->LDMARightBuffers + (codecParams->bufferSize * (codecParams->numBuffers - 1))),
                                                                                       codecParams->bufferSize,
                                                                                       -(codecParams->numBuffers - 1));

       codecParams->ldmaRightRxDescriptors[codecParams->numBuffers - 1].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
       codecParams->ldmaRightRxDescriptors[codecParams->numBuffers - 1].xfer.ignoreSrec = true;
       codecParams->ldmaRightRxDescriptors[codecParams->numBuffers - 1].xfer.size = ldmaCtrlSizeHalf;
       codecParams->ldmaRightRxDescriptors[codecParams->numBuffers - 1].xfer.doneIfs = true;
   }
   else
   {
       *(codecParams->ldmaRightRxDescriptors) = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(
                                                                                       &USART3->RXDOUBLE,
                                                                                       &_rxValue,
                                                                                       1,
                                                                                       0);

       codecParams->ldmaRightRxDescriptors->xfer.blockSize = ldmaCtrlBlockSizeUnit1;
       codecParams->ldmaRightRxDescriptors->xfer.ignoreSrec = true;
       codecParams->ldmaRightRxDescriptors->xfer.size = ldmaCtrlSizeHalf;
       codecParams->ldmaRightRxDescriptors->xfer.doneIfs = false;
   }



   //  Setup TX LDMA
   //  The first descriptor points to the second buffer since the first buffer is reserved for the first RX LDMA descriptor
   codecParams->ldmaLeftTxTransferCfg = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART3_TXBL);
   codecParams->ldmaRightTxTransferCfg = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART3_TXBLRIGHT);


   for (int32_t i = 0; i < codecParams->numBuffers - 1; ++i)
   {
       int32_t index = (1 + i) % codecParams->numBuffers;

       codecParams->ldmaLeftTxDescriptors[i] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(
                                                                   (uint8_t*)(codecParams->LDMALeftBuffers + (index * codecParams->bufferSize)),
                                                                   &USART3->TXDOUBLE,
                                                                   codecParams->bufferSize,
                                                                   1
       );

       codecParams->ldmaLeftTxDescriptors[i].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
       codecParams->ldmaLeftTxDescriptors[i].xfer.ignoreSrec = true;
       codecParams->ldmaLeftTxDescriptors[i].xfer.size = ldmaCtrlSizeHalf;
       codecParams->ldmaLeftTxDescriptors[i].xfer.doneIfs = false;


       int16_t *sourceAddress = codecParams->LDMALeftBuffers + (index * codecParams->bufferSize);
       if (!codecParams->isMono)
           sourceAddress = codecParams->LDMARightBuffers + (index * codecParams->bufferSize);

       codecParams->ldmaRightTxDescriptors[i] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(
                                                                   (uint8_t*)sourceAddress,
                                                                   &USART3->TXDOUBLE,
                                                                   codecParams->bufferSize,
                                                                   1
           );

       codecParams->ldmaRightTxDescriptors[i].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
       codecParams->ldmaRightTxDescriptors[i].xfer.ignoreSrec = true;
       codecParams->ldmaRightTxDescriptors[i].xfer.size = ldmaCtrlSizeHalf;
       codecParams->ldmaRightTxDescriptors[i].xfer.doneIfs = false;
   }


   codecParams->ldmaLeftTxDescriptors[codecParams->numBuffers - 1] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(
                                                                       (uint8_t *)codecParams->LDMALeftBuffers,
                                                                       &USART3->TXDOUBLE,
                                                                       codecParams->bufferSize,
                                                                       -(codecParams->numBuffers - 1));

   codecParams->ldmaLeftTxDescriptors[codecParams->numBuffers - 1].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
   codecParams->ldmaLeftTxDescriptors[codecParams->numBuffers - 1].xfer.ignoreSrec = true;
   codecParams->ldmaLeftTxDescriptors[codecParams->numBuffers - 1].xfer.size = ldmaCtrlSizeHalf;
   codecParams->ldmaLeftTxDescriptors[codecParams->numBuffers - 1].xfer.doneIfs = false;


   int16_t *sourceAddress = codecParams->LDMALeftBuffers;
   if (!codecParams->isMono)
       sourceAddress = codecParams->LDMARightBuffers;

   codecParams->ldmaRightTxDescriptors[codecParams->numBuffers - 1] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(
                                                                     (uint8_t *)sourceAddress,
                                                                       &USART3->TXDOUBLE,
                                                                       codecParams->bufferSize,
                                                                       -(codecParams->numBuffers - 1));

   codecParams->ldmaRightTxDescriptors[codecParams->numBuffers - 1].xfer.blockSize = ldmaCtrlBlockSizeUnit1;
   codecParams->ldmaRightTxDescriptors[codecParams->numBuffers - 1].xfer.ignoreSrec = true;
   codecParams->ldmaRightTxDescriptors[codecParams->numBuffers - 1].xfer.size = ldmaCtrlSizeHalf;
   codecParams->ldmaRightTxDescriptors[codecParams->numBuffers - 1].xfer.doneIfs = false;
}

/*
 *  Initialize the CS42L53 driver
 *  The driver uses USART0 to manage transfers between the microcontroller and the codec
 *  Note that since the microcontroller is controlled by the codec, USART TX and RX are actually swapped!
 *      ie.  TX = MOSI and RX = MISO
 *  Be careful of this when assigning ports
 */
int32_t MW_Driver_CS42L52_init(MW_CS42L52_Params *codecParams)
{
    MW_Driver_CS42L52_setupCodecI2C(codecParams);
    MW_Driver_CS42L52_setupI2S(codecParams);
    MW_Driver_CS42L52_setupLDMA(codecParams);

    //  Set the RESET pin to high (ACTIVE)
    GPIO_PinModeSet(codecParams->resetPort, codecParams->resetPin, gpioModePushPull, 0);
    GPIO_PinOutSet(codecParams->resetPort, codecParams->resetPin);

    MW_Driver_CS42L52_runCodecInitializationRoutine(codecParams);

    codecParams->isInitialized = 1;

    return 1;
}


uint8_t MW_Driver_CS42L52_getChipID(MW_CS42L52_Params *codecParams)
{
    if (codecParams == NULL)
        return 0;

    if (!codecParams->isInitialized)
        return 0;

    uint8_t cmd[1] = {0x01};
    uint8_t rx[1];
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_READ, cmd, 1, rx, 1);

    return (rx[0] & 0xF8) >> 3;
}


uint8_t MW_Driver_CS42L52_getChipRev(MW_CS42L52_Params *codecParams)
{
    if (codecParams == NULL)
        return 0;

    if (!codecParams->isInitialized)
        return 0;

    uint8_t cmd = 0x01;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_READ, &cmd, 1, &cmd, 1);

    return cmd & 0x07;
}

int32_t MW_Driver_CS42L52_start(MW_CS42L52_Params *codecParams)
{
    LDMA_StartTransfer(MW_DRIVER_CS42L52_LEFT_RX_LDMA_CHANNEL, (void *)&codecParams->ldmaLeftRxTransferCfg, (void *)&codecParams->ldmaLeftRxDescriptors[0]);
    LDMA_StartTransfer(MW_DRIVER_CS42L52_RIGHT_RX_LDMA_CHANNEL, (void *)&codecParams->ldmaRightRxTransferCfg, (void *)&codecParams->ldmaRightRxDescriptors[0]);
    LDMA_StartTransfer(MW_DRIVER_CS42L52_LEFT_TX_LDMA_CHANNEL, (void *)&codecParams->ldmaLeftTxTransferCfg, (void *)&codecParams->ldmaLeftTxDescriptors[0]);
    LDMA_StartTransfer(MW_DRIVER_CS42L52_RIGHT_TX_LDMA_CHANNEL, (void *)&codecParams->ldmaRightTxTransferCfg, (void *)&codecParams->ldmaRightTxDescriptors[0]);

    USART_Enable(USART3, usartEnable);

    return 1;
}

int32_t MW_Driver_CS42L52_stop(MW_CS42L52_Params *codecParams)
{
    LDMA_StopTransfer(MW_DRIVER_CS42L52_LEFT_RX_LDMA_CHANNEL);
    LDMA_StopTransfer(MW_DRIVER_CS42L52_LEFT_TX_LDMA_CHANNEL);
    LDMA_StopTransfer(MW_DRIVER_CS42L52_RIGHT_RX_LDMA_CHANNEL);
    LDMA_StopTransfer(MW_DRIVER_CS42L52_RIGHT_TX_LDMA_CHANNEL);

    USART_Enable(USART3, usartDisable);
    
    return 1;
}

/*
 *  Set RESET pin of codec high or low
 *  
 *  Inputs:
 *      codecParams:    MW_CS42L52 instance
 *      state:          0: Reset LOW (codec INACTIVE), 1:  Reset HIGH (codec ACTIVE)
 */
int32_t MW_Driver_CS42L52_setReset(MW_CS42L52_Params *codecParams, int32_t state)
{
    if (codecParams == NULL)
        return 0;

    if (state)
        GPIO_PinOutSet(codecParams->resetPort, codecParams->resetPin);
    else
        GPIO_PinOutClear(codecParams->resetPort, codecParams->resetPin);

    return 1;
}


//  Set the input ADCA volume (0x16)
//  Range is from -96 dB to 24 dB
void MW_Driver_CS42L52_setADCVolume(MW_CS42L52_Params *codecParams, int8_t volume)
{
    uint8_t txBuffer[2] = {0x16, volume};
    uint8_t rxBuffer[2];

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);
}


//  Set the master (MSTA) volume (0x20)
//  Range is from -102 dB to 12 dB with step size of 0.5 dB
void MW_Driver_CS42L52_setMasterVolume(MW_CS42L52_Params *codecParams, int8_t volume)
{
    uint8_t txBuffer[2] = {0x20, volume};
    uint8_t rxBuffer[2];

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    txBuffer[0] = 0x21;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);
}


//  Headphone volume range is from -96 dB to 0 dB
void MW_Driver_CS42L52_setHeadphoneVolume(MW_CS42L52_Params *codecParams, int8_t volume)
{
    //  Set left channel volume
    uint8_t txBuffer[2] = {0x22, volume};
    uint8_t rxBuffer[2];

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    //  Set right channel volume
    txBuffer[0] = 0x23;
    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);
}


int32_t MW_Driver_CS42L52_enableToneControls(const MW_CS42L52_Params * const codecParams, const int32_t enable)
{
    if (codecParams == NULL)
        return 0;

    uint8_t txBuffer[2] = {0x1E, 0x01};
    uint8_t rxBuffer[2];

    if (enable == 0)
        txBuffer[1] = 0x00;

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);
    
    return 1;
}


void MW_Driver_CS42L52_setBassGain(const MW_CS42L52_Params * const codecParams, float32_t gainDB)
{
    if (codecParams == NULL)
        return 0;

    if (gainDB > MAX_BASS_GAIN)
        gainDB = MAX_BASS_GAIN;

    if (gainDB < MIN_BASS_GAIN)
        gainDB = MIN_BASS_GAIN;

    int32_t multiplier = (int32_t)(gainDB / 1.5f);
    int32_t registerValue = 8 - multiplier;

    if (multiplier < 0)
        registerValue = -8 - multiplier;

    uint8_t txBuffer[2] = {0x1F, 0x00};
    uint8_t rxBuffer[2];

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_READ, txBuffer, 1, rxBuffer, 1);

    //  Modify only the lower nybble of the bass/treble gain register
    txBuffer[1] = (rxBuffer[0] & 0xF0) | (registerValue & 0x0F);

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);
}


void MW_Driver_CS42L52_setTrebleGain(const MW_CS42L52_Params * const codecParams, float32_t gainDB)
{
    if (codecParams == NULL)
        return 0;

    if (gainDB > MAX_TREBLE_GAIN)
        gainDB = MAX_TREBLE_GAIN;

    if (gainDB < MIN_TREBLE_GAIN)
        gainDB = MIN_TREBLE_GAIN;


    int multiplier = (int32_t)(gainDB / 1.5f);
    int32_t registerValue = 8 - multiplier;


    if (multiplier < 0)
        registerValue = -8 - multiplier;

    uint8_t txBuffer[2] = {0x1F, 0x00};
    uint8_t rxBuffer[2];

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_READ, txBuffer, 1, rxBuffer, 1);

    //  Modify only the lower nybble of the bass/treble gain register
    txBuffer[1] = (rxBuffer[0] & 0x0F) | (registerValue << 4);

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);
}

int32_t MW_Driver_CS42L52_setAudioPassthrough(const MW_CS42L52_Params * const codecParams, const int32_t byPass)
{
    if (codecParams == NULL)
        return 0;

    // uint8_t txBuffer[2] = {0x0E, 0x42};
    // uint8_t rxBuffer[2];

    // if (!byPass)
    //     txBuffer[1] = 0x02;

    // startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    uint8_t txBuffer[2] = {0x07, 0x20};
    uint8_t rxBuffer[2];

    if (!byPass)
        txBuffer[1] = 0x00;

    startI2CTransfer(codecParams, I2C_FLAG_WRITE_WRITE, txBuffer, 2, rxBuffer, 0);

    return 1;
}
