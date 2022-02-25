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

#ifndef SRC_BM_COMMON_H_
#define SRC_BM_COMMON_H_

#include "arm_math.h"
#include "MW_AFXUnit_MiscUtils.h"

#define INCREMENT_CLIP(value, increment, limit) \
        value += increment; \
        if (value >= limit) value = limit;

#define DECREMENT_CLIP(value, decrement, limit) \
        value -= decrement; \
        if (value < limit) value = limit;

#define INCREMENT_WRAP(value, increment, limit) \
        value += increment; \
        if (value >= limit) value = 0;

#define DECREMENT_WRAP(value, decrement, limit, wrapValue) \
        value -= decrement; \
        if (value < limit) value = wrapValue;      

#define BM_ASSERT(expr) do { BM_Error success = expr; if (success != BM_NO_ERR) while(1); } while(0);

#define BM_UNIT_TEST_ASSERT(expr) do { BM_Error success = expr; if (success != BM_NO_ERR) return BM_UNIT_TEST_FAIL;} while(0);
#define BM_TEST_FOR_FAIL(expr) do { BM_Error success = expr; if (success == BM_NO_ERR) return BM_UNIT_TEST_FAIL; } while(0);
#define BM_TEST_FOR_PASS(expr) do { BM_Error success = expr; if (success != BM_NO_ERR) return success; } while(0);
#define BM_TEST_EXPECT(expr) do {if (!expr) return BM_UNIT_TEST_FAIL; } while(0);

#define FS 32000.f
#define AUDIO_BUFFER_SIZE 1024

#define FRAME_RATE 30.f

#define BM_DISP_ENABLE_PORT gpioPortF
#define BM_DISP_ENABLE_PIN  2

#define BM_LDMA_CH_DISP 4

typedef enum
{
  BM_DIR_NONE = 0,
  BM_DIR_UP,
  BM_DIR_DOWN,
  BM_DIR_LEFT,
  BM_DIR_RIGHT,
  BM_DIR_INSIDE
}BM_Direction;


typedef struct
{
  BM_Direction first;
  BM_Direction second;
}BM_DirectionPair;


typedef enum
{
  BM_STATE_SLEEP = 0,
  BM_STATE_IDLE,
  BM_STATE_ACTIVE,
  BM_STATE_MENU,
  BM_STATE_ERROR
}BM_SystemState;


extern float32_t BM_FADE_IN_ENVELOPE[AUDIO_BUFFER_SIZE];
extern float32_t BM_FADE_OUT_ENVELOPE[AUDIO_BUFFER_SIZE];


void  BM_Common_init();

//      Temporary
void convertTimeToChar(int32_t val, char* output);
char itoa(int32_t val);


#endif /* SRC_BM_COMMON_H_ */
