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


#include "BM_SceneDelegate_Unicycle_Buildings.h"

#define BM_SCENE_UNICYCLE_BUILDINGS_VELOCITY_NORMAL 2.f
#define BM_SCENE_UNICYCLE_BUILDINGS_VELOCITY_ACCEL  2.5f
#define BM_SCENE_UNICYCLE_BUILDINGS_YPOS            35

static BM_Sprite   _buildings;

static float32_t   _targetBuildingVelocity = 0.f;
static float32_t   _buildingVelocity = 0.f;
static float32_t   _buildingAcceleration = 0.f;

static BM_Scene_Unicycle_Motion     _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;



BM_Error BM_SceneDelegate_Unicycle_Buildings_init()
{
    _buildings.currentXPos = 0;
    _buildings.currentYPos = BM_SCENE_UNICYCLE_BUILDINGS_YPOS;
    _buildings.currentBitmap = BM_ASSETS_UNICYCLE_BUILDINGS;
    _buildings.currentMask = BM_ASSETS_UNICYCLE_BUILDINGS_MASK;
    _buildings.width = BM_ASSETS_UNICYCLE_BUILDINGS_WIDTH;
    _buildings.height = BM_ASSETS_UNICYCLE_BUILDINGS_HEIGHT;
    _buildings.dx = 0.f;
    _buildings.dy = 0.f;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Unicycle_Buildings_update()
{
    if (_buildingVelocity != _targetBuildingVelocity)
    {
        if (_buildingAcceleration < 0)
        {
            if (_buildingVelocity <= _targetBuildingVelocity)
            {
                _buildingVelocity = _targetBuildingVelocity;
                _buildingAcceleration = 0.f;
            }
            else
                _buildingVelocity += _buildingAcceleration;
        }
        else
        {
            if (_buildingVelocity >= _targetBuildingVelocity)
            {
                _buildingVelocity = _targetBuildingVelocity;
                _buildingAcceleration = 0.f;
            }
            else
                _buildingVelocity += _buildingAcceleration;
        }
    }

    BM_Sprite_setVelocity(&_buildings, _buildingVelocity, 0.f);
    BM_Sprite_update(&_buildings);

    //  Check to see if the buildings sprite has moved completely off screen and reset its position
    if (_buildings.currentXPos <= -BM_DISPLAY_WIDTH)
        _buildings.currentXPos = 0.f;

    if (_buildings.currentXPos >= BM_DISPLAY_WIDTH)
        _buildings.currentXPos = 0.f;
}


void BM_SceneDelegate_Unicycle_Buildings_draw()
{
    BM_RE_drawSprite(_buildings.currentBitmap,
                    _buildings.currentMask,
                    _buildings.currentXPos,
                    _buildings.currentYPos,
                    _buildings.width,
                    _buildings.height);

    if (_buildings.currentXPos != 0.f)
    {
        if (_buildings.currentXPos < 0.f)
        {
            BM_RE_drawSprite(_buildings.currentBitmap,
                            _buildings.currentMask,
                            BM_DISPLAY_WIDTH + _buildings.currentXPos,
                            BM_SCENE_UNICYCLE_BUILDINGS_YPOS,
                            _buildings.width,
                            _buildings.height);
        }
        if (_buildings.currentXPos > 0.f)
        {
            float32_t remainingWidth = _buildings.currentXPos - BM_DISPLAY_WIDTH;

            BM_RE_drawSprite(_buildings.currentBitmap,
                            _buildings.currentMask,
                            remainingWidth,
                            BM_SCENE_UNICYCLE_BUILDINGS_YPOS,
                            _buildings.width,
                            _buildings.height);
        }
    }
}


void BM_SceneDelegate_Unicycle_Buildings_reset()
{
    _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;
    _targetBuildingVelocity = 0.f;
    _buildingAcceleration = 0.f;
    _buildingVelocity = 0.f;
}


void BM_SceneDelegate_Unicycle_Buildings_setMotionTargets(const float32_t numSteps)
{
    if (numSteps < 0)
        return;

    switch (_currentMotion)
    {
        case BM_SCENE_UNICYCLE_MOTION_FORWARD:
            _targetBuildingVelocity = -BM_SCENE_UNICYCLE_BUILDINGS_VELOCITY_NORMAL;
            break;

        case BM_SCENE_UNICYCLE_MOTION_REVERSE:
            _targetBuildingVelocity = BM_SCENE_UNICYCLE_BUILDINGS_VELOCITY_NORMAL;
            break;

        case BM_SCENE_UNICYCLE_MOTION_ACCELERATE:
            _targetBuildingVelocity = -BM_SCENE_UNICYCLE_BUILDINGS_VELOCITY_ACCEL;
            break;

        case BM_SCENE_UNICYCLE_MOTION_STOPPING:
            _targetBuildingVelocity = 0.f;
            break;

        case BM_SCENE_UNICYCLE_MOTION_BRAKE:
            _targetBuildingVelocity = 0.f;
            break;

        default:
            break;
    }

    _buildingAcceleration = (_targetBuildingVelocity - _buildingVelocity) / numSteps;
}


void BM_SceneDelegate_Unicycle_Buildings_setMotionState(const BM_Scene_Unicycle_Motion motion)
{
    if (motion < 0 || motion >= BM_SCENE_UNICYCLE_MOTION_NUM_MOTIONS)
        return;

    _currentMotion = motion;
}