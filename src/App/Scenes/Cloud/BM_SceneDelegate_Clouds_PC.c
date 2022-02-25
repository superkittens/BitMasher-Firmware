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


#include "BM_SceneDelegate_Clouds_PC.h"

static BM_Scene_Cloud_Sprite_PC _pc;

static float32_t _GRAVITY = 1.0;
static float32_t _BM_SPRITEDELEGATE_CLOUDS_PC_VELOCITY = 2;
static float32_t _BM_SPRITEDELEGATE_CLOUDS_PC_JUMP_VELOCITY = 8;
static int32_t   _BM_SPRITEDELEGATE_CLOUDS_PC_MAX_JUMPS = 2;

static int32_t   _WALK_COUNTER_TIMEOUT = 4;

static int32_t  _isWalking = 0;


static void BM_SceneDelegate_Cloud_PC_checkBounds()
{   
    //  Check to see if the PC is going off of the left display edge
    if (_pc.spriteAttributes.currentXPos < 0)
        BM_Sprite_setPosition(&_pc.spriteAttributes, 0, _pc.spriteAttributes.currentYPos);
    
    //  Check to see if the PC is going off of the right display edge
    if (_pc.spriteAttributes.currentXPos + _pc.spriteAttributes.width >= BM_DISPLAY_WIDTH)
        BM_Sprite_setPosition(&_pc.spriteAttributes, BM_DISPLAY_WIDTH - _pc.spriteAttributes.width, _pc.spriteAttributes.currentYPos);
}


static void BM_SceneDelegate_Clouds_PC_updateMovement()
{
    switch(_pc.currentDirection)
    {
        case BM_DIR_LEFT:
        {
            //  If the cat is not in the middle of a jump, change walk animations
            if (_isWalking)
            {
                _pc.walkCounter++;
                if (_pc.walkCounter >= _WALK_COUNTER_TIMEOUT)
                {
                    BM_Animation_update(&_pc.leftWalkAnimation);
                    BM_Animation_setSpriteBitmapsToFrame(&_pc.leftWalkAnimation, &_pc.spriteAttributes);
                    _pc.walkCounter = 0;
                }

                _pc.spriteAttributes.colliderBoundsWidth = _pc.leftWalkAnimation.width;
                _pc.spriteAttributes.colliderBoundsHeight = _pc.leftWalkAnimation.height;
            }

            //  Otherwise, set the bitmap to the jump bitmap
            if (_pc.spriteAttributes.isMidAir)
            {
                _pc.spriteAttributes.currentBitmap = BM_ASSETS_CAT_JUMP_LEFT;
                _pc.spriteAttributes.currentMask = BM_ASSETS_CAT_JUMP_LEFT_MASK;
                _pc.spriteAttributes.width = BM_ASSETS_CAT_JUMP_LEFT_WIDTH;
                _pc.spriteAttributes.height = BM_ASSETS_CAT_JUMP_LEFT_HEIGHT;
                _pc.spriteAttributes.colliderBoundsWidth = BM_ASSETS_CAT_JUMP_LEFT_WIDTH;
                _pc.spriteAttributes.colliderBoundsHeight = BM_ASSETS_CAT_JUMP_LEFT_HEIGHT;
            }
            break;
        }

        case BM_DIR_RIGHT:
        {
            if (_isWalking)
            {
                _pc.walkCounter++;
                if (_pc.walkCounter >= _WALK_COUNTER_TIMEOUT)
                {
                    BM_Animation_update(&_pc.rightWalkAnimation);
                    BM_Animation_setSpriteBitmapsToFrame(&_pc.rightWalkAnimation, &_pc.spriteAttributes);
                    _pc.walkCounter = 0;
                }

                _pc.spriteAttributes.colliderBoundsWidth = _pc.rightWalkAnimation.width;
                _pc.spriteAttributes.colliderBoundsHeight = _pc.rightWalkAnimation.height;
            }

            if (_pc.spriteAttributes.isMidAir)
            {
                _pc.spriteAttributes.currentBitmap = BM_ASSETS_CAT_JUMP_RIGHT;
                _pc.spriteAttributes.currentMask = BM_ASSETS_CAT_JUMP_RIGHT_MASK;
                _pc.spriteAttributes.width = BM_ASSETS_CAT_JUMP_RIGHT_WIDTH;
                _pc.spriteAttributes.height = BM_ASSETS_CAT_JUMP_RIGHT_HEIGHT;
                _pc.spriteAttributes.colliderBoundsWidth = BM_ASSETS_CAT_JUMP_RIGHT_WIDTH;
                _pc.spriteAttributes.colliderBoundsHeight = BM_ASSETS_CAT_JUMP_RIGHT_HEIGHT;
            }

            break;
        }

        default:
            break;
    }
}


