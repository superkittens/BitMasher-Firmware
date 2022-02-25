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


#include "BM_SceneDelegate_Unicycle_Clouds.h"


#define BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_NORMAL    0.5f
#define BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_ACCEL     1.f
#define BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_INITIAL   -0.2f

#define BM_SCENE_UNICYCLE_CLOUDS_NUM_CLOUDS         2

static BM_Sprite    _clouds[BM_SCENE_UNICYCLE_CLOUDS_NUM_CLOUDS];

//  Cloud movement parameters
static float32_t    _cloudVelocity = 0;
static float32_t    _cloudAcceleration = 0;
static float32_t    _targetCloudVelocity;

static int32_t      _activeCloud = 0;

static BM_Scene_Unicycle_Motion     _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;



static void BM_SceneDelegate_Unicycle_Clouds_resetCloudPosition()
{
    _clouds[0].currentXPos = 10;
    _clouds[0].currentYPos = 10;

    _clouds[1].currentXPos = BM_DISPLAY_WIDTH;
    _clouds[1].currentYPos = 15;
}




BM_Error BM_SceneDelegate_Unicycle_Clouds_init()
{
    _clouds[0].currentBitmap = BM_ASSETS_UNICYCLE_CLOUD1;
    _clouds[0].currentMask = BM_ASSETS_UNICYCLE_CLOUD1_MASK;
    _clouds[0].width = BM_ASSETS_UNICYCLE_CLOUD1_WIDTH;
    _clouds[0].height = BM_ASSETS_UNICYCLE_CLOUD1_HEIGHT;
    _clouds[0].currentXPos = 10;
    _clouds[0].currentYPos = 10;

    _clouds[1].currentBitmap = BM_ASSETS_UNICYCLE_CLOUD2;
    _clouds[1].currentMask = BM_ASSETS_UNICYCLE_CLOUD2_MASK;
    _clouds[1].width = BM_ASSETS_UNICYCLE_CLOUD2_WIDTH;
    _clouds[1].height = BM_ASSETS_UNICYCLE_CLOUD2_HEIGHT;
    _clouds[1].currentXPos = BM_DISPLAY_WIDTH;
    _clouds[1].currentYPos = 15;

    _targetCloudVelocity = BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_INITIAL;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Unicycle_Clouds_update()
{
    //  Check to see if cloud velocities have ramped up or down
    if (_cloudVelocity != _targetCloudVelocity)
    {
        if (_cloudAcceleration < 0)
        {
            if (_cloudVelocity <= _targetCloudVelocity)
            {
                _cloudVelocity = _targetCloudVelocity;
                _cloudAcceleration = 0.f;
            }
            else
                _cloudVelocity += _cloudAcceleration;
        }
        else
        {
            if (_cloudVelocity >= _targetCloudVelocity)
            {
                _cloudVelocity = _targetCloudVelocity;
                _cloudAcceleration = 0.f;
            }
            else
                _cloudVelocity += _cloudAcceleration;
        }
    }

    //  Check to see if a cloud has moved off screen
    if (_cloudVelocity < 0)
    {
        if (_clouds[_activeCloud].currentXPos <= -_clouds[_activeCloud].width)
        {
            _clouds[_activeCloud].currentXPos = BM_DISPLAY_WIDTH;
            _activeCloud = (_activeCloud + 1) % BM_SCENE_UNICYCLE_CLOUDS_NUM_CLOUDS;
        }
    }
    else
    {
       if (_clouds[_activeCloud].currentXPos >= BM_DISPLAY_WIDTH)
        {
            _clouds[_activeCloud].currentXPos = -_clouds[_activeCloud].width;
            _activeCloud = (_activeCloud + 1) % BM_SCENE_UNICYCLE_CLOUDS_NUM_CLOUDS;
        } 
    }

    BM_Sprite_setVelocity(&_clouds[_activeCloud], _cloudVelocity, 0);
    BM_Sprite_update(&_clouds[_activeCloud]);
}


void BM_SceneDelegate_Unicycle_Clouds_draw()
{
    for (int32_t i = 0; i < BM_SCENE_UNICYCLE_CLOUDS_NUM_CLOUDS; ++i)
        BM_RE_drawSprite(_clouds[i].currentBitmap,
                        _clouds[i].currentMask,
                        _clouds[i].currentXPos,
                        _clouds[i].currentYPos,
                        _clouds[i].width,
                        _clouds[i].height);
}


void BM_SceneDelegate_Unicycle_Clouds_reset()
{
    BM_SceneDelegate_Unicycle_Clouds_resetCloudPosition();
    _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;
    _cloudVelocity = 0.f;
    _cloudAcceleration = 0.f;
    _targetCloudVelocity = 0.f;
    _activeCloud = 0;
}


void BM_SceneDelegate_Unicycle_Clouds_setMotionTargets(const float32_t numSteps)
{
    if (numSteps <= 0)
        return;

    switch (_currentMotion)
    {
        case BM_SCENE_UNICYCLE_MOTION_FORWARD:
        {
            _targetCloudVelocity = -BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_NORMAL;
            _cloudAcceleration = (_targetCloudVelocity - _cloudVelocity) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_REVERSE:
        {
            _targetCloudVelocity = BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_NORMAL;
            _cloudAcceleration = (_targetCloudVelocity - _cloudVelocity) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_ACCELERATE:
        {
            _targetCloudVelocity = -BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_ACCEL;
            _cloudAcceleration = (_targetCloudVelocity - _cloudVelocity) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_STOPPING:
        case BM_SCENE_UNICYCLE_MOTION_BRAKE:
        {
            _targetCloudVelocity = BM_SCENE_UNICYCLE_CLOUDS_VELOCITY_INITIAL;
            _cloudAcceleration = (_targetCloudVelocity - _cloudVelocity) / numSteps;

            break;
        }
    }
}


void BM_SceneDelegate_Unicycle_Clouds_setMotionState(const BM_Scene_Unicycle_Motion motion)
{
    if (motion < 0 || motion >= BM_SCENE_UNICYCLE_MOTION_NUM_MOTIONS)
        return;

    _currentMotion = motion;
}