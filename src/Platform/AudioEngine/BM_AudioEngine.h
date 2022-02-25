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

#ifndef SRC_AUDIOENGINE_BM_AUDIOENGINE_H_
#define SRC_AUDIOENGINE_BM_AUDIOENGINE_H_

#include "BM_Common.h"
#include "BM_ErrorCodes.h"
#include "BM_ServiceQueue.h"
#include "BM_DebugServices.h"
#include "BM_PeripheralCommon.h"
#include "BM_ISR.h"
#include "MW_Driver_CS42L52.h"

#include "arm_math.h"
#include "em_adc.h"
#include "em_vdac.h"
#include "em_timer.h"
#include "em_cmu.h"
#include "em_ldma.h"
#include "em_prs.h"

#define BM_AE_NUM_BUFFERS   3
#define BM_AE_QUEUE_SIZE    BM_AE_NUM_BUFFERS

extern volatile uint32_t  BM_AE_bufferCurrentlyProcessing;

//  Audio Buffers and Queues
extern volatile int16_t BM_AE_buffer[BM_AE_NUM_BUFFERS][AUDIO_BUFFER_SIZE];


BM_Error    BM_AE_init(const float32_t fs, int32_t useInternalCodec);
BM_Error    BM_AE_startAudioEngine();
void        BM_AE_stopAudioEngine();
void        BM_AE_setCodecActive(int32_t active);

void        BM_AE_setADCVolume(int32_t volumeDB);
void        BM_AE_setMasterVolume(int32_t volumeDB);
void        BM_AE_setMute(int8_t mute);
void        BM_AE_setBassGain(const float32_t gainDB);
void        BM_AE_setTrebleGain(const float32_t gainDB);

BM_Error    BM_AE_setAudioBypass(const int32_t byPass);

void        BM_AE_clearBuffers();

//  ISR Routines
void ADC0_IRQHandler();
void TIMER0_IRQHandler();
void BM_AE_audioISR();

#endif /* SRC_AUDIOENGINE_BM_AUDIOENGINE_H_ */
