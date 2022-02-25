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

#ifndef SRC_BM_ERRORCODES_H_
#define SRC_BM_ERRORCODES_H_

typedef enum
{
  BM_NO_ERR                     = 0,
  BM_INVALID_PARAM,

  BM_AE_SETUP_FAIL              = 100,
  BM_AE_INVALID_FS,
  BM_AE_NOT_INITIALIZED,
  BM_AE_CANNOT_INIT_CODEC,
  BM_AE_CODEC_FAIL,

  BM_RE_SETUP_FAIL              = 200,
  BM_RE_NOT_INITIALIZED,

  BM_SERVICE_QUEUE_ERROR        = 300,
  BM_SERVICE_QUEUE_QUEUE_EMPTY,

  BM_AFXUNIT_INIT_FAIL          = 400,

  BM_DSP_INIT_FAIL              = 500,

  BM_ANIMATION_INVALID_MEMBERS  = 600,
  BM_ANIMATION_NOT_INITIALIZED,
  BM_ANIMATION_INVALID_DIRECTION,

  BM_CONTROLLER_FAIL            = 700,
  BM_CONTROLLER_MAX_SCENES_REACHED,

  BM_USERIO_NOT_INITIALIZED     = 800,
  BM_USERIO_POST_FAIL,
  BM_USERIO_INVALID_BUTTON,
  BM_USERIO_INVALID_EDGE,

  BM_RTC_NOT_INITIALIZED        = 900,
  BM_RTC_INVALID_LFXO_FREQ,
  BM_INVALID_TIME,

  BM_CLOCKTREE_ERROR            = 1000,

  BM_POWER_MGMT_ERROR           = 1100,

  BM_TRNG_NOT_ENABLED           = 1200,

  BM_DEBUG_TIMER_OVF            = 8900,

  BM_UNIT_TEST_FAIL             = 9000,

  BM_NULLPTR                  = 10000
}BM_Error;

#endif /* SRC_BM_ERRORCODES_H_ */
