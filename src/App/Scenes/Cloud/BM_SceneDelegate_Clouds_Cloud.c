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


#include "BM_SceneDelegate_Clouds_Cloud.h"

#define BM_SCENE_CLOUD_CLOUDS_LFO_AMPLITUDE 3.f
#define BM_SCENE_CLOUD_CLOUDS_LFO_FREQ 0.5f


static int32_t      _COLLISION_BOUND_DELTA = 15;
static float32_t    _CLOUD_VELOCITY = 2.3f;
static int32_t      _EXPRESSION_X_OFFSET = 20;
static int32_t      _EXPRESSION_Y_OFFSET = 10;

static float32_t      _CLOUD0_TOP_YPOS = 54;
static float32_t      _CLOUD0_BOTTOM_YPOS = 90;
static float32_t      _CLOUD1_TOP_YPOS = 40;
static float32_t      _CLOUD1_BOTTOM_YPOS = 76;

static BM_Scene_Clouds_Cloud _clouds[BM_SCENE_CLOUDS_NUM_CLOUDS];


static BM_Error BM_SceneDelegate_Clouds_Cloud_initCloud(BM_Scene_Clouds_Cloud *cloud, int32_t xPos, int32_t yPos)
{
    if (cloud == NULL)
        return BM_NULLPTR;

    cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL;

    cloud->spriteAttributes.currentXPos = xPos;
    cloud->spriteAttributes.currentYPos = yPos;
    cloud->spriteAttributes.currentBitmap = cloud->cloudOutlineBitmap;
    cloud->spriteAttributes.currentMask = cloud->cloudOutlineMask;
    cloud->spriteAttributes.dx = 0;
    cloud->spriteAttributes.dy = 0;
    cloud->spriteAttributes.g = 0;
    cloud->spriteAttributes.width = cloud->cloudWidth;
    cloud->spriteAttributes.height = cloud->cloudHeight;
    cloud->spriteAttributes.isMidAir = 0;
    cloud->spriteAttributes.isContacting = 0;
    cloud->spriteAttributes.colliderBoundsOriginDy = _COLLISION_BOUND_DELTA;
    cloud->spriteAttributes.colliderBoundsHeight = cloud->cloudHeight - _COLLISION_BOUND_DELTA;

    cloud->currentExpressionBitmap = cloud->normalExpressionBitmap;
    cloud->currentExpressionWidth = cloud->normalExpressionWidth;
    cloud->currentExpressionHeight = cloud->normalExpressionHeight;

    cloud->clawAttributes.currentBitmap = BM_ASSETS_CLOUDS_GRABBER;
    cloud->clawAttributes.currentMask = BM_ASSETS_CLOUDS_GRABBER_MASK;
    cloud->clawAttributes.width = BM_ASSETS_CLOUDS_GRABBER_WIDTH;
    cloud->clawAttributes.height = BM_ASSETS_CLOUDS_GRABBER_HEIGHT;

    cloud->pc = NULL;

    int32_t success = MW_AFXUnit_Utils_FastSine_init(&cloud->oscillator, cloud->lfoAmplitude, cloud->lfoFreq, FRAME_RATE);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;

    return BM_NO_ERR;
}


