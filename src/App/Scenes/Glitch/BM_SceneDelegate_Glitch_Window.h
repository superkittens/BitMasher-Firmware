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

#ifndef SRC_SCENES_GLITCH_BM_SCENEDELEGATE_GLITCH_WINDOW_H_
#define SRC_SCENES_GLITCH_BM_SCENEDELEGATE_GLITCH_WINDOW_H_

#include "arm_math.h"
#include "BM_Scene_Glitch_Common.h"
#include "BM_Assets_Glitch.h"
#include "BM_ErrorCodes.h"
#include "BM_RenderEngine.h"

BM_Error    BM_SceneDelegate_Glitch_Window_init(BM_Scene_Glitch_Element *e);
void        BM_SceneDelegate_Glitch_Window_update();

void        BM_Scene_Glitch_onWindowDragged(float32_t dx, float32_t dy);
void        BM_Scene_Glitch_onWindowRelease();



#endif /* SRC_SCENES_GLITCH_BM_SCENEDELEGATE_GLITCH_WINDOW_H_ */