static void BM_SceneDelegate_Clouds_PC_jump()
{
    if (_pc.jumpCounter < _BM_SPRITEDELEGATE_CLOUDS_PC_MAX_JUMPS)
    {
        _pc.jumpCounter++;

        _pc.spriteAttributes.isMidAir = 1;
        BM_Sprite_setVelocity(&_pc.spriteAttributes, 0, -_BM_SPRITEDELEGATE_CLOUDS_PC_JUMP_VELOCITY);
        BM_Sprite_setAcceleration(&_pc.spriteAttributes, 0, _GRAVITY);
        BM_Sprite_setPosition(&_pc.spriteAttributes, _pc.spriteAttributes.currentXPos, _pc.spriteAttributes.currentYPos + _pc.spriteAttributes.dy);

        _isWalking = 0;
    }
}



//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//




BM_Error BM_SceneDelegate_Cloud_PC_init()
{
    //  Initialize the playable character
    _pc.spriteAttributes.currentBitmap = BM_ASSETS_CAT_FRONT;
    _pc.spriteAttributes.currentMask = BM_ASSETS_CAT_FRONT_MASK;
    _pc.spriteAttributes.width = BM_ASSETS_CAT_FRONT_WIDTH;
    _pc.spriteAttributes.height = BM_ASSETS_CAT_FRONT_HEIGHT;
    _pc.spriteAttributes.currentXPos = 20;
    _pc.spriteAttributes.currentYPos = 0;
    _pc.spriteAttributes.colliderBoundsOriginDx = 0;
    _pc.spriteAttributes.colliderBoundsOriginDy = 0;
    _pc.spriteAttributes.isMidAir = 1;
    _pc.spriteAttributes.colliderBoundsWidth = BM_ASSETS_CAT_FRONT_WIDTH;
    _pc.spriteAttributes.colliderBoundsHeight = BM_ASSETS_CAT_FRONT_HEIGHT;
    _pc.jumpCounter = 0;

    BM_Sprite_setAcceleration(&_pc.spriteAttributes, 0, _GRAVITY);

    //  Setup walking animations
    _pc.leftWalkAnimation.frames = BM_ASSETS_CAT_WALK_LEFT;
    _pc.leftWalkAnimation.frameMasks = BM_ASSETS_CAT_WALK_LEFT_MASK;
    _pc.leftWalkAnimation.width = BM_ASSETS_CAT_WALK_LEFT_WIDTH;
    _pc.leftWalkAnimation.height = BM_ASSETS_CAT_WALK_LEFT_HEIGHT;
    _pc.leftWalkAnimation.useMask = 1;
    _pc.leftWalkAnimation.numFrames = BM_ASSETS_CAT_WALK_LEFT_NUM_FRAMES;
    BM_Animation_init(&_pc.leftWalkAnimation, 0);

    _pc.rightWalkAnimation.frames = BM_ASSETS_CAT_WALK_RIGHT;
    _pc.rightWalkAnimation.frameMasks = BM_ASSETS_CAT_WALK_RIGHT_MASK;
    _pc.rightWalkAnimation.width = BM_ASSETS_CAT_WALK_RIGHT_WIDTH;
    _pc.rightWalkAnimation.height = BM_ASSETS_CAT_WALK_RIGHT_HEIGHT;
    _pc.rightWalkAnimation.useMask = 1;
    _pc.rightWalkAnimation.numFrames = BM_ASSETS_CAT_WALK_RIGHT_NUM_FRAMES;
    BM_Animation_init(&_pc.rightWalkAnimation, 0);
    
    return BM_NO_ERR;
}


