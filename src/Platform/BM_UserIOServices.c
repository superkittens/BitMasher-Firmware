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

#ifdef HW_P1
#define BM_IO_MAP_UP_PORT gpioPortF
#define BM_IO_MAP_DOWN_PORT gpioPortF
#define BM_IO_MAP_LEFT_PORT gpioPortF
#define BM_IO_MAP_RIGHT_PORT gpioPortF

#else
#define BM_IO_MAP_UP_PORT gpioPortA
#define BM_IO_MAP_DOWN_PORT gpioPortA
#define BM_IO_MAP_LEFT_PORT gpioPortA
#define BM_IO_MAP_RIGHT_PORT gpioPortA
#endif

#define BM_IO_MAP_A_PORT gpioPortF
#define BM_IO_MAP_B_PORT gpioPortF
#define BM_IO_MAP_MENU_PORT gpioPortF
#define BM_IO_MAP_SHIFT_PORT gpioPortF

#ifdef HW_P1
#define BM_IO_MAP_UP_PIN 0
#define BM_IO_MAP_DOWN_PIN 1
#define BM_IO_MAP_LEFT_PIN 2
#define BM_IO_MAP_RIGHT_PIN 3
#else

#define BM_IO_MAP_UP_PIN 2
#define BM_IO_MAP_DOWN_PIN 3
#define BM_IO_MAP_LEFT_PIN 4
#define BM_IO_MAP_RIGHT_PIN 5
#endif

#define BM_IO_MAP_A_PIN 4
#define BM_IO_MAP_B_PIN 5
#define BM_IO_MAP_MENU_PIN 6
#define BM_IO_MAP_SHIFT_PIN 7

//  External GPIO Interrupt Triggers are mapped to EXTI DEPENDING on the pin number!
//  Pins 0, 1, 2 and 3 belong to GROUP 0 (pins 0 -3)
//  Pins 4, 5, 6 and 7 belong to GROUP 1 (pins 4 - 7)
#ifdef HW_P1
#define BM_IO_A_EXTI      0
#define BM_IO_B_EXTI      1
#define BM_IO_MENU_EXTI   2
#define BM_IO_SHIFT_EXTI  3

#else
#define BM_IO_A_EXTI      4
#define BM_IO_B_EXTI      5
#define BM_IO_MENU_EXTI   6
#define BM_IO_SHIFT_EXTI  7
#endif


//  ====================================================================================================== //

static int32_t  _userIOInitialized = 0;
static uint32_t _prevButtonStates = 0xFFFFFFFF;
static uint32_t _currentButtonStates = 0xFFFFFFFF;


extern volatile int32_t BM_Controller_Sleep_wake;





BM_Error BM_UserIO_init()
{
  CMU_ClockEnable(cmuClock_GPIO, true);
  
  GPIO_PinModeSet(BM_IO_MAP_A_PORT, BM_IO_MAP_A_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BM_IO_MAP_B_PORT, BM_IO_MAP_B_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BM_IO_MAP_UP_PORT, BM_IO_MAP_UP_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BM_IO_MAP_DOWN_PORT, BM_IO_MAP_DOWN_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BM_IO_MAP_LEFT_PORT, BM_IO_MAP_LEFT_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BM_IO_MAP_RIGHT_PORT, BM_IO_MAP_RIGHT_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BM_IO_MAP_MENU_PORT, BM_IO_MAP_MENU_PIN, gpioModeInputPullFilter, 1);
  GPIO_PinModeSet(BM_IO_MAP_SHIFT_PORT, BM_IO_MAP_SHIFT_PIN, gpioModeInputPullFilter, 1);

  BM_ASSERT(BM_ISR_GPIO_registerUserIOISR(BM_UserIO_ISR));

  _userIOInitialized = 1;

  return BM_NO_ERR;
}


/*
 *  Call this function at regular intervals to get the latest state of the GPIOs
 *  Note that button presses are active low
 *  To get the state of the IOs, either BM_UserIO_getButtonStates() or BM_UserIO_getButtonEdgeStates()
 *  should be called.
 */
BM_Error BM_UserIO_update()
{
  if (!_userIOInitialized)
    return BM_USERIO_NOT_INITIALIZED;

  _prevButtonStates = _currentButtonStates;
  _currentButtonStates = 0;

  uint32_t portState = GPIO_PortInGet(BM_IO_MAP_A_PORT);
  if (portState & (1 << BM_IO_MAP_A_PIN))
    _currentButtonStates |= (1 << BM_USERIO_A);

  portState = GPIO_PortInGet(BM_IO_MAP_B_PORT);
  if (portState & (1 << BM_IO_MAP_B_PIN))
    _currentButtonStates |= (1 << BM_USERIO_B);

  portState = GPIO_PortInGet(BM_IO_MAP_UP_PORT);
  if (portState & (1 << BM_IO_MAP_UP_PIN))
    _currentButtonStates |= (1 << BM_USERIO_UP);

  portState = GPIO_PortInGet(BM_IO_MAP_DOWN_PORT);
  if (portState & (1 << BM_IO_MAP_DOWN_PIN))
    _currentButtonStates |= (1 << BM_USERIO_DOWN);

  portState = GPIO_PortInGet(BM_IO_MAP_LEFT_PORT);
  if (portState & (1 << BM_IO_MAP_LEFT_PIN))
    _currentButtonStates |= (1 << BM_USERIO_LEFT);

  portState = GPIO_PortInGet(BM_IO_MAP_RIGHT_PORT);
  if (portState & (1 << BM_IO_MAP_RIGHT_PIN))
    _currentButtonStates |= (1 << BM_USERIO_RIGHT);

  portState = GPIO_PortInGet(BM_IO_MAP_MENU_PORT);
  if (portState & (1 << BM_IO_MAP_MENU_PIN))
    _currentButtonStates |= (1 << BM_USERIO_MENU);

  return BM_NO_ERR;
}


