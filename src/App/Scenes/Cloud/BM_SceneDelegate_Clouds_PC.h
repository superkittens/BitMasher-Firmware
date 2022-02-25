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

#ifndef SRC_SPRITES_BM_SCENEDELEGATE_CLOUDS_PC_H_
#define SRC_SPRITES_BM_SCENEDELEGATE_CLOUDS_PC_H_

#include "arm_math.h"
#include "BM_Sprite.h"
#include "BM_RenderEngine.h"
#include "BM_Common.h"
#include "BM_ErrorCodes.h"
#include "BM_UserIOServices.h"

#include "BM_Assets_Cats.h"



typedef struct
{
    BM_Sprite       spriteAttributes;

    BM_Animation    leftWalkAnimation, rightWalkAnimation;

    int32_t         jumpCounter;
    int32_t         walkCounter;

    BM_Direction    currentDirection;

}BM_Scene_Cloud_Sprite_PC;

BM_Error    BM_SceneDelegate_Cloud_PC_init();
void        BM_SceneDelegate_Clouds_PC_update();
void        BM_SceneDelegate_Clouds_PC_draw();
void        BM_SceneDelegate_Clouds_PC_fall();
void        BM_SceneDelegate_Clouds_PC_onOffScreen();
void        BM_SceneDelegate_Clouds_PC_landed();
void        BM_SceneDelegate_Clouds_PC_handleUserIO(const uint32_t buttonStates);

BM_Scene_Cloud_Sprite_PC*   BM_SceneDelegate_Clouds_PC_getPC();


#endif /* SRC_SPRITES_BM_SCENEDELEGATE_CLOUDS_PC_H_ */
