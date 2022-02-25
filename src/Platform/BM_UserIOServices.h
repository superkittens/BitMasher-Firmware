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

#ifndef SRC_BM_USERIO_SERVICES_H_
#define SRC_BM_USERIO_SERVICES_H_

#include "BM_Common.h"
#include "BM_ErrorCodes.h"
#include "BM_ISR.h"
#include "em_gpio.h"
#include "em_cmu.h"



typedef enum
{
    BM_USERIO_RISING = 0,
    BM_USERIO_FALLING,
    BM_USERIO_NUM_EDGES
}BM_UserIO_Edge;

typedef enum
{
    BM_USERIO_A = 0,
    BM_USERIO_B,
    BM_USERIO_UP,
    BM_USERIO_DOWN,
    BM_USERIO_LEFT,
    BM_USERIO_RIGHT,
    BM_USERIO_MENU,
    BM_USERIO_NOT_ASSIGNED,
    BM_USERIO_NUM_BUTTONS
}BM_UserIO_Button;


BM_Error    BM_UserIO_init();
BM_Error    BM_UserIO_update();
BM_Error    BM_UserIO_getAllButtonStates(uint32_t * const buttonStates);
BM_Error    BM_UserIO_getButtonState(const BM_UserIO_Button button, uint32_t * const buttonState);
BM_Error    BM_UserIO_getAllButtonEdgeStates(uint32_t * const edgeStates, const BM_UserIO_Edge edge);
BM_Error    BM_UserIO_getButtonEdgeState(const BM_UserIO_Button button, uint32_t * const edgeState, const BM_UserIO_Edge edge);

BM_Error    BM_UserIO_powerOnSelfTest();

BM_Error    BM_UserIO_setInterrupts(int32_t enable);
void        BM_UserIO_ISR();



#endif /* SRC_BM_USERIO_SERVICES_H_ */
