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


#include "BM_SceneDelegate_Unicycle_Road.h"

#define BM_SCENE_UNICYCLE_NUM_ROAD_LINES            4
#define BM_SCENE_UNICYCLE_ROAD_VELOCITY_NORMAL      4.f
#define BM_SCENE_UNICYCLE_ROAD_VELOCITY_ACCEL       8.f
#define BM_SCENE_UNICYCLE_SIGN_MIN_VELOCITY         -4.f


static BM_Sprite    _roadSurface;
static BM_Sprite    _roadLines[BM_SCENE_UNICYCLE_NUM_ROAD_LINES];
static int32_t      _lastRoadLine = 3;

static BM_Sprite    _sign;

static float32_t    _targetRoadVelocity = 0.f;
static float32_t    _roadVelocity = 0.f;
static float32_t    _roadAcceleration = 0.f;

static BM_Scene_Unicycle_Motion     _currentMotion;




static void BM_SceneDelegate_Unicycle_Road_initializeRoad()
{
    _roadSurface.currentBitmap = BM_ASSETS_UNICYCLE_ROAD;
    _roadSurface.currentMask = NULL;
    _roadSurface.currentXPos = 0;
    _roadSurface.currentYPos = 87;
    _roadSurface.width = BM_ASSETS_UNICYCLE_ROAD_WIDTH;
    _roadSurface.height = BM_ASSETS_UNICYCLE_ROAD_HEIGHT;

    float32_t numLinesPerScreen = ((float32_t)BM_DISPLAY_WIDTH / (float32_t)BM_ASSETS_UNICYCLE_ROADLINES_WIDTH) - 1;
    int32_t numLinesPerScreenInt = numLinesPerScreen;
    int32_t numPixelsBetweenLines = (BM_DISPLAY_WIDTH - (BM_ASSETS_UNICYCLE_ROADLINES_WIDTH * numLinesPerScreenInt)) / (numLinesPerScreenInt - 1);


    for (int32_t i = 0; i < BM_SCENE_UNICYCLE_NUM_ROAD_LINES; ++i)
    {
        _roadLines[i].currentBitmap = BM_ASSETS_UNICYCLE_ROADLINES;
        _roadLines[i].currentMask = NULL;
        _roadLines[i].currentXPos = i * (BM_ASSETS_UNICYCLE_ROADLINES_WIDTH + numPixelsBetweenLines);
        _roadLines[i].currentYPos = _roadSurface.currentYPos + (((BM_DISPLAY_HEIGHT - _roadSurface.currentYPos) / 2) - BM_ASSETS_UNICYCLE_ROADLINES_HEIGHT);
        _roadLines[i].width = BM_ASSETS_UNICYCLE_ROADLINES_WIDTH;
        _roadLines[i].height = BM_ASSETS_UNICYCLE_ROADLINES_HEIGHT;
    }
}



//  ================================================================================================    //
//  ================================================================================================    //



