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


#include "BM_Sprite.h"


void BM_Sprite_setPosition(BM_Sprite *sprite, float32_t x, float32_t y)
{
    if (sprite == NULL)
        return;

    sprite->currentXPos = x;
    sprite->currentYPos = y;
    sprite->prevXPos = x;
    sprite->prevYPos = y;
}


void BM_Sprite_setVelocity(BM_Sprite *sprite, float32_t vx, float32_t vy)
{
    if (sprite == NULL)
        return;

    sprite->dx = vx;
    sprite->dy = vy;
}


void BM_Sprite_setAcceleration(BM_Sprite *sprite, float32_t ax, float32_t ay)
{
    if (sprite == NULL)
        return;

    sprite->ax = ax;
    sprite->ay = ay;
}


void BM_Sprite_update(BM_Sprite *sprite)
{
    if (sprite == NULL)
        return;
        
    sprite->prevXPos = sprite->currentXPos;
    sprite->prevYPos = sprite->currentYPos;
    
    sprite->currentXPos += sprite->dx;
    sprite->currentYPos += sprite->dy;

    sprite->dx += sprite->ax;
    sprite->dy += sprite->ay;
}


//  =================================================================================  //
//  =================================================================================  //


/*
 *  Initialize an instance of BM_Animation
 *  Call this function after populating the public members of the struct
 * 
 *  Inputs:
 *      sequence:           Pointer to a BM_Animation instance
 *      playbackDirection:  Direction to play animation sequence (0 = forward, 1 = backward) 
 * 
 *  Returns:
 *      BM_NO_ERR if no errors
 */
BM_Error BM_Animation_init(BM_Animation *sequence, int32_t playbackDirection)
{
    if (sequence == NULL)
        return BM_NULLPTR;

    if (sequence->width <= 0 || sequence->height <= 0)
        return BM_ANIMATION_INVALID_MEMBERS;
    
    if (sequence->frameMasks == NULL && sequence->useMask == 1)
        return BM_ANIMATION_INVALID_MEMBERS;

    if (sequence->numFrames <= 0)
        return BM_ANIMATION_INVALID_MEMBERS;

    
    sequence->_currentFrame = 0;
    sequence->_isInitialized = 1;
    sequence->_bytesPerFrame = (((sequence->width - 1) / 8) + 1) * sequence->height;

    return BM_NO_ERR;
}


/*
 *  Move _currentFrame pointer forward or backward one frame
 * 
 *  Inputs:
 *      sequence:   Pointer to a BM_Animation instance
 * 
 *  Returns:
 *      None
 */
void BM_Animation_update(BM_Animation *sequence)
{
    if (sequence == NULL)
        return;

    if (!sequence->_isInitialized)
        return;

    if (!sequence->_playBackDirection)
        sequence->_currentFrame = (sequence->_currentFrame + 1) % sequence->numFrames;
    
    else
    {
        sequence->_currentFrame -= 1;
        if (sequence->_currentFrame < 0)
            sequence->_currentFrame = sequence->numFrames - 1;
    }
}


/*
 *  Set playback direction (forward or backward)
 *  Calling this functin will NOT reset _currentFrame back to 0.
 *  Subsequent calls to BM_Animation_update() will move _currentFrame from its position before setPlaybackDirection() was called
 * 
 *  Inputs:
 *      sequence:   Pointer to a BM_Animation instance
 *      direction:  Playback direction (0 = forward, 1 = backward)
 * 
 *  Returns:
 *      BM_NO_ERR if successful
 */
BM_Error BM_Animation_setPlaybackDirection(BM_Animation *sequence, int32_t direction)
{
    if (sequence == NULL)
        return BM_NULLPTR;

    if (!sequence->_isInitialized)
        return BM_ANIMATION_NOT_INITIALIZED;

    if (direction < 0 || direction > 1)
        return BM_ANIMATION_INVALID_DIRECTION;
    
    sequence->_playBackDirection = direction;

    return BM_NO_ERR;
}


/*
 *  Get the current frame pointed to by sequence->_currentFrame
 *  This function will NOT update _currentFrame.  You must call BM_Animation_update() to do this
 * 
 *  Inputs:
 *      sequence:   Pointer to a BM_Animation instance
 * 
 *  Returns:
 *      Pointer to the frame corresponding to _currentFrame
 *      NULL if sequence is NULL
 */
uint8_t* BM_Animation_getCurrentFrame(BM_Animation *sequence)
{
    if (sequence == NULL)
        return NULL;

    if (!sequence->_isInitialized)
        return NULL;

    return (sequence->frames + (sequence->_bytesPerFrame * sequence->_currentFrame));
}


/*
 *  Get the current frame number
 *
 *  Inputs:
 *      sequence:   Pointer to a BM_Animation instance
 * 
 *  Returns:
 *      Frame number (>= 0)
 *      -1 if the pointer is NULL
 */
int32_t BM_Animation_getCurrentFrameNumber(BM_Animation *sequence)
{
    if (sequence == NULL)
        return -1;

    return sequence->_currentFrame;
}


/*
 *  Get the animation direction
 *
 *  Inputs:
 *      sequence:   Pointer to a BM_Animation instance
 * 
 *  Returns:
 *      Frame number (>= 0)
 *      -1 if the pointer is NULL
 */
int32_t BM_Animation_getDirection(BM_Animation *sequence)
{
    if (sequence == NULL)
        return -1;

    return sequence->_playBackDirection;
}

/*
 *  Same as BM_Animation_getCurrentFrame() except for the frame masks
 * 
 *  Inputs:
 *      sequence:   Pointer to a BM_Animation instance
 * 
 *  Returns:
 *      Pointer to the frame mask corresponding to _currentFrame
 *      NULL if invalid parameters input or sequence is not initialized
 */
uint8_t* BM_Animation_getCurrentFrameMask(BM_Animation *sequence)
{
    if (sequence == NULL)
        return NULL;

    if (!sequence->_isInitialized)
        return NULL;

    if (!sequence->useMask)
        return NULL;

    return (sequence->frameMasks + (sequence->_bytesPerFrame * sequence->_currentFrame));
}


void BM_Animation_setSpriteBitmapsToFrame(BM_Animation *sequence, BM_Sprite *sprite)
{
    if (sequence == NULL || sprite == NULL)
        return;

    if (!sequence->_isInitialized)
        return;
    
    sprite->currentBitmap = BM_Animation_getCurrentFrame(sequence);
    sprite->width = sequence->width;
    sprite->height = sequence->height;

    if (sequence->useMask)
        sprite->currentMask = BM_Animation_getCurrentFrameMask(sequence);
}


/*
 *  Reset _currentFrame back to 0
 *  Reset _playbackDirection back to forward
 * 
 *  Inputs:
 *      sequence:   Pointer to a BM_Animation instance
 * 
 *  Returns:
 *      None
 */
void BM_Animation_reset(BM_Animation *sequence)
{
    if (sequence == NULL)
        return;

    if (sequence->_isInitialized)
        return;

    sequence->_currentFrame = 0;
    sequence->_playBackDirection = 0;
}
