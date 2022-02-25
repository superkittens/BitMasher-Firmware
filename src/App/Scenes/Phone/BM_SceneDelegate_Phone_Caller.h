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

#ifndef SRC_SCENES_PHONE_BM_SCENEDELEGATE_PHONE_CALLER_H_
#define SRC_SCENES_PHONE_BM_SCENEDELEGATE_PHONE_CALLER_H_

#include "arm_math.h"
#include "BM_ErrorCodes.h"
#include "BM_Common.h"
#include "BM_RenderEngine.h"
#include "BM_Scene_Phone_Common.h"
#include "BM_UserIOServices.h"
#include "BM_Sprite.h"

#include "BM_Assets_Cats.h"
#include "BM_Assets_Telephone.h"

#include "MW_AFXUnit_SVFilter.h"

BM_Error    BM_SceneDelegate_Phone_Caller_init();
void        BM_SceneDelegate_Phone_Caller_update();
void        BM_SceneDelegate_Phone_Caller_draw();
void        BM_SceneDelegate_Phone_Caller_processAudio(float32_t *inputBuffer, const size_t bufferSize);
void        BM_SceneDelegate_Phone_Caller_handleUserIO(const uint32_t buttonStates);

void        BM_SceneDelegate_Phone_Caller_setActive();
void        BM_SceneDelegate_Phone_Caller_setInactive();
void        BM_SceneDelegate_Phone_Caller_reset();

BM_Scene_Phone_AudioState   BM_SceneDelegate_Phone_Caller_getAudioState();


#endif /* SRC_SCENES_PHONE_BM_SCENEDELEGATE_PHONE_CALLER_H_ */
