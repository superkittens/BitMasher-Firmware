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


#include "BM_Scene_Settings.h"

#define BM_SCENE_SETTINGS_NUM_SELECTIONS 2
#define BM_SCENE_SETTINGS_CURSOR_X_OFFSET 2
#define BM_SCENE_SETTINGS_CURSOR_Y_OFFSET 0

#define BM_SCENE_SETTINGS_TITLE_X_OFFSET 40
#define BM_SCENE_SETTINGS_TITLE_Y_OFFSET 5

#define BM_SCENE_SETTINGS_OPTIONS_X_OFFSET 5
#define BM_SCENE_SETTINGS_OPTIONS_Y_OFFSET 20
#define BM_SCENE_SETTINGS_OPTIONS_SPACING  11

BM_GUIObject_Selection _selections[BM_SCENE_SETTINGS_NUM_SELECTIONS];
static int32_t _currentSelection = 0;

static int32_t _AButtonPressed = 0;
static int32_t _BButtonPressed = 0;
static int32_t _menuButtonPressed = 0;



BM_Error BM_Scene_Settings_init(BM_Scene *scene)
{
    if (scene == NULL)
        return BM_NULLPTR;

    scene->draw = &BM_Scene_Settings_draw;
    scene->handleUserIO = &BM_Scene_Settings_handleUserIO;
    scene->processAudio = &BM_Scene_Settings_processAudio;
    scene->reset = &BM_Scene_Settings_reset;
    scene->update = &BM_Scene_Settings_update;

    _selections[0].text = "Audio Settings";
    _selections[0].xPos = BM_SCENE_SETTINGS_OPTIONS_X_OFFSET;
    _selections[0].yPos = BM_SCENE_SETTINGS_OPTIONS_Y_OFFSET;
    _selections[0].active = 1;

    _selections[1].text = "Clock Settings";
    _selections[1].xPos = BM_SCENE_SETTINGS_OPTIONS_X_OFFSET;
    _selections[1].yPos = BM_SCENE_SETTINGS_OPTIONS_Y_OFFSET + BM_SCENE_SETTINGS_OPTIONS_SPACING;
    _selections[1].active = 0;

    return BM_NO_ERR;
}


void BM_Scene_Settings_reset()
{
    _AButtonPressed = 0;
    _BButtonPressed = 0;
    _menuButtonPressed = 0;

    _selections[_currentSelection].active = 0;
    _selections[0].active = 1;
    _currentSelection = 0;
}


void BM_Scene_Settings_update(void *data)
{
    BM_Controller_Menu_Data *sceneData = data;

    //  If A Button is pressed go to the settings page pointed to by _currentSelection
    if (_AButtonPressed)
    {
        if (_currentSelection == 0)
        {
            sceneData->sceneStackSize += 1;
            if (sceneData->sceneStackSize > sceneData->MAX_STACK_SIZE)
                while(1);

            BM_ASSERT(BM_Scene_Settings_Audio_init(&sceneData->sceneStack[sceneData->sceneStackSize - 1]));
        }
        
        if (_currentSelection == 1)
        {
            sceneData->sceneStackSize += 1;
            if (sceneData->sceneStackSize > sceneData->MAX_STACK_SIZE)
                while(1);

            BM_ASSERT(BM_Scene_Settings_Clock_init(&sceneData->sceneStack[sceneData->sceneStackSize - 1]));
        }

        BM_Scene_Settings_reset();
    }

    if (_BButtonPressed || _menuButtonPressed)
    {
        sceneData->sceneStackSize -= 1;
        BM_Scene_Settings_reset();
    }
}


void BM_Scene_Settings_draw()
{
    BM_RE_drawLightBackground();
    BM_RE_drawString(BM_SCENE_SETTINGS_TITLE_X_OFFSET,
                BM_SCENE_SETTINGS_TITLE_Y_OFFSET,
                "Settings");

    for (int32_t i = 0; i < BM_SCENE_SETTINGS_NUM_SELECTIONS; ++i)
        BM_GUIObject_Selection_draw(&_selections[i]);
}


void BM_Scene_Settings_processAudio()
{

}


void BM_Scene_Settings_handleUserIO(const uint32_t buttonStates)
{
    _selections[_currentSelection].active = 0;

    int32_t buttonState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_DOWN, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        INCREMENT_WRAP(_currentSelection, 1, BM_SCENE_SETTINGS_NUM_SELECTIONS);

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_UP, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        DECREMENT_WRAP(_currentSelection, 1, 0, BM_SCENE_SETTINGS_NUM_SELECTIONS - 1);

    _selections[_currentSelection].active = 1;

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        _AButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_B, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        _BButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_MENU, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        _menuButtonPressed = 1;
}
