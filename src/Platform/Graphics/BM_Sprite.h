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

#ifndef SRC_SPRITES_BM_SPRITE_H_
#define SRC_SPRITES_BM_SPRITE_H_

#include "arm_math.h"
#include "BM_ErrorCodes.h"

/*
 *  BM_Sprite structure defines various properties of a sprite
 *  When creating a sprite, populate an instance of the struct then to move the sprite,
 *  call BM_Sprite_update().
 *  Call setPosition and setVelocity to change movement properties
 * 
 *  The sprite's enclosing bounds and the bounds used to determine collisions between sprites are defined separately
 *  To create the collider bounds, define delta x and y values from the sprite's origin as well as its width and height
 */

typedef struct
{
  float32_t   currentXPos;
  float32_t   currentYPos;
  float32_t   prevXPos;
  float32_t   prevYPos;
  float32_t   dx;
  float32_t   dy;
  float32_t   ax;
  float32_t   ay;
  float32_t   g;                        //  Deprecated.  Use ax, ay instead
  int32_t    width;
  int32_t    height;
  uint8_t     *currentBitmap;
  uint8_t     *currentMask;
  uint32_t    isContacting;
  uint32_t    isMidAir;
  int32_t     colliderBoundsOriginDx;   //  Origin (x) delta from sprite's origin
  int32_t     colliderBoundsOriginDy;   //  Origin (y) delta from sprite's origin
  int32_t     colliderBoundsWidth;
  int32_t     colliderBoundsHeight;
}BM_Sprite;


void  BM_Sprite_setPosition(BM_Sprite *sprite, float32_t x, float32_t y);
void  BM_Sprite_setVelocity(BM_Sprite *sprite, float32_t vx, float32_t vy);
void  BM_Sprite_setAcceleration(BM_Sprite *sprite, float32_t ax, float32_t ay);
void  BM_Sprite_update(BM_Sprite *sprite);



/*
 *  BM_Animation encapsulates data for a single animation sequence
 *  To use this struct, populate the "public" member functions then call BM_Animation_init()
 *  You must have frames (bitmaps) for a given sequence defined somewhere
 *  If your frames do not have a mask, set useMask to 0.  Otherwise, set to 1 and point the frameMasks member to the proper location
 */
typedef struct
{
  uint8_t     *frames;
  uint8_t     *frameMasks;
  int32_t     useMask;
  int32_t     numFrames;
  int32_t     width, height;

  int32_t     _currentFrame;
  int32_t     _bytesPerFrame;
  int32_t     _playBackDirection;
  int32_t     _isInitialized;
}BM_Animation;


BM_Error  BM_Animation_init(BM_Animation *sequence, int32_t playbackDirection);
void      BM_Animation_update(BM_Animation *sequence);
BM_Error  BM_Animation_setPlaybackDirection(BM_Animation *sequence, int32_t direction);
uint8_t*  BM_Animation_getCurrentFrame(BM_Animation *sequence);
uint8_t*  BM_Animation_getCurrentFrameMask(BM_Animation *sequence);
int32_t   BM_Animation_getCurrentFrameNumber(BM_Animation *sequence);
int32_t   BM_Animation_getDirection(BM_Animation *sequence);
void      BM_Animation_setSpriteBitmapsToFrame(BM_Animation *sequence, BM_Sprite *sprite);
void      BM_Animation_reset(BM_Animation *sequence);



#endif /* SRC_SPRITES_BM_SPRITE_H_ */
