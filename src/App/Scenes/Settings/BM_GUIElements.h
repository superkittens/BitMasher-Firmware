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

#pragma once

#include "arm_math.h"
#include "BM_RenderEngine.h"
#include "MW_AFXUnit_MiscUtils.h"

struct BM_GUIObject_Slider;

typedef struct
{
    const char  *text;
    float32_t   minSliderValue, maxSliderValue;
    float32_t   currentValue;
    float32_t   deltaValue;
    void        (*onSliderChange)(struct BM_GUIObject_Slider*, const BM_Direction);
    int32_t     sliderWidth;
    int32_t     xPos, yPos;
    int32_t     active;
}BM_GUIObject_Slider;

typedef struct
{
    const char  *text;
    int32_t     xPos, yPos;
    int32_t     active;
}BM_GUIObject_Selection;


void            BM_GUIObject_Slider_draw(const BM_GUIObject_Slider * const slider);
const int32_t   BM_GUIObject_Slider_getSliderLength();
const int32_t   BM_GUIObject_Slider_getSliderHeight();

void            BM_GUIObject_Selection_draw(const BM_GUIObject_Selection * const selection);