/*
 *  Get the states of all of the buttons
 *  Note that button presses are active low!
 */
BM_Error BM_UserIO_getAllButtonStates(uint32_t * const buttonStates)
{
  if (!_userIOInitialized)
    return BM_USERIO_NOT_INITIALIZED;

  *buttonStates = _currentButtonStates;

  return BM_NO_ERR;
}


/*
 *  Get the state of a particular button
 *  If the button is pressed, buttonState should return 0
 *  If the button is NOT pressed, buttonState should return some value > 0
 */
BM_Error BM_UserIO_getButtonState(const BM_UserIO_Button button, uint32_t * const buttonState)
{
  if (!_userIOInitialized)
    return BM_USERIO_NOT_INITIALIZED;

  if (button >= BM_USERIO_NUM_BUTTONS)
    return BM_USERIO_INVALID_BUTTON;

  if (buttonState == NULL)
    return BM_NULLPTR;

  *buttonState = 0;
  *buttonState = _currentButtonStates & (1 << button);

  return BM_NO_ERR;
}


/*
 *  Get information about the rising/falling edges of all buttons
 *  If the user pressed a button, then there is a falling edge from the transition from high to low
 *  This may be preferable over reading the button states when writing logic for button presses in certain scenes
 * 
 *  If a rising/falling edge is detected on a button, the corresponding bit in edgeStates will be set
 */
BM_Error BM_UserIO_getAllButtonEdgeStates(uint32_t * const edgeStates, const BM_UserIO_Edge edge)
{
  if (!_userIOInitialized)
    return BM_USERIO_NOT_INITIALIZED;

  if (edge >= BM_USERIO_NUM_EDGES)
    return BM_USERIO_INVALID_EDGE;

  if (edgeStates == NULL)
    return BM_NULLPTR;

  *edgeStates = 0;

  for (BM_UserIO_Button button = BM_USERIO_A; button < BM_USERIO_NUM_BUTTONS; ++button)
  {
    uint32_t state = 0;
    BM_Error error = BM_UserIO_getButtonEdgeState(button, &state, edge);
    if (error != BM_NO_ERR)
      return error;

    if (state)
      *edgeStates |= (1 << button);
  }

  return BM_NO_ERR;
}


BM_Error BM_UserIO_getButtonEdgeState(const BM_UserIO_Button button, uint32_t * const edgeState, const BM_UserIO_Edge edge)
{
  if (!_userIOInitialized)
    return BM_USERIO_NOT_INITIALIZED;

  if (edge >= BM_USERIO_NUM_EDGES)
    return BM_USERIO_INVALID_EDGE;

  if (edgeState == NULL)
    return BM_NULLPTR;

  *edgeState = 0;

  uint32_t prevMasked = _prevButtonStates & (1 << button);
  uint32_t currentMasked = _currentButtonStates & (1 << button);
  uint32_t combined = prevMasked ^ currentMasked;   //  If there is a change, the XOR operation will flag it

  if (combined)
  {
    if (edge == BM_USERIO_RISING)
    {
      if (!prevMasked)
      {
        *edgeState |= (1 << button);
      }
    }

    else
    {
      if (prevMasked)
      {
        *edgeState |= (1 << button);
      }
    }
  }

  return BM_NO_ERR;
}


BM_Error BM_UserIO_setInterrupts(int32_t enable)
{
  if (!_userIOInitialized)
    return BM_USERIO_NOT_INITIALIZED;

  //  If interrupts are enabled, A, B, MENU, SHIFT presses will trigger an interrupt
  if (enable)
  {
    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_A_PIN, BM_IO_A_EXTI, 0, 1, true);
    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_B_PIN, BM_IO_B_EXTI, 0, 1, true);
    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_MENU_PIN, BM_IO_MENU_EXTI, 0, 1, true);
    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_SHIFT_PIN, BM_IO_SHIFT_EXTI, 0, 1, true);
    
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);
    NVIC_EnableIRQ(GPIO_ODD_IRQn);

    GPIO_IntClear(0xFFFF);
  }
  else
  {
    NVIC_DisableIRQ(GPIO_EVEN_IRQn);
    NVIC_DisableIRQ(GPIO_ODD_IRQn);

    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_A_PIN, BM_IO_A_EXTI, 0, 1, false);
    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_B_PIN, BM_IO_B_EXTI, 0, 1, false);
    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_MENU_PIN, BM_IO_MENU_EXTI, 0, 1, false);
    GPIO_ExtIntConfig(BM_IO_MAP_A_PORT, BM_IO_MAP_SHIFT_PIN, BM_IO_SHIFT_EXTI, 0, 1, false);
  }

  return BM_NO_ERR;
}


void BM_UserIO_ISR()
{
  GPIO_IntClear(0xFFFF);
  BM_Controller_Sleep_wake = 1;
}