static void BM_SceneDelegate_Clouds_Cloud_updateCloud(BM_Scene_Clouds_Cloud *cloud)
{
    if (cloud == NULL)
        return;

    switch(cloud->currentState)
    {
        //  Normal state, cloud floats at top positin and oscillates
        case BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL:
        {
            MW_AFXUnit_Utils_FastSine_update(&cloud->oscillator);
            BM_Sprite_setPosition(&cloud->spriteAttributes, cloud->spriteAttributes.currentXPos, cloud->topYPos + cloud->oscillator.s[1]);
            break;
        }

        //  Cat is on cloud, cloud moves down
        case BM_SCENE_CLOUDS_CLOUD_STATE_PUSH:
        {
            BM_Sprite_update(&cloud->spriteAttributes);
            
            BM_Sprite_setPosition(cloud->pc, cloud->pc->currentXPos, cloud->spriteAttributes.currentYPos + cloud->spriteAttributes.colliderBoundsOriginDy - cloud->pc->height);

            if (cloud->spriteAttributes.currentYPos >= cloud->bottomYPos)
            {
                BM_Sprite_setPosition(&cloud->spriteAttributes, cloud->spriteAttributes.currentXPos, cloud->bottomYPos);
                BM_Sprite_setPosition(cloud->pc, cloud->pc->currentXPos, cloud->spriteAttributes.currentYPos + cloud->spriteAttributes.colliderBoundsOriginDy - cloud->pc->height);
                BM_Sprite_setVelocity(&cloud->spriteAttributes, 0, 0);
                BM_Sprite_setVelocity(cloud->pc, 0, 0);

                MW_AFXUnit_Utils_FastSine_init(&cloud->oscillator, -cloud->lfoAmplitude, cloud->lfoFreq, FRAME_RATE);

                cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_BOTTOM;
            }
            
            break;
        }

        //  Cat is on cloud, cloud is at the bottom and oscillates
        case BM_SCENE_CLOUDS_CLOUD_STATE_BOTTOM:
        {
            MW_AFXUnit_Utils_FastSine_update(&cloud->oscillator);
            BM_Sprite_setPosition(&cloud->spriteAttributes, cloud->spriteAttributes.currentXPos, cloud->bottomYPos + cloud->oscillator.s[1]);

            BM_Sprite_setPosition(cloud->pc, cloud->pc->currentXPos, cloud->spriteAttributes.currentYPos + cloud->spriteAttributes.colliderBoundsOriginDy - cloud->pc->height);
            break;
        }

        //  Cat jumps off cloud, cloud rises
        case BM_SCENE_CLOUDS_CLOUD_STATE_RISE:
        {
            BM_Sprite_update(&cloud->spriteAttributes);

            if (cloud->spriteAttributes.currentYPos <= cloud->topYPos)
            {
                BM_Sprite_setPosition(&cloud->spriteAttributes, cloud->spriteAttributes.currentXPos, cloud->topYPos);
                BM_Sprite_setVelocity(&cloud->spriteAttributes, 0, 0);
                MW_AFXUnit_Utils_FastSine_init(&cloud->oscillator, cloud->lfoAmplitude, cloud->lfoFreq, FRAME_RATE);
                cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL;
            }

            break;
        }

        //  If cat falls off screen.  Start cat retrieval
        case BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_DOWN:
        {
            BM_Sprite_update(&cloud->clawAttributes);

            //  If the claw reaches the bottom, change state and setup appropriate bitmaps
            //  Not only does the claw need to be changed but now the PC sprite must appear
            if (cloud->clawAttributes.currentYPos >= BM_DISPLAY_HEIGHT)
            {
                cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_UP;
                BM_Sprite_setVelocity(&cloud->clawAttributes, 0, -_CLOUD_VELOCITY);
                BM_Sprite_setPosition(cloud->pc, cloud->spriteAttributes.currentXPos + 5, cloud->clawAttributes.currentYPos + 10);
                BM_Sprite_setVelocity(cloud->pc, 0, -_CLOUD_VELOCITY);
            }

            break;
        }

        case BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_UP:
        {
            BM_Sprite_update(&cloud->clawAttributes);

            if (cloud->clawAttributes.currentYPos <= cloud->spriteAttributes.currentYPos + 5)
                BM_Sprite_setVelocity(&cloud->clawAttributes, 0, 0);
            
            if ((cloud->pc->currentYPos + cloud->pc->height) <= (cloud->spriteAttributes.currentYPos + _COLLISION_BOUND_DELTA))
            {
                cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL;
                BM_Sprite_setVelocity(cloud->pc, 0, 0);
            }

            break;
        }

        default:
            break;
    }
}


static void BM_SceneDelegate_Clouds_Cloud_drawCloud(BM_Scene_Clouds_Cloud *cloud)
{
    if (cloud == NULL)
        return;

    if (cloud->currentState == BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_DOWN)
    {
        //  Draw claw arm
        BM_RE_drawRectangleNotFilled(cloud->spriteAttributes.currentXPos + 25,
                            cloud->spriteAttributes.currentYPos + 15,
                            6,
                            cloud->clawAttributes.currentYPos - (cloud->spriteAttributes.currentYPos + 15));

        //  Draw 'claw head'
        BM_RE_drawSprite(cloud->clawAttributes.currentBitmap, 
                        cloud->clawAttributes.currentMask,
                        cloud->clawAttributes.currentXPos,
                        cloud->clawAttributes.currentYPos,
                        cloud->clawAttributes.width,
                        cloud->clawAttributes.height);
    }

    if (cloud->currentState == BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_UP)
    {
        //  Draw claw arm
        BM_RE_drawRectangleNotFilled(cloud->spriteAttributes.currentXPos + 25,
                            cloud->spriteAttributes.currentYPos + 15,
                            6,
                            cloud->clawAttributes.currentYPos - (cloud->spriteAttributes.currentYPos + 15));

        //  Draw 'claw head'
        BM_RE_drawSprite(cloud->clawAttributes.currentBitmap, 
                        cloud->clawAttributes.currentMask,
                        cloud->clawAttributes.currentXPos,
                        cloud->clawAttributes.currentYPos,
                        cloud->clawAttributes.width,
                        cloud->clawAttributes.height);
    }

    //  "Build" the cloud by combining the outline and expressions
    BM_RE_drawSprite(cloud->cloudOutlineBitmap,
                     cloud->cloudOutlineMask,
                     cloud->spriteAttributes.currentXPos,
                     cloud->spriteAttributes.currentYPos,
                     cloud->cloudWidth,
                     cloud->cloudHeight);

    BM_RE_drawSprite(cloud->currentExpressionBitmap,
                     NULL,
                     cloud->spriteAttributes.currentXPos + _EXPRESSION_X_OFFSET,
                     cloud->spriteAttributes.currentYPos + _EXPRESSION_Y_OFFSET,
                     cloud->currentExpressionWidth,
                     cloud->currentExpressionHeight);


}


