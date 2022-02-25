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

#ifndef SRC_SCENES_CLOUD_BM_SCENEDELEGATE_CLOUDS_TWINKLE_H_
#define SRC_SCENES_CLOUD_BM_SCENEDELEGATE_CLOUDS_TWINKLE_H_

#include "arm_math.h"
#include "BM_RenderEngine.h"

#define BM_SCENE_CLOUDS_NUM_STARS 17

typedef struct
{
    int32_t xPos, yPos;
    int32_t width, height;
    int32_t twinklePeriod;
    int32_t counter;
    int32_t state;
}BM_Scene_Clouds_Twinkle;


void BM_SceneDelegate_Twinkle_init();
void BM_SceneDelegate_Twinkle_update();
void BM_SceneDelegate_Twinkle_draw();

#endif /* SRC_SCENES_CLOUD_BM_SCENEDELEGATE_CLOUDS_TWINKLE_H_ */