void BM_SceneDelegate_Clouds_PC_update()
{
    BM_SceneDelegate_Clouds_PC_updateMovement();
    BM_Sprite_update(&_pc.spriteAttributes);
    BM_SceneDelegate_Cloud_PC_checkBounds();
}


void BM_SceneDelegate_Clouds_PC_draw()
{
    BM_RE_drawSprite(   _pc.spriteAttributes.currentBitmap, 
                        _pc.spriteAttributes.currentMask, 
                        _pc.spriteAttributes.currentXPos,
                        _pc.spriteAttributes.currentYPos,
                        _pc.spriteAttributes.width,
                        _pc.spriteAttributes.height);
}


void BM_SceneDelegate_Clouds_PC_fall()
{
    BM_Sprite_setAcceleration(&_pc.spriteAttributes, _pc.spriteAttributes.ax, _GRAVITY);
}


//  When the PC falls off screen, reset the PC's velocity, acceleration, bitmaps and other flags
void BM_SceneDelegate_Clouds_PC_onOffScreen()
{
    BM_Sprite_setVelocity(&_pc.spriteAttributes, 0, 0);
    BM_Sprite_setAcceleration(&_pc.spriteAttributes, 0, 0);
    BM_Animation_setSpriteBitmapsToFrame(&_pc.rightWalkAnimation, &_pc.spriteAttributes);
    _isWalking = 0;
    _pc.spriteAttributes.isMidAir = 0;
}


void BM_SceneDelegate_Clouds_PC_landed()
{
    _pc.jumpCounter = 0;
    _pc.spriteAttributes.isMidAir = 0;

    switch(_pc.currentDirection)
    {
        case BM_DIR_LEFT:
        {
            BM_Animation_setSpriteBitmapsToFrame(&_pc.leftWalkAnimation, &_pc.spriteAttributes);
            break;
        }

        case BM_DIR_RIGHT:
        {
            BM_Animation_setSpriteBitmapsToFrame(&_pc.rightWalkAnimation, &_pc.spriteAttributes);
            break;
        }

        default:
            break;
    }
}


void BM_SceneDelegate_Clouds_PC_handleUserIO(const uint32_t buttonStates)
{
    uint32_t buttonEdgeState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
        BM_SceneDelegate_Clouds_PC_jump();

    uint32_t leftButtonState = 0;
    uint32_t rightButtonState = 0;

    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_LEFT, &leftButtonState));
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_RIGHT, &rightButtonState));

    do
    {
        if (!leftButtonState)
        {
            BM_Sprite_setVelocity(&_pc.spriteAttributes, -_BM_SPRITEDELEGATE_CLOUDS_PC_VELOCITY, _pc.spriteAttributes.dy);
            _pc.currentDirection = BM_DIR_LEFT;

            if (!_pc.spriteAttributes.isMidAir)
                _isWalking = 1;

            break;
        }

        if (!rightButtonState)
        {
            BM_Sprite_setVelocity(&_pc.spriteAttributes, _BM_SPRITEDELEGATE_CLOUDS_PC_VELOCITY, _pc.spriteAttributes.dy);
            _pc.currentDirection = BM_DIR_RIGHT;

            if (!_pc.spriteAttributes.isMidAir)
                _isWalking = 1;

            break;
        }

        else
        {
            BM_Sprite_setVelocity(&_pc.spriteAttributes, 0, _pc.spriteAttributes.dy);
            _isWalking = 0;
        }
    } while (0);
}


BM_Scene_Cloud_Sprite_PC* BM_SceneDelegate_Clouds_PC_getPC()
{
    return &_pc;
}
