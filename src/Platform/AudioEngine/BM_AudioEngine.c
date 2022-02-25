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


#include "BM_AudioEngine.h"


static int32_t audioEngineInitialized = 0;
static int32_t usingInternalCodec = 0;
static MW_CS42L52_Params codecParams;

volatile uint32_t  BM_AE_bufferCurrentlyProcessing;

//  Audio Buffers and Queues
volatile int16_t BM_AE_buffer[BM_AE_NUM_BUFFERS][AUDIO_BUFFER_SIZE];



//  LDMA Configuration Variables
//---------------------------------------------------------------------------------------------------------//
static volatile LDMA_TransferCfg_t leftRxLDMATransCfg;
static volatile LDMA_Descriptor_t leftRxLDMADesc[BM_AE_NUM_BUFFERS];
static volatile LDMA_TransferCfg_t leftTxLDMATransCfg;
static volatile LDMA_Descriptor_t leftTxLDMADesc[BM_AE_NUM_BUFFERS];

static volatile LDMA_Descriptor_t rightRxLDMADesc;
static volatile LDMA_Descriptor_t rightTxLDMADesc[BM_AE_NUM_BUFFERS];

static uint32_t ADC_LDMA_CHANNEL = 0;
static uint32_t DAC_LDMA_CHANNEL = 1;

static uint32_t currentInputBuffer = 0;




/*
 *  Initialize sampling timer (TIMER0) that drives audio sample reads and writes
 *  The timer configuration here is a count up type
 *  The timer will need to be MANUALLY started after initialization to kick off the audio engine processes
 */
static void BM_AE_setupSamplingTimer(const float32_t fs)
{
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_TIMER0, true);
  CMU_ClockEnable(cmuClock_PRS, true);

  //  Configure the timer
  //  We want an up-counting timer that goes up to a number that corresponds with 12.5 usec
  //  For a 40 MHz clock (no prescaling), that is FS_TIMER_LIMIT
  TIMER_Init_TypeDef init = TIMER_INIT_DEFAULT;

  //  Don't start the timer after initialization yet
  init.enable = false;
  init.dmaClrAct = true;

  TIMER_Init(TIMER0, &init);

  //  Calculate the number of pulses needed for each sampling period
  float32_t samplingPeriod = 1 / fs;
  float32_t numPulsesf = samplingPeriod * (float32_t)CMU_ClockFreqGet(cmuClock_TIMER0);
  uint32_t numPulses = (uint32_t)numPulsesf;

  TIMER_TopSet(TIMER0, numPulses);

  PRS_SourceAsyncSignalSet(0, PRS_CH_CTRL_SOURCESEL_TIMER0, PRS_CH_CTRL_SIGSEL_TIMER0OF);
}


/*
 *  Setup internal ADC for audio sampling
 *  NOTE:  The internal ADC will eventually be replaced by an audio codec
 *  The internal ADC will be used for proof-of-concept purposes
 */
static void BM_AE_setupADC()
{
  CMU_ClockEnable(cmuClock_ADC0, true);

  ADC_Init_TypeDef init = ADC_INIT_DEFAULT;
  ADC_InitSingle_TypeDef sInit = ADC_INITSINGLE_DEFAULT;

  init.timebase = ADC_TimebaseCalc(0);
  init.prescale = ADC_PrescaleCalc(10000000, 0);

  ADC_Init(ADC0, &init);

  sInit.reference = adcRefVDD;
  sInit.acqTime = adcAcqTime8;
  sInit.posSel = adcPosSelAPORT0XCH0;
  sInit.negSel = adcNegSelVSS;
  sInit.rep = false;
  sInit.prsEnable = true;
  sInit.prsSel = adcPRSSELCh0;

  ADC_InitSingle(ADC0, &sInit);
}


/*
 * Setup internal DAC for audio output
 * NOTE:  The internal DAC will eventually be replaced by an audio codec
 * The internal DAC will be used for proof-of-concept purposes
 */
static void BM_AE_setupDAC()
{
  //  Enable VDAC clock
  CMU_ClockEnable(cmuClock_VDAC0, true);

  VDAC_Init_TypeDef vdac_init = VDAC_INIT_DEFAULT;
  VDAC_InitChannel_TypeDef vdac_init_channel = VDAC_INITCHANNEL_DEFAULT;

  vdac_init.prescaler = VDAC_PrescaleCalc(1000000, true, 0);
  VDAC_Init(VDAC0, &vdac_init);

  vdac_init_channel.enable = true;
  VDAC_InitChannel(VDAC0, &vdac_init_channel, 0);
}


