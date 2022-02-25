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

#ifndef SRC_SCENES_GLITCH_BM_SCENE_GLITCH_H_
#define SRC_SCENES_GLITCH_BM_SCENE_GLITCH_H_

#include "arm_math.h"

#include "BM_Common.h"
#include "BM_Scene.h"
#include "BM_RenderEngine.h"
#include "BM_ErrorCodes.h"
#include "BM_UserIOServices.h"
#include "BM_Sprite.h"
#include "BM_Scene_Glitch_Common.h"
#include "BM_Assets_Glitch.h"
#include "BM_SceneDelegate_Glitch_Window.h"

#include "MW_AFXUnit_Granular.h"


BM_Error    BM_Scene_Glitch_init(BM_Scene *scene);
void        BM_Scene_Glitch_reset();
void        BM_Scene_Glitch_update(void *data);
void        BM_Scene_Glitch_draw();
void        BM_Scene_Glitch_handleUserIO(const uint32_t buttonStates);
void        BM_Scene_Glitch_processAudio(float32_t *inputBuffer, const size_t bufferSize);


#endif /* SRC_SCENES_GLITCH_BM_SCENE_GLITCH_H_ */