// ------------------------------------------------------------------- //
// ------------------------------------------------------------------- //


BM_Error BM_SceneDelegate_Clouds_Cloud_init()
{
        //  Initialize clouds
    _clouds[0].cloudOutlineBitmap = BM_ASSETS_CLOUDS_CLOUD0;
    _clouds[0].cloudOutlineMask = BM_ASSETS_CLOUDS_CLOUD0_MASK;
    _clouds[0].cloudWidth = BM_ASSETS_CLOUDS_CLOUD0_WIDTH;
    _clouds[0].cloudHeight = BM_ASSETS_CLOUDS_CLOUD0_HEIGHT;
    _clouds[0].spriteAttributes.colliderBoundsOriginDx = 0;
    _clouds[0].spriteAttributes.colliderBoundsWidth = BM_ASSETS_CLOUDS_CLOUD0_WIDTH - 15;
    _clouds[0].normalExpressionBitmap = BM_ASSETS_CLOUDS_CLOUD0_NORMAL_EXPRESSION;
    _clouds[0].normalExpressionWidth = BM_ASSETS_CLOUDS_CLOUD0_NORMAL_EXPRESSION_WIDTH;
    _clouds[0].normalExpressionHeight = BM_ASSETS_CLOUDS_CLOUD0_NORMAL_EXPRESSION_HEIGHT;
    _clouds[0].pushExpressionBitmap = BM_ASSETS_CLOUDS_CLOUD0_PUSH_EXPRESSION;
    _clouds[0].pushExpressionWidth = BM_ASSETS_CLOUDS_CLOUD0_PUSH_EXPRESSION_WIDTH;
    _clouds[0].pushExpressionHeight = BM_ASSETS_CLOUDS_CLOUD0_PUSH_EXPRESSION_HEIGHT;
    _clouds[0].topYPos = _CLOUD0_TOP_YPOS;
    _clouds[0].bottomYPos = _CLOUD0_BOTTOM_YPOS;
    _clouds[0].lfoAmplitude = BM_SCENE_CLOUD_CLOUDS_LFO_AMPLITUDE;
    _clouds[0].lfoFreq = BM_SCENE_CLOUD_CLOUDS_LFO_FREQ;

    BM_Error error = BM_SceneDelegate_Clouds_Cloud_initCloud(&_clouds[0], 0, _CLOUD0_TOP_YPOS);
    if (error != BM_NO_ERR)
        return error;

    _clouds[1].cloudOutlineBitmap = BM_ASSETS_CLOUDS_CLOUD1;
    _clouds[1].cloudOutlineMask = BM_ASSETS_CLOUDS_CLOUD1_MASK;
    _clouds[1].cloudWidth = BM_ASSETS_CLOUDS_CLOUD1_WIDTH;
    _clouds[1].cloudHeight = BM_ASSETS_CLOUDS_CLOUD1_HEIGHT;
    _clouds[1].spriteAttributes.colliderBoundsOriginDx = 10;
    _clouds[1].spriteAttributes.colliderBoundsWidth = BM_ASSETS_CLOUDS_CLOUD1_WIDTH - 10;
    _clouds[1].normalExpressionBitmap = BM_ASSETS_CLOUDS_CLOUD1_NORMAL_EXPRESSION;
    _clouds[1].normalExpressionWidth = BM_ASSETS_CLOUDS_CLOUD1_NORMAL_EXPRESSION_WIDTH;
    _clouds[1].normalExpressionHeight = BM_ASSETS_CLOUDS_CLOUD1_NORMAL_EXPRESSION_HEIGHT;
    _clouds[1].pushExpressionBitmap = BM_ASSETS_CLOUDS_CLOUD1_PUSH_EXPRESSION;
    _clouds[1].pushExpressionWidth = BM_ASSETS_CLOUDS_CLOUD1_PUSH_EXPRESSION_WIDTH;
    _clouds[1].pushExpressionHeight = BM_ASSETS_CLOUDS_CLOUD1_PUSH_EXPRESSION_HEIGHT;
    _clouds[1].topYPos = _CLOUD1_TOP_YPOS;
    _clouds[1].bottomYPos = _CLOUD1_BOTTOM_YPOS;
    _clouds[1].lfoAmplitude = BM_SCENE_CLOUD_CLOUDS_LFO_AMPLITUDE;
    _clouds[1].lfoFreq = BM_SCENE_CLOUD_CLOUDS_LFO_FREQ;

    error = BM_SceneDelegate_Clouds_Cloud_initCloud(&_clouds[1], 64, _CLOUD1_TOP_YPOS);
    if (error != BM_NO_ERR)
        return error;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Clouds_Cloud_update()
{
    for (int32_t i = 0; i < BM_SCENE_CLOUDS_NUM_CLOUDS; ++i)
        BM_SceneDelegate_Clouds_Cloud_updateCloud(&_clouds[i]);
}


void BM_SceneDelegate_Clouds_Cloud_draw()
{
    for (int32_t i = 0; i < BM_SCENE_CLOUDS_NUM_CLOUDS; ++i)
        BM_SceneDelegate_Clouds_Cloud_drawCloud(&_clouds[i]);
}


/*
 *  When the PC makes contact with the cloud, the cloud takes 'ownership' of the PC's yPos
 *  and the PC's rate of descent is the same as the cloud's to show the PC descending with the cloud
 */
void BM_SceneDelegate_Clouds_Cloud_pcContact(BM_Scene_Clouds_Cloud *cloud, BM_Sprite *pc)
{
    if (cloud == NULL || pc == NULL)
        return;

    BM_Sprite_setPosition(cloud->pc, cloud->pc->currentXPos, cloud->spriteAttributes.currentYPos + cloud->spriteAttributes.colliderBoundsOriginDy - cloud->pc->height);
    
    if (cloud->currentState == BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL || cloud->currentState == BM_SCENE_CLOUDS_CLOUD_STATE_RISE)
    {
        cloud->pc = pc;
        cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_PUSH;
        cloud->currentExpressionBitmap = cloud->pushExpressionBitmap;
        cloud->currentExpressionWidth = cloud->pushExpressionWidth;
        cloud->currentExpressionHeight = cloud->pushExpressionHeight;
        BM_Sprite_setAcceleration(cloud->pc, 0, 0);
        BM_Sprite_setVelocity(cloud->pc, cloud->pc->dx, 0);
        BM_Sprite_setVelocity(&cloud->spriteAttributes, 0, _CLOUD_VELOCITY);
    }
}   


void BM_SceneDelegate_Clouds_Cloud_pcLeaveContact(BM_Scene_Clouds_Cloud *cloud)
{
    if (cloud == NULL)
        return;
    
    cloud->pc = NULL;
    cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_RISE;
    cloud->currentExpressionBitmap = cloud->normalExpressionBitmap;
    cloud->currentExpressionWidth = cloud->normalExpressionWidth;
    cloud->currentExpressionHeight = cloud->normalExpressionHeight;
    BM_Sprite_setVelocity(&cloud->spriteAttributes, 0, -_CLOUD_VELOCITY);
}


void BM_SceneDelegate_Clouds_Cloud_startRetrieval(BM_Scene_Clouds_Cloud *cloud, BM_Sprite *pc)
{
    if (cloud == NULL)
        return;

    cloud->currentState = BM_SCENE_CLOUDS_CLOUD_STATE_CLAW_DOWN;
    cloud->pc = pc;

    BM_Sprite_setPosition(&cloud->clawAttributes, cloud->spriteAttributes.currentXPos + 5, cloud->spriteAttributes.currentYPos + _COLLISION_BOUND_DELTA);
    BM_Sprite_setVelocity(&cloud->clawAttributes, 0, _CLOUD_VELOCITY);
}


BM_Scene_Clouds_Cloud* BM_SceneDelegate_Clouds_Cloud_getClouds()
{
    return _clouds;
}