/*
 *  Set LDMA descriptors for internal codec
 *  (Internal ADC and DAC)
 *  NOTE:  POR version uses an external codec.  Internal codec was used for the prototype only 
 */
static void BM_AE_setupLDMA()
{

  CMU_ClockEnable(cmuClock_LDMA, true);

  LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
  ldmaInit.ldmaInitCtrlNumFixed = 0;

  LDMA_Init(&ldmaInit);

  //  Setup ADC LDMA Transfers and descriptors
  leftRxLDMATransCfg = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_ADC0_SINGLE);

  for (int i = 0; i < BM_AE_NUM_BUFFERS - 1; ++i)
  {
      leftRxLDMADesc[i] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(
                        &(ADC0->SINGLEDATA),
                        BM_AE_buffer[i],
                        AUDIO_BUFFER_SIZE,
                        1);

      leftRxLDMADesc[i].xfer.blockSize = 0;
      leftRxLDMADesc[i].xfer.ignoreSrec = true;
      leftRxLDMADesc[i].xfer.size = ldmaCtrlSizeWord;
      leftRxLDMADesc[i].xfer.doneIfs = true;
  }

  //  Setup last LDMA descriptor to loop back to the first descriptor
  leftRxLDMADesc[BM_AE_NUM_BUFFERS - 1] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_P2M_BYTE(
                                          &(ADC0->SINGLEDATA),
                                          BM_AE_buffer[BM_AE_NUM_BUFFERS - 1],
                                          AUDIO_BUFFER_SIZE,
                                          -(BM_AE_NUM_BUFFERS - 1));

  leftRxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.blockSize = 0;
  leftRxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.ignoreSrec = true;
  leftRxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.size = ldmaCtrlSizeWord;
  leftRxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.doneIfs = true;

  NVIC_ClearPendingIRQ(LDMA_IRQn);
  NVIC_EnableIRQ(LDMA_IRQn);

  LDMA_StartTransfer(ADC_LDMA_CHANNEL, (void *)&leftRxLDMATransCfg, (void *)&leftRxLDMADesc[0]);
}


static void BM_AE_setupDacLDMA()
{
  const uint32_t DAC_BUFFER_OFFSET = BM_AE_NUM_BUFFERS - 2;

  leftTxLDMATransCfg = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_TIMER0_UFOF);

  for (int i = 0; i < BM_AE_NUM_BUFFERS - 1; ++i)
  {
      uint32_t index = (DAC_BUFFER_OFFSET + i) % BM_AE_NUM_BUFFERS;
      leftTxLDMADesc[i] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(
                        BM_AE_buffer[index],
                        &VDAC0->CH0DATA,
                        AUDIO_BUFFER_SIZE,
                        1);

      leftTxLDMADesc[i].xfer.blockSize = 0;
      leftTxLDMADesc[i].xfer.ignoreSrec = true;
      leftTxLDMADesc[i].xfer.size = ldmaCtrlSizeWord;
      leftTxLDMADesc[i].xfer.doneIfs = true;
  }

  leftTxLDMADesc[BM_AE_NUM_BUFFERS - 1] = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_LINKREL_M2P_BYTE(
                                        BM_AE_buffer[BM_AE_NUM_BUFFERS - 3],
                                        &VDAC0->CH0DATA,
                                        AUDIO_BUFFER_SIZE,
                                        -(BM_AE_NUM_BUFFERS - 1));

  leftTxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.blockSize = 0;
  leftTxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.ignoreSrec = true;
  leftTxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.size = ldmaCtrlSizeWord;
  leftTxLDMADesc[BM_AE_NUM_BUFFERS - 1].xfer.doneIfs = true;

  LDMA_StartTransfer(DAC_LDMA_CHANNEL, (void *)&leftTxLDMATransCfg, (void *)&leftTxLDMADesc[0]);
}



// -------------------------------------------------------------------------------------------------------- //
// -------------------------------------------------------------------------------------------------------- //


/*
 *  Initialize audio transport
 *  If the external codec (CS42L52) is used, the sampling frequency parameter is ignored and the sampling frequency is set to 32 kHz
 *  If uC ADC and DAC (internal codec) is used instead, you can specify any fs you need.  Just be sure to also set useInternalCode to 1
 * 
 *  Inputs:
 *    fs:               (Using internal codec only) Set sampling frequency
 *    useInternalCodec: Set to 0 if using external codec.  Set to 1 if using internal codec
 */
