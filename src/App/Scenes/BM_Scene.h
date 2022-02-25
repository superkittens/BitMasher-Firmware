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

#ifndef SRC_SCENES_BM_SCENE_H_
#define SRC_SCENES_BM_SCENE_H_

#include "arm_math.h"



/*
 * The BM_Scene struct lists the necessary functions that ALL scenes will need to implement
 * An instance of this structure is passed into an initialization function where its fields
 * are populated to the appropriate addresses of the functions
 */
typedef struct
{
  void (*handleUserIO)(const uint32_t buttonStates);
  void (*update)(void* data);
  void (*draw)(void);
  void (*processAudio)(float32_t *buffer, const size_t bufferSize);
  void (*reset)(void);

  uint8_t     *iconBitmap;
  const char  *sceneName;

}BM_Scene;

#endif /* SRC_SCENES_BM_SCENE_H_ */
