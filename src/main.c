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

#include "BM_UserIOServices.h"
#include "BM_DebugServices.h"
#include "BM_Controller_Active.h"
#include "BM_Controller_Sleep.h"
#include "BM_SceneRegister.h"
#include "BM_RTCModule.h"
#include "BM_ClockTree.h"
#include "BM_PowerManagement.h"
#include "BM_PeripheralCommon.h"

#include "em_device.h"
#include "em_chip.h"
#include "em_emu.h"

#ifdef __RUN_UNIT_TESTS
#include "BM_CollisionModuleTests.h"
#include "BM_RTCTests.h"
#endif

#define ever (;;)

float32_t largeBuffer[32000];

int main(void)
{
  /* Chip errata */
  CHIP_Init();

  BM_Error error = BM_ClockTree_EnableClocks();
  if (error != BM_NO_ERR)
    return 0;

  BM_PeripheralCommon_initLDMA();


#ifdef __RUN_UNIT_TESTS
//  if (!BM_CM_runUnitTests())
//    return 0;

  if (BM_RTCTests_runUnitTests() != BM_NO_ERR)
    return 0;

#endif

  BM_DebugServices_init();
  BM_ASSERT(BM_UserIO_init());
  BM_ASSERT(BM_Power_init());

  BM_SystemState systemState = BM_STATE_ACTIVE;

  error = BM_RTCModule_init();
  if (error != BM_NO_ERR)
    return 0;

  BM_Common_init();
  BM_SceneRegister_registerScenes();


  /*
   * Run the system state machine
   * Each state will have its own infinite loop
   */
  for ever {
    switch (systemState)
    {
      case BM_STATE_ACTIVE:
      {
        BM_Controller_Active_init();
        BM_Controller_Active_enter(&systemState);
        break;
      }

      case BM_STATE_SLEEP:
      {
        BM_Controller_Sleep_enter(&systemState);
        break;
      }

      default:
        return 0;
    }
  }

  return 0;
}
