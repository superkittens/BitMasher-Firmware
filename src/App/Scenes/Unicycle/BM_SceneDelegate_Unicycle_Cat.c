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

#include "BM_SceneDelegate_Unicycle_Cat.h"

#define BM_SCENE_UNICYCLE_FRAME_CHANGE_START    30
#define BM_SCENE_UNICYCLE_FRAME_CHANGE_NORMAL   10
#define BM_SCENE_UNICYCLE_FRAME_CHANGE_ACCEL    5

#define BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS       20.f
#define BM_SCENE_UNICYCLE_CAT_ACCEL_XPOS        30.f
#define BM_SCENE_UNICYCLE_CAT_REV_XPOS          5.f

#define BM_SCENE_UNICYCLE_CAT_FISH_FRAME_CHANGE_VALUE   10


static BM_Animation _catAnimation;
static BM_Sprite    _cat;
static BM_Sprite    _catExpression;

static BM_Animation _fishAnimation;
static BM_Sprite    _fish;

static float32_t    _catFrameTimer = 0;
static float32_t    _catFrameChangeValue = BM_SCENE_UNICYCLE_FRAME_CHANGE_NORMAL;
static float32_t    _catFrameChangeVelocity = 0;    //  TODO:  Figure out if we actually need this

static int32_t      _fishFrameTimer = 0;

static BM_Scene_Unicycle_Motion _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;


