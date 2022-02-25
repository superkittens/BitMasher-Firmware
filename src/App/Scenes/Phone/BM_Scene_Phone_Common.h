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

#ifndef SRC_SCENES_PHONE_BM_SCENE_PHONE_COMMON_H_
#define SRC_SCENES_PHONE_BM_SCENE_PHONE_COMMON_H_

typedef enum
{
    BM_SCENE_PHONE_EFFECT_STATE_INACTIVE = 0,
    BM_SCENE_PHONE_EFFECT_STATE_ACTIVE
}BM_Scene_Phone_EffectState;

typedef enum
{
    BM_SCENE_PHONE_AUDIO_STATE_NORMAL,
    BM_SCENE_PHONE_AUDIO_STATE_FX,
    BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL,
    BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX,
    BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX,
    BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_NORMAL
}BM_Scene_Phone_AudioState;

typedef enum
{
    BM_SCENE_PHONE_EFFECT_FLUTTER = 0,
    BM_SCENE_PHONE_EFFECT_BITCRUSH
}BM_Scene_Phone_Effect;



#endif /* SRC_SCENES_PHONE_BM_SCENE_PHONE_COMMON_H_ */
