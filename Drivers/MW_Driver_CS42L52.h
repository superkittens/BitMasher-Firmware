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

#ifndef SRC_RENDERENGINE_MW_DRIVER_CS42L52_H_
#define SRC_RENDERENGINE_MW_DRIVER_CS42L52_H_

#include "arm_math.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "em_usart.h"
#include "em_gpio.h"
#include "em_ldma.h"

//  I2C Address is dependent on the state of the AD0 pin
//  If AD0 is LOW, address is 0x94
//  If AD0 is HIGH, address is 0x96
#define MW_DRIVER_CS42L52_I2C_ADDR 0x94
#define MW_DRIVER_CS42L52_I2C_FSCK 100000

//  MCLK frequencies and associated scaling values for EFM32 DPLL
//  Currently, the DPLL doesn't work and uses a contingency method to generate a 12.288 MHz clock
#define MW_DRIVER_CS42L52_DPLL_N 2999
#define MW_DRIVER_CS42L52_DPLL_M 899
#define MW_DRIVER_CS42L52_MCLK_FREQ 12000000
#define MW_DRIVER_CS42L52_FS 32000

#define MW_DRIVER_CS42L52_LEFT_RX_LDMA_CHANNEL 0
#define MW_DRIVER_CS42L52_LEFT_TX_LDMA_CHANNEL 1
#define MW_DRIVER_CS42L52_RIGHT_RX_LDMA_CHANNEL 2
#define MW_DRIVER_CS42L52_RIGHT_TX_LDMA_CHANNEL 3


typedef struct
{
    GPIO_Port_TypeDef   resetPort;
    uint32_t            resetPin;

    I2C_TypeDef         *i2cModule;
    GPIO_Port_TypeDef   i2cSDAPort;
    uint32_t            i2cSDAPin;
    uint32_t            i2cSDALoc;
    GPIO_Port_TypeDef   i2cSCLPort;
    uint32_t            i2cSCLPin;
    uint32_t            i2cSCLLoc;

    GPIO_Port_TypeDef   MCLKPort;
    uint32_t            MCLKPin;
    uint32_t            MCLKLoc;
    GPIO_Port_TypeDef   i2sLRCKPort;
    uint32_t            i2sLRCKPin;
    uint32_t            i2sLRCKLoc;
    GPIO_Port_TypeDef   i2sSDOUTPort;
    uint32_t            i2sSDOUTPin;
    uint32_t            i2sSDOUTLoc;
    GPIO_Port_TypeDef   i2sSDINPort;
    uint32_t            i2sSDINPin;
    uint32_t            i2sSDINLoc;
    GPIO_Port_TypeDef   i2sSCKPort;
    uint32_t            i2sSCKPin;
    uint32_t            i2sSCKLoc;

    int16_t            *LDMALeftBuffers;
    int16_t            *LDMARightBuffers;
    int32_t             numBuffers;
    int32_t             bufferSize;
    int32_t             isMono;
    LDMA_TransferCfg_t  ldmaLeftTxTransferCfg;
    LDMA_TransferCfg_t  ldmaLeftRxTransferCfg;
    LDMA_Descriptor_t   *ldmaLeftTxDescriptors;
    LDMA_Descriptor_t   *ldmaLeftRxDescriptors;
    LDMA_TransferCfg_t  ldmaRightTxTransferCfg;
    LDMA_TransferCfg_t  ldmaRightRxTransferCfg;
    LDMA_Descriptor_t   *ldmaRightTxDescriptors;
    LDMA_Descriptor_t   *ldmaRightRxDescriptors;

    //  Variables to track codec states
    int32_t             isInitialized;
    int32_t             isRunning;

}MW_CS42L52_Params;



int32_t MW_Driver_CS42L52_init(MW_CS42L52_Params *codecParams);
uint8_t MW_Driver_CS42L52_getChipID(MW_CS42L52_Params *codecParams);
uint8_t MW_Driver_CS42L52_getChipRev(MW_CS42L52_Params *codecParams);
int32_t MW_Driver_CS42L52_start(MW_CS42L52_Params *codecParams);
int32_t MW_Driver_CS42L52_stop(MW_CS42L52_Params *codecParams);
int32_t MW_Driver_CS42L52_setReset(MW_CS42L52_Params *codecParams, int32_t state);

void    MW_Driver_CS42L52_setADCVolume(MW_CS42L52_Params *codecParams, int8_t volumeDB);
void    MW_Driver_CS42L52_setMasterVolume(MW_CS42L52_Params *codecParams, int8_t volumeDB);
void    MW_Driver_CS42L52_setHeadphoneVolume(MW_CS42L52_Params *codecParams, int8_t volume);

int32_t MW_Driver_CS42L52_enableToneControls(const MW_CS42L52_Params * const codecParams, const int32_t enable);
void    MW_Driver_CS42L52_setBassGain(const MW_CS42L52_Params * const codecParams, float32_t gainDB);
void    MW_Driver_CS42L52_setTrebleGain(const MW_CS42L52_Params * const codecParams, float32_t gainDB);

int32_t MW_Driver_CS42L52_setAudioPassthrough(const MW_CS42L52_Params * const codecParams, const int32_t byPass);


#endif /* SRC_RENDERENGINE_MW_DRIVER_CS42L52_H_ */
