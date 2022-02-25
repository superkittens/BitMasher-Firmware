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

#ifndef SRC_SPRITES_BM_SCENEDELEGATE_CLOUDS_CLOUD_H_
#define SRC_SPRITES_BM_SCENEDELEGATE_CLOUDS_CLOUD_H_


#include "arm_math.h"
#include "BM_Sprite.h"
#include "BM_RenderEngine.h"
#include "BM_Common.h"
#include "BM_ErrorCodes.h"
#include "MW_DSP_CombFilter.h"
#include "MW_AFXUnit_MiscUtils.h"
#include "BM_Assets_Clouds.h"

#include "BM_DebugServices.h"

#define BM_SCENE_CLOUDS_NUM_CLOUDS 2


typedef enum
{
    BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL = 0,
    BM_SCENE_CLOUDS_CLOUD_STATE_PUSH,
    BM_SCENE_CLOUDS_CLOUD_STATE_BOTTOM,
    BM_SCENE_CLOUDS_CLOUD_STATE_RISE,
    BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_DOWN,
    BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_UP,
    BM_SCENE_CLOUDS_CLOUD_STATE_INVALID

}BM_Scene_Clouds_Cloud_State;


typedef struct
{
    uint8_t     *cloudOutlineBitmap;
    uint8_t     *cloudOutlineMask;
    uint8_t     *normalExpressionBitmap;
    uint8_t     *pushExpressionBitmap;

    int32_t     cloudWidth, cloudHeight;
    int32_t     normalExpressionWidth, normalExpressionHeight;
    int32_t     pushExpressionWidth, pushExpressionHeight;

    int32_t     topYPos;
    int32_t     bottomYPos;

    float32_t   lfoFreq;
    float32_t   lfoAmplitude;    

    BM_Scene_Clouds_Cloud_State  currentState;

    BM_Sprite   spriteAttributes; 
    BM_Sprite   clawAttributes;
    BM_Sprite   *pc;
    // int32_t     normalYPos;
    uint8_t     *currentExpressionBitmap;
    int32_t     currentExpressionWidth, currentExpressionHeight;

    MW_AFXUnit_FastSine         oscillator;

}BM_Scene_Clouds_Cloud;


BM_Error    BM_SceneDelegate_Clouds_Cloud_init();
void        BM_SceneDelegate_Clouds_Cloud_update();
void        BM_SceneDelegate_Clouds_Cloud_draw();

void        BM_SceneDelegate_Clouds_Cloud_pcContact(BM_Scene_Clouds_Cloud *cloud, BM_Sprite *pc);
void        BM_SceneDelegate_Clouds_Cloud_pcLeaveContact(BM_Scene_Clouds_Cloud *cloud);
void        BM_SceneDelegate_Clouds_Cloud_startRetrieval(BM_Scene_Clouds_Cloud *cloud, BM_Sprite *pc);
BM_Scene_Clouds_Cloud* BM_SceneDelegate_Clouds_Cloud_getClouds();





#endif /* SRC_SPRITES_BM_SCENEDELEGATE_CLOUDS_CLOUD_H_ */
