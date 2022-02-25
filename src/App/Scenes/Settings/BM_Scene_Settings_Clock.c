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

#include "BM_Scene_Settings_Clock.h"

#define BM_SCENE_SETTINGS_CLOCK_YPOS            40
#define BM_SCENE_SETTINGS_CLOCK_IND_YPOS        85
#define BM_SCENE_SETTINGS_CLOCK_HOUR_IND_XPOS   4
#define BM_SCENE_SETTINGS_CLOCK_MIN_IND_XPOS    72
#define BM_SCENE_SETTINGS_CLOCK_IND_WIDTH       52
#define BM_SCENE_SETTINGS_CLOCK_IND_HEIGHT      5

BM_RTCModule_TimeHMS _currentTime;


static int32_t _hourMinute = 0;
static int32_t _setTimeAndExit = 0;

static int32_t _BButtonPressed = 0;


BM_Error BM_Scene_Settings_Clock_init(BM_Scene *scene)
{
    if (scene == NULL)
        return BM_NULLPTR;

    scene->draw = &BM_Scene_Settings_Clock_draw;
    scene->handleUserIO = &BM_Scene_Settings_Clock_handleUserIO;
    scene->processAudio = &BM_Scene_Settings_Clock_processAudio;
    scene->reset = &BM_Scene_Settings_Clock_reset;
    scene->update = &BM_Scene_Settings_Clock_update;

    BM_RTCModule_getCurrentTimeHMS(&_currentTime);

    _hourMinute = 0;

    return BM_NO_ERR;
}


void BM_Scene_Settings_Clock_reset()
{
    _hourMinute = 0;
    _setTimeAndExit = 0;
    _BButtonPressed = 0;
}


void BM_Scene_Settings_Clock_update(void *data)
{
    BM_Controller_Menu_Data *sceneData = data;
    
    if (_setTimeAndExit || _BButtonPressed)
    {
        sceneData->sceneStackSize -= 1;

        if (_setTimeAndExit)
            BM_RTCModule_setTime(_currentTime);

        BM_Scene_Settings_Clock_reset();
    }
}


void BM_Scene_Settings_Clock_draw()
{
    BM_RE_drawLightBackground();

    BM_RE_drawString(40, 10, "Set Time");
    
    BM_RTCModule_TimeBCD timeBCD;
    BM_RTCModule_convertHMSToBCD(_currentTime, &timeBCD);

    BM_RE_drawTime(timeBCD, BM_SCENE_SETTINGS_CLOCK_YPOS);

    if (_hourMinute == 0)
        BM_RE_drawRectangleFilled(BM_SCENE_SETTINGS_CLOCK_HOUR_IND_XPOS,
                             BM_SCENE_SETTINGS_CLOCK_IND_YPOS,
                             BM_SCENE_SETTINGS_CLOCK_IND_WIDTH,
                             BM_SCENE_SETTINGS_CLOCK_IND_HEIGHT);
    
    if (_hourMinute == 1)
        BM_RE_drawRectangleFilled(BM_SCENE_SETTINGS_CLOCK_MIN_IND_XPOS,
                             BM_SCENE_SETTINGS_CLOCK_IND_YPOS,
                             BM_SCENE_SETTINGS_CLOCK_IND_WIDTH,
                             BM_SCENE_SETTINGS_CLOCK_IND_HEIGHT);
}


void BM_Scene_Settings_Clock_processAudio()
{

}


void BM_Scene_Settings_Clock_handleUserIO(const uint32_t buttonStates)
{
    int32_t buttonState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_UP, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
    {
        if (_hourMinute == 0)
            INCREMENT_WRAP(_currentTime.hour, 1, 24);
        if (_hourMinute == 1)
            INCREMENT_WRAP(_currentTime.minute, 1, 60);
    }

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_DOWN, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
    {
        if (_hourMinute == 0)
            DECREMENT_WRAP(_currentTime.hour, 1, 0, 23);
        if (_hourMinute == 1)
            DECREMENT_WRAP(_currentTime.minute, 1, 0, 59);
    }

    //  A Button Press
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
    {
        _hourMinute++;
        if (_hourMinute == 2)
            _setTimeAndExit = 1;
    }

    //  B Button Press
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_B, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        _BButtonPressed = 1;
}