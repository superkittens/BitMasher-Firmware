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


#include "BM_Common.h"

float32_t BM_FADE_IN_ENVELOPE[AUDIO_BUFFER_SIZE];
float32_t BM_FADE_OUT_ENVELOPE[AUDIO_BUFFER_SIZE];

void BM_Common_init()
{
    MW_AFXUnit_Utils_createFadeEnvelope(BM_FADE_IN_ENVELOPE, AUDIO_BUFFER_SIZE, 0);
    MW_AFXUnit_Utils_createFadeEnvelope(BM_FADE_OUT_ENVELOPE, AUDIO_BUFFER_SIZE, 1);
}

char itoa(int32_t val)
{
    return '0' + val;
}

void convertTimeToChar(int32_t val, char* output)
{
    output[0] = itoa(val / 10);
    output[1] = itoa(val % 10);
}