BM_Error BM_SceneDelegate_Unicycle_Road_init()
{
    BM_SceneDelegate_Unicycle_Road_initializeRoad();

    _sign.currentBitmap = BM_ASSETS_UNICYCLE_SIGN;
    _sign.currentMask = BM_ASSETS_UNICYCLE_SIGN_MASK;
    _sign.width = BM_ASSETS_UNICYCLE_SIGN_WIDTH;
    _sign.height = BM_ASSETS_UNICYCLE_SIGN_HEIGHT;
    _sign.currentYPos = 70;
    _sign.currentXPos = BM_DISPLAY_WIDTH;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Unicycle_Road_update()
{
    //  Check to see if we are ramping up/down the road velocity
    if (_roadVelocity != _targetRoadVelocity)
    {
        if (_roadAcceleration < 0)
        {
            if (_roadVelocity <= _targetRoadVelocity)
            {
                _roadVelocity = _targetRoadVelocity;
                _roadAcceleration = 0.f;
            }
            else
                _roadVelocity += _roadAcceleration;
        }
        else
        {
            if (_roadVelocity >= _targetRoadVelocity)
            {
                _roadVelocity = _targetRoadVelocity;
                _roadAcceleration = 0.f;
            }
            else
                _roadVelocity += _roadAcceleration;
        }
    }

    //  If we are braking, then the stop sign should appear once we drop to a certain velocity
    if (_currentMotion == BM_SCENE_UNICYCLE_MOTION_BRAKE)
    {
        if (_roadVelocity >= BM_SCENE_UNICYCLE_SIGN_MIN_VELOCITY)
        {
            BM_Sprite_setVelocity(&_sign, _roadVelocity, 0);
            BM_Sprite_update(&_sign);
        }
    }

    BM_Sprite_setAcceleration(&_roadSurface, _roadAcceleration, 0.f);
    BM_Sprite_setVelocity(&_roadSurface, _roadVelocity, 0.f);
    BM_Sprite_update(&_roadSurface);

    //  Reset road surface position if it moves completely off screen
    if (_roadSurface.currentXPos <= -BM_DISPLAY_WIDTH)
        _roadSurface.currentXPos = 0.f;

    if (_roadSurface.currentXPos >= BM_DISPLAY_WIDTH)
        _roadSurface.currentXPos = 0.f;



    //  Update road lines and reposition them if they move off screen
    int32_t relocateRoadLine = -1;
    for (int32_t i = 0; i < BM_SCENE_UNICYCLE_NUM_ROAD_LINES; ++i)
    {
        BM_Sprite_setAcceleration(&_roadLines[i], _roadAcceleration, 0.f);
        BM_Sprite_setVelocity(&_roadLines[i], _roadVelocity, 0.f);

        BM_Sprite_update(&_roadLines[i]);

        if ((_roadVelocity) < 0 && (_roadLines[i].currentXPos < -_roadLines[i].width))
                relocateRoadLine = i;
        
        if ((_roadVelocity > 0) && (_roadLines[i].currentXPos >= BM_DISPLAY_WIDTH))
                relocateRoadLine = i;
    }

    //  If a road line needs relocating, do a linear search to find the visible road line 
    //  That we need to position the road line to move relative to
    if (relocateRoadLine >= 0)
    {
        if (_roadVelocity < 0)
        {
            float32_t lastLineXPos = 0;
            for (int32_t i = 0; i < BM_SCENE_UNICYCLE_NUM_ROAD_LINES; ++i)
            {
                if (i == relocateRoadLine) continue;
                if (_roadLines[i].currentXPos > lastLineXPos) lastLineXPos = _roadLines[i].currentXPos;
            }

            _roadLines[relocateRoadLine].currentXPos = lastLineXPos + BM_ASSETS_UNICYCLE_ROADLINES_WIDTH + 16;
        }
        if (_roadVelocity > 0)
        {
            float32_t lastLineXPos = BM_DISPLAY_WIDTH;
            for (int32_t i = 0; i < BM_SCENE_UNICYCLE_NUM_ROAD_LINES; ++i)
            {
                if (i == relocateRoadLine) continue;
                if (_roadLines[i].currentXPos < lastLineXPos) lastLineXPos = _roadLines[i].currentXPos;
            }

            _roadLines[relocateRoadLine].currentXPos = lastLineXPos - BM_ASSETS_UNICYCLE_ROADLINES_WIDTH - 16;
        }
    }
}


void BM_SceneDelegate_Unicycle_Road_draw()
{
    BM_RE_drawSprite(_roadSurface.currentBitmap,
                    _roadSurface.currentMask,
                    _roadSurface.currentXPos,
                    _roadSurface.currentYPos,
                    _roadSurface.width,
                    _roadSurface.height);
    
    if (_roadSurface.currentXPos != 0)
    {
        if (_roadSurface.currentXPos < 0)
        {
            BM_RE_drawSprite(_roadSurface.currentBitmap,
                            _roadSurface.currentMask,
                            BM_DISPLAY_WIDTH + _roadSurface.currentXPos,
                            _roadSurface.currentYPos,
                            _roadSurface.width,
                            _roadSurface.height);
        }

        if (_roadSurface.currentXPos > 0)
        {
            float32_t width = _roadSurface.currentXPos - BM_DISPLAY_WIDTH;

            BM_RE_drawSprite(_roadSurface.currentBitmap,
                            _roadSurface.currentMask,
                            width,
                            _roadSurface.currentYPos,
                            _roadSurface.width,
                            _roadSurface.height);
        }

    }

    for (int32_t i = 0; i < BM_SCENE_UNICYCLE_NUM_ROAD_LINES; ++i)
    {
        BM_RE_drawSprite(_roadLines[i].currentBitmap,
                        _roadLines[i].currentMask,
                        _roadLines[i].currentXPos,
                        _roadLines[i].currentYPos,
                        _roadLines[i].width,
                        _roadLines[i].height);
    }

    if (_currentMotion == BM_SCENE_UNICYCLE_MOTION_BRAKE)
        BM_RE_drawSprite(_sign. currentBitmap,
                        _sign.currentMask,
                        _sign.currentXPos,
                        _sign.currentYPos,
                        _sign.width,
                        _sign.height);
}


void BM_SceneDelegate_Unicycle_Road_reset()
{
    _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;
    _targetRoadVelocity = 0.f;
    _roadVelocity = 0.f;
    _roadAcceleration = 0.f;
}


void BM_SceneDelegate_Unicycle_Road_setMotionTargets(const float32_t numSteps)
{
    if (numSteps <= 0)
        return;

    //  Reset sign position
    _sign.currentXPos = BM_DISPLAY_WIDTH;

    switch (_currentMotion)
    {
        case BM_SCENE_UNICYCLE_MOTION_FORWARD:
        {
            _targetRoadVelocity = -BM_SCENE_UNICYCLE_ROAD_VELOCITY_NORMAL;
            _roadAcceleration = (_targetRoadVelocity - _roadVelocity) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_REVERSE:
        {
            _targetRoadVelocity = BM_SCENE_UNICYCLE_ROAD_VELOCITY_NORMAL;
            _roadAcceleration = (_targetRoadVelocity - _roadVelocity) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_ACCELERATE:
        {
            _targetRoadVelocity = -BM_SCENE_UNICYCLE_ROAD_VELOCITY_ACCEL;
            _roadAcceleration = (_targetRoadVelocity - _roadVelocity) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_STOPPING:
        {
            _targetRoadVelocity = 0.f;
            _roadAcceleration = (_targetRoadVelocity - _roadVelocity) / numSteps;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_BRAKE:
        {
            _targetRoadVelocity = 0.f;
            _roadAcceleration = (_targetRoadVelocity - _roadVelocity) / numSteps;

            break;
        }
    }
}


void BM_SceneDelegate_Unicycle_Road_setMotionState(const BM_Scene_Unicycle_Motion motion)
{
    if (motion < 0 || motion >= BM_SCENE_UNICYCLE_MOTION_NUM_MOTIONS)
        return;

    _currentMotion = motion;
}