BM_Error BM_AE_init(const float32_t fs, int32_t useInternalCodec)
{
  if (fs <= 0)
    return BM_AE_INVALID_FS;

  //  Setup internal codec (sampling timers, ADC and DAC)
  if (useInternalCodec == 1)
  {
    usingInternalCodec = 1;
    BM_AE_setupSamplingTimer(fs);
    BM_AE_setupADC();
    BM_AE_setupDAC();
  }

  //  Setup external codec (CS42L52)
  else
  {
    codecParams.MCLKPort = gpioPortD;
    codecParams.MCLKPin = 9;
    codecParams.MCLKLoc = CMU_ROUTELOC0_CLKOUT0LOC_LOC4;

    codecParams.i2sSCKPort = gpioPortD;
    codecParams.i2sSCKPin = 15;
    codecParams.i2sSCKLoc = USART_ROUTELOC0_CLKLOC_LOC5;

    codecParams.i2sLRCKPort = gpioPortD;
    codecParams.i2sLRCKPin = 12;
    codecParams.i2sLRCKLoc = USART_ROUTELOC0_CSLOC_LOC1;

    codecParams.i2sSDINPort = gpioPortD;
    codecParams.i2sSDINPin = 11;
    codecParams.i2sSDINLoc = USART_ROUTELOC0_RXLOC_LOC2;

    codecParams.i2sSDOUTPort = gpioPortD;
    codecParams.i2sSDOUTPin = 10;
    codecParams.i2sSDOUTLoc = USART_ROUTELOC0_TXLOC_LOC2;

    codecParams.i2cModule = I2C0;

    codecParams.i2cSCLPort = gpioPortB;
    codecParams.i2cSCLPin = 11;
    codecParams.i2cSCLLoc = I2C_ROUTELOC0_SCLLOC_LOC5;

    codecParams.i2cSDAPort = gpioPortB;
    codecParams.i2cSDAPin = 12;
    codecParams.i2cSDALoc = I2C_ROUTELOC0_SDALOC_LOC7;

    codecParams.resetPort = gpioPortB;
    codecParams.resetPin = 13;

    codecParams.isMono = 1;
    codecParams.LDMALeftBuffers = &BM_AE_buffer;
    codecParams.LDMARightBuffers = NULL;  //  Since we are only using mono
    codecParams.ldmaLeftRxDescriptors = leftRxLDMADesc;
    codecParams.ldmaLeftTxDescriptors = leftTxLDMADesc;
    codecParams.ldmaRightRxDescriptors = &rightRxLDMADesc;
    codecParams.ldmaRightTxDescriptors = rightTxLDMADesc;
    codecParams.numBuffers = BM_AE_NUM_BUFFERS;
    codecParams.bufferSize = AUDIO_BUFFER_SIZE;

    int32_t success = MW_Driver_CS42L52_init(&codecParams);
    if (!success)
      return BM_AE_CANNOT_INIT_CODEC;

    success = MW_Driver_CS42L52_enableToneControls(&codecParams, 1);
    if (!success)
      return BM_AE_CODEC_FAIL;

    BM_ISR_LDMA_registerAudioISR(BM_AE_audioISR);

  }

  audioEngineInitialized = 1;

  return BM_NO_ERR;
}


/*
 *  Start audio transport
 *  Start the process of moving audio samples to/from the codec
 */
BM_Error BM_AE_startAudioEngine()
{
  if (audioEngineInitialized == 0)
    return BM_AE_NOT_INITIALIZED;

  if (usingInternalCodec == 1)
  {
    BM_AE_setupLDMA();
    BM_AE_setupDacLDMA();
    TIMER_Enable(TIMER0, true);
  }

  else
    MW_Driver_CS42L52_start(&codecParams);

  return BM_NO_ERR;
}


/*
 *  Stop audio transport
 */
void BM_AE_stopAudioEngine()
{
  if (usingInternalCodec == 1)
  {
    LDMA_StopTransfer(ADC_LDMA_CHANNEL);
    LDMA_StopTransfer(DAC_LDMA_CHANNEL);
  }
  else
    MW_Driver_CS42L52_stop(&codecParams);

  BM_AE_bufferCurrentlyProcessing = 0;
  currentInputBuffer = 0;
}


/*
 *  Set the codec to active or low power mode
 *  This holds the RESET pin on the codec high or low
 * 
 *  Inputs:
 *    active:   0:  Set codec to low power mode, 1:  Set codec to active mode
 */