BM_Error BM_SceneDelegate_Unicycle_Cat_init()
{
    //  Set animation sequences
    _catAnimation.width = BM_ASSETS_UNICYCLE_CAT_WIDTH;
    _catAnimation.height = BM_ASSETS_UNICYCLE_CAT_HEIGHT;
    _catAnimation.frames = BM_ASSETS_UNICYCLE_CAT;
    _catAnimation.frameMasks = BM_ASSETS_UNICYCLE_CAT_MASK;
    _catAnimation.useMask = 1;
    _catAnimation.numFrames = BM_ASSETS_UNICYCLE_CAT_NUM_FRAMES;

    BM_Error err = BM_Animation_init(&_catAnimation, 0);
    if (err != BM_NO_ERR)
        return err;

    _catExpression.currentBitmap = BM_ASSETS_UNICYCLE_EX_NORMAL;
    _catExpression.currentMask = BM_ASSETS_UNICYCLE_EX_NORMAL_MASK;
    _catExpression.width = BM_ASSETS_UNICYCLE_EX_NORMAL_WIDTH;
    _catExpression.height = BM_ASSETS_UNICYCLE_EX_NORMAL_HEIGHT;

    _cat.width = BM_ASSETS_UNICYCLE_CAT_WIDTH;
    _cat.height = BM_ASSETS_UNICYCLE_CAT_HEIGHT;
    _cat.currentXPos = BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS;
    _cat.currentYPos = 40;

    _fishAnimation.width = BM_ASSETS_UNICYCLE_FISH_WIDTH;
    _fishAnimation.height = BM_ASSETS_UNICYCLE_FISH_HEIGHT;
    _fishAnimation.frames = BM_ASSETS_UNICYCLE_FISH;
    _fishAnimation.frameMasks = BM_ASSETS_UNICYCLE_FISH_MASK;
    _fishAnimation.useMask = 1;
    _fishAnimation.numFrames = BM_ASSETS_UNICYCLE_FISH_NUM_FRAMES;

    _fish.currentBitmap = BM_ASSETS_UNICYCLE_FISH;
    _fish.currentMask = BM_ASSETS_UNICYCLE_FISH_MASK;
    _fish.width = BM_ASSETS_UNICYCLE_FISH_WIDTH;
    _fish.height = BM_ASSETS_UNICYCLE_FISH_HEIGHT;

    err = BM_Animation_init(&_fishAnimation, 0);
    if (err != BM_NO_ERR)
        return err;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Unicycle_Cat_setMotionTargets(const float32_t numSteps)
{
    float32_t catVelocity = 0.f;

    switch (_currentMotion)
    {
        case BM_SCENE_UNICYCLE_MOTION_FORWARD:
        {
            _catExpression.currentBitmap = BM_ASSETS_UNICYCLE_EX_NORMAL;
            _catExpression.currentMask = BM_ASSETS_UNICYCLE_EX_NORMAL_MASK;
            _catExpression.width = BM_ASSETS_UNICYCLE_EX_NORMAL_WIDTH;
            _catExpression.height = BM_ASSETS_UNICYCLE_EX_NORMAL_HEIGHT;

            _catFrameChangeValue = BM_SCENE_UNICYCLE_FRAME_CHANGE_NORMAL;

            catVelocity = (BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS - _cat.currentXPos) / numSteps;

            break;
        }
        case BM_SCENE_UNICYCLE_MOTION_REVERSE:
        {
            _catExpression.currentBitmap = BM_ASSETS_UNICYCLE_EX_NORMAL;
            _catExpression.currentMask = BM_ASSETS_UNICYCLE_EX_NORMAL_MASK;
            _catExpression.width = BM_ASSETS_UNICYCLE_EX_NORMAL_WIDTH;
            _catExpression.height = BM_ASSETS_UNICYCLE_EX_NORMAL_HEIGHT;

            _catFrameChangeValue = BM_SCENE_UNICYCLE_FRAME_CHANGE_NORMAL;

            catVelocity = (BM_SCENE_UNICYCLE_CAT_REV_XPOS - _cat.currentXPos) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_ACCELERATE:
        {
            _catExpression.currentBitmap = BM_ASSETS_UNICYCLE_EX_ACCEL;
            _catExpression.currentMask = BM_ASSETS_UNICYCLE_EX_ACCEL_MASK;
            _catExpression.width = BM_ASSETS_UNICYCLE_EX_ACCEL_WIDTH;
            _catExpression.height = BM_ASSETS_UNICYCLE_EX_ACCEL_HEIGHT;

            _catFrameChangeValue = BM_SCENE_UNICYCLE_FRAME_CHANGE_ACCEL;

            catVelocity = (BM_SCENE_UNICYCLE_CAT_ACCEL_XPOS - _cat.currentXPos) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_STOPPING:
        case BM_SCENE_UNICYCLE_MOTION_STOPPED:
        {
            _catExpression.currentBitmap = BM_ASSETS_UNICYCLE_EX_NORMAL;
            _catExpression.currentMask = BM_ASSETS_UNICYCLE_EX_NORMAL_MASK;
            _catExpression.width = BM_ASSETS_UNICYCLE_EX_NORMAL_WIDTH;
            _catExpression.height = BM_ASSETS_UNICYCLE_EX_NORMAL_HEIGHT;

            _catFrameChangeValue = BM_SCENE_UNICYCLE_FRAME_CHANGE_NORMAL;

            catVelocity = (BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS - _cat.currentXPos) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_BRAKE:
        {
            _catExpression.currentBitmap = BM_ASSETS_UNICYCLE_EX_BRAKE;
            _catExpression.currentMask = BM_ASSETS_UNICYCLE_EX_BRAKE_MASK;
            _catExpression.width = BM_ASSETS_UNICYCLE_EX_BRAKE_WIDTH;
            _catExpression.height = BM_ASSETS_UNICYCLE_EX_BRAKE_HEIGHT;

            catVelocity = (BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS - _cat.currentXPos) / numSteps;

            break;
        }

        default:
          break;
    }
        BM_Sprite_setVelocity(&_cat, catVelocity, 0);
}


void BM_SceneDelegate_Unicycle_Cat_update()
{
    //  There should be no animation updates when the cat is not moving
    if (_currentMotion != BM_SCENE_UNICYCLE_MOTION_STOPPED && _currentMotion != BM_SCENE_UNICYCLE_MOTION_BRAKE)
    {
        _catFrameTimer++;
        _fishFrameTimer++;
        if (_catFrameTimer >= _catFrameChangeValue)
        {
            _catFrameTimer = 0;
            BM_Animation_update(&_catAnimation);
        }

        if (_fishFrameTimer >= BM_SCENE_UNICYCLE_CAT_FISH_FRAME_CHANGE_VALUE)
        {
            _fishFrameTimer = 0;
            BM_Animation_update(&_fishAnimation);
        }
    }

    //  Move the cat forward or backward depending on the motion
    if (_currentMotion == BM_SCENE_UNICYCLE_MOTION_ACCELERATE)
    {
        if (_cat.currentXPos >= BM_SCENE_UNICYCLE_CAT_ACCEL_XPOS)
        {
            _cat.currentXPos = BM_SCENE_UNICYCLE_CAT_ACCEL_XPOS;
            BM_Sprite_setVelocity(&_cat, 0, 0);
        }
    }
    else if (_currentMotion == BM_SCENE_UNICYCLE_MOTION_REVERSE)
    {
        if (_cat.currentXPos <= BM_SCENE_UNICYCLE_CAT_REV_XPOS)
        {
            _cat.currentXPos = BM_SCENE_UNICYCLE_CAT_REV_XPOS;
            BM_Sprite_setVelocity(&_cat, 0, 0);
        }
    }
    else
    {
        if (_cat.dx < 0)
        {
            if (_cat.currentXPos <= BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS)
            {
                _cat.currentXPos = BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS;
                BM_Sprite_setVelocity(&_cat, 0, 0);
            }
        }
        if (_cat.dx > 0)
        {
            if (_cat.currentXPos >= BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS)
            {
                _cat.currentXPos = BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS;
                BM_Sprite_setVelocity(&_cat, 0, 0);
            }
        }
    }

    BM_Sprite_update(&_cat);
}


void BM_SceneDelegate_Unicycle_Cat_draw()
{
    BM_RE_drawSprite(BM_Animation_getCurrentFrame(&_catAnimation),
                    BM_Animation_getCurrentFrameMask(&_catAnimation),
                    _cat.currentXPos,
                    _cat.currentYPos,
                    _cat.width,
                    _cat.height);

    BM_RE_drawSprite(_catExpression.currentBitmap,
                    _catExpression.currentMask,
                    _cat.currentXPos + 32,
                    _cat.currentYPos + 8,
                    _catExpression.width,
                    _catExpression.height);

    if (_currentMotion == BM_SCENE_UNICYCLE_MOTION_ACCELERATE)
    {
        BM_RE_drawSprite(BM_Animation_getCurrentFrame(&_fishAnimation),
                        BM_Animation_getCurrentFrameMask(&_fishAnimation),
                        _cat.currentXPos + _cat.width + 10,
                        _cat.currentYPos + 10,
                        _fishAnimation.width,
                        _fishAnimation.height);
    }
}


void BM_SceneDelegate_Unicycle_Cat_reset()
{
    _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;
    _catFrameTimer = 0.f;
    _catFrameChangeVelocity = 0.f;

    _cat.currentXPos = BM_SCENE_UNICYCLE_CAT_NORMAL_XPOS;
}


void BM_SceneDelegate_Unicycle_Cat_setMotionState(const BM_Scene_Unicycle_Motion motion)
{
    if (motion < 0 || motion >= BM_SCENE_UNICYCLE_MOTION_NUM_MOTIONS)
        return;

    _currentMotion = motion;
}
