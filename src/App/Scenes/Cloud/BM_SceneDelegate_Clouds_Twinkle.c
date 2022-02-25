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


#include "BM_SceneDelegate_Clouds_Twinkle.h"


static BM_Scene_Clouds_Twinkle _stars[BM_SCENE_CLOUDS_NUM_STARS];
static int32_t _STARS_X_POS[BM_SCENE_CLOUDS_NUM_STARS] = {1, 28, 41, 8, 119, 99, 106, 50, 81, 53, 35, 122, 10, 99, 48, 22, 124};
static int32_t _STARS_Y_POS[BM_SCENE_CLOUDS_NUM_STARS] = {4, 16, 36, 62, 8, 0, 50, 37, 69, 84, 55, 59, 74, 82, 8, 77, 77};
static int32_t _STARS_TWINKLE_PERIODS[BM_SCENE_CLOUDS_NUM_STARS] = {25, 39, 20, 33, 15, 51, 35, 67, 44, 25, 128, 40, 40, 15, 75, 85, 38};
static int32_t _STARS_INITIAL_COUNTERS[BM_SCENE_CLOUDS_NUM_STARS] = {0, 2, 5, 0, 10, 20, 5, 0, 0, 15, 32, 0, 101, 5, 20, 0, 13};
static int32_t _STARS_INITIAL_STATE[BM_SCENE_CLOUDS_NUM_STARS] = {1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1};



void BM_SceneDelegate_Twinkle_init()
{
    for (int32_t i = 0; i < BM_SCENE_CLOUDS_NUM_STARS; ++i)
    {
        _stars[i].xPos = _STARS_X_POS[i];
        _stars[i].yPos = _STARS_Y_POS[i];
        _stars[i].width = 2;
        _stars[i].height = 2;
        _stars[i].counter = _STARS_INITIAL_COUNTERS[i];
        _stars[i].twinklePeriod = _STARS_TWINKLE_PERIODS[i];
        _stars[i].state = _STARS_INITIAL_STATE[i];
    }
}


void BM_SceneDelegate_Twinkle_update()
{
    for (int32_t i = 0; i < BM_SCENE_CLOUDS_NUM_STARS; ++i)
    {
        _stars[i].counter++;
        if (_stars[i].counter >= _stars[i].twinklePeriod)
        {
            if (_stars[i].state == 1)
                _stars[i].state = 0;
            else
                _stars[i].state = 1;

            _stars[i].counter = 0;
        }
    }
}


void BM_SceneDelegate_Twinkle_draw()
{
    for (int32_t i = 0; i < BM_SCENE_CLOUDS_NUM_STARS; ++i)
    {   
        if (_stars[i].state == 0)
            BM_RE_drawRectangleFilled(_stars[i].xPos, _stars[i].yPos, _stars[i].width, _stars[i].height);
    }
}
