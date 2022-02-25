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

#ifndef SRC_RENDERENGINE_BM_RENDERENGINE_H_
#define SRC_RENDERENGINE_BM_RENDERENGINE_H_

#include "MW_Driver_LS013B7DH03.h"
#include "BM_ErrorCodes.h"
#include "BM_ServiceQueue.h"
#include "BM_DebugServices.h"
#include "BM_PeripheralCommon.h"
#include "BM_ISR.h"
#include "BM_RTCModule.h"
#include "BM_Common.h"
 
#include "BM_Assets_Clock.h"

#include "em_timer.h"
#include "em_letimer.h"
#include "em_cmu.h"

#include "font.h"

#include "arm_math.h"


#define BM_DISPLAY_WIDTH 128
#define BM_DISPLAY_HEIGHT 128

BM_Error    BM_RE_init(float32_t frameRate);
BM_Error    BM_RE_startVSYNC();
BM_Error    BM_RE_stopRenderEngine();
void        BM_RE_drawBackground(uint8_t *backgroundBitmap);
void        BM_RE_drawLightBackground();
void        BM_RE_drawDarkBackground();
void        BM_RE_drawSprite(uint8_t *spriteBitmap, uint8_t *spriteMask, int32_t xPos, int32_t yPos, int32_t width, int32_t height);
void        BM_RE_drawSpriteNoMask(uint8_t *spriteBitmap, int32_t xPos, int32_t yPos, int32_t width, int32_t height);
void        BM_RE_drawPixel(const int32_t xPos, const int32_t yPos, const int32_t colour);
void        BM_RE_drawTime(const BM_RTCModule_TimeBCD time, const int32_t yPos);

void        BM_RE_updateDisplay(const bool asyncWrite);
uint8_t*    BM_RE_getFrameBuffer();

void        BM_RE_drawRectangleFilled(int32_t x, int32_t y, int32_t width, int32_t height);
void        BM_RE_drawRectangleNotFilled(const int32_t x, const int32_t y, const int32_t width, const int32_t height);

void        BM_RE_drawString(const int32_t xPos, const int32_t yPos, const char *str);

void        BM_RE_displayUpdateISR();
void        BM_RE_displaySignallingTimerISR();
void        BM_RE_VSYNCISR();



#endif /* SRC_RENDERENGINE_BM_RENDERENGINE_H_ */