void BM_AE_setCodecActive(int32_t active)
{
  if (active)
    MW_Driver_CS42L52_setReset(&codecParams, active);
  else
    MW_Driver_CS42L52_setReset(&codecParams, 0);
}


/*
 *  Set the input gain from -96 to 24 dB.
 *  External codec only!  If using the internal codec, nothing will happen
 * 
 *  Inputs:
 *    volumeDB:   Volume in dB.  Values past -96 or 24 dB will be clipped
 */
void BM_AE_setADCVolume(int32_t volumeDB)
{
  if (!usingInternalCodec)
  {
    if (volumeDB > 24)
      volumeDB = 24;

    if (volumeDB < -96)
      volumeDB = -96;

    int8_t vol = volumeDB;
    MW_Driver_CS42L52_setADCVolume(&codecParams, volumeDB);
  }
}


/*
 *  Set the master volume from -102 to 12 dB
 *  External codec only!  If using the internal codec, nothing will happen
 * 
 *  Inputs:
 *    volumeDB:   Volume in dB.  Values past -102 or 12 dB will be clipped
 */
void BM_AE_setMasterVolume(int32_t volumeDB)
{
  if (!usingInternalCodec)
  {
    if (volumeDB > 12)
      volumeDB = 12;

    if (volumeDB < -102)
      volumeDB = -102;

    int8_t vol = volumeDB;
    
    //  The master volume increment is 0.5 dB (1 LSB = 0.5 dB) so vol needs to be multiplied by 2 
    //  when passed into MW_Driver_CS42L52_setMasterVolume()
    vol = vol << 1;
    MW_Driver_CS42L52_setMasterVolume(&codecParams, vol);
  }
}


/*
 *  For use with the external codec only!
 *  
 *  Inputs:
 *    mute:   0: mute off, 1: mute on
 */
void BM_AE_setMute(int8_t mute)
{
  if (!usingInternalCodec)
    MW_Driver_CS42L52_setHeadphoneVolume(&codecParams, mute);
}


void BM_AE_setBassGain(const float32_t gainDB)
{
  if (!usingInternalCodec)
    MW_Driver_CS42L52_setBassGain(&codecParams, gainDB);
}


void BM_AE_setTrebleGain(const float32_t gainDB)
{
  if (!usingInternalCodec)
    MW_Driver_CS42L52_setTrebleGain(&codecParams, gainDB);
}

/*
 *  Bypass the analog input from the conversion blocks and route signal directly to output
 *  
 *  Inputs:
 *    byPass:   0: bypass off, 1: bypass on
 */
BM_Error BM_AE_setAudioBypass(const int32_t byPass)
{
  if (byPass > 1 || byPass < 0)
    return BM_INVALID_PARAM;

  if (!MW_Driver_CS42L52_setAudioPassthrough(&codecParams, byPass))
    return BM_AE_CODEC_FAIL;

  return BM_NO_ERR;
}


void BM_AE_clearBuffers()
{
  for (int32_t i = 0; i < BM_AE_NUM_BUFFERS; ++i)
    arm_fill_q15(0, BM_AE_buffer[i], AUDIO_BUFFER_SIZE);
}


void BM_AE_audioISR()
{
  uint32_t flag = LDMA->IF;
  if (flag & (1 << 0))
  {
    //  TODO:  Make audio buffer overrun detection more robust
    //  Check to see if processing takes too long
    //  Halt the transport if processing is still happening after input buffer is filled
  //  if (BM_AE_bufferCurrentlyProcessing == 1)
  //  {
  //   //  BM_AE_stopAudioEngine();
  //   //  BM_UserIO_setAlertLED();
  //  }

   BM_ServiceQueue_addItemToServiceQueue(BM_SERVICE_AUDIO, BM_AE_buffer[currentInputBuffer]);

   currentInputBuffer = (currentInputBuffer + 1) % BM_AE_NUM_BUFFERS;

    LDMA->IFC |= (1 << 0);
  }


  if (flag & (1 << 1))
  {
    //  If processing takes too long, this backlogs the audio transport
    //  Halt the transport if this occurs and optimize processing
  //  if (BM_AE_bufferCurrentlyProcessing == 1)
  //  {
  //    BM_AE_stopAudioEngine();
  //   //  BM_UserIO_setAlertLED();
  //  }

    LDMA->IFC |= (1 << 1);
  }
}


