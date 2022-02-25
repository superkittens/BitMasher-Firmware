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

#include "BM_Scene_Settings_Audio.h"


//  Audio options that the user can adjust
//  -------------------------------------------------   //
#define BM_SCENE_SETTINGS_AUDIO_NUM_SELECTIONS    3

#define BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN        0
#define BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN       1
#define BM_SCENE_SETTINGS_AUDIO_BASS_GAIN         2
//  -------------------------------------------------   //

//  Audio min/max/delta parameters
//  -------------------------------------------------   //
#define BM_SCENE_SETTINGS_AUDIO_GAIN_DELTA        1.f
#define BM_SCENE_SETTINGS_AUDIO_MIN_GAIN          -24.f
#define BM_SCENE_SETTINGS_AUDIO_MAX_GAIN          24.f

#define BM_SCENE_SETTINGS_AUDIO_TONE_DELTA        1.5f
#define BM_SCENE_SETTINGS_AUDIO_TONE_MIN          -10.5f
#define BM_SCENE_SETTINGS_AUDIO_TONE_MAX          12.f
//  -------------------------------------------------   //

#define BM_SCENE_SETTINGS_AUDIO_SLIDER_WIDTH      100
#define BM_SCENE_SETTINGS_AUDIO_SLIDER_X_OFFSET   15
#define BM_SCENE_SETTINGS_AUDIO_SLIDER_Y_OFFSET   20
#define BM_SCENE_SETTINGS_AUDIO_SLIDER_SPACING    8

#define BM_SCENE_SETTINGS_AUDIO_CURSOR_X_OFFSET   2
#define BM_SCENE_SETTINGS_AUDIO_CURSOR_Y_OFFSET   0
#define BM_SCENE_SETTINGS_AUDIO_CURSOR_WIDTH      6

#define BM_SCENE_SETTINGS_AUDIO_TEXT_XPOS         23
#define BM_SCENE_SETTINGS_AUDIO_TEXT_YPOS         5

#define BM_SCENE_SETTINGS_AUDIO_MAX_COUNTER_VAL   3

static int32_t _BButtonPressed = 0;
static int32_t _sliderCounter = 0;
static int32_t _leftRightButtonPressed = 0;

static BM_GUIObject_Slider  _selections[BM_SCENE_SETTINGS_AUDIO_NUM_SELECTIONS];

static int32_t              _currentSelection = 0;



static void BM_Scene_Settings_Audio_drawSelections()
{
    for (int32_t i = 0; i < BM_SCENE_SETTINGS_AUDIO_NUM_SELECTIONS; ++i)
    {
        BM_GUIObject_Slider_draw(&_selections[i]);

        if (_selections[i].active)
            BM_RE_drawRectangleFilled(  BM_SCENE_SETTINGS_AUDIO_CURSOR_X_OFFSET, 
                                        _selections[i].yPos + BM_SCENE_SETTINGS_AUDIO_CURSOR_Y_OFFSET,
                                        BM_SCENE_SETTINGS_AUDIO_CURSOR_WIDTH,
                                        BM_SCENE_SETTINGS_AUDIO_CURSOR_WIDTH);

        else
            BM_RE_drawRectangleNotFilled(  BM_SCENE_SETTINGS_AUDIO_CURSOR_X_OFFSET, 
                                        _selections[i].yPos + BM_SCENE_SETTINGS_AUDIO_CURSOR_Y_OFFSET,
                                        BM_SCENE_SETTINGS_AUDIO_CURSOR_WIDTH,
                                        BM_SCENE_SETTINGS_AUDIO_CURSOR_WIDTH);
    }
}


static void BM_Scene_Settings_Audio_onInputGainSliderChange(BM_GUIObject_Slider *slider, const BM_Direction direction)
{
    if (direction == BM_DIR_LEFT)
        DECREMENT_CLIP(slider->currentValue, slider->deltaValue, slider->minSliderValue);

    if (direction == BM_DIR_RIGHT)
        INCREMENT_CLIP(slider->currentValue, slider->deltaValue, slider->maxSliderValue);

    BM_AE_setADCVolume(slider->currentValue);
}


static void BM_Scene_Settings_Audio_onTrebleSliderChange(BM_GUIObject_Slider *slider, const BM_Direction direction)
{
    if (direction == BM_DIR_LEFT)
        DECREMENT_CLIP(slider->currentValue, slider->deltaValue, slider->minSliderValue);

    if (direction == BM_DIR_RIGHT)
        INCREMENT_CLIP(slider->currentValue, slider->deltaValue, slider->maxSliderValue);

    BM_AE_setTrebleGain(slider->currentValue);
}


static void BM_Scene_Settings_Audio_onBassSliderChange(BM_GUIObject_Slider *slider, const BM_Direction direction)
{
    if (direction == BM_DIR_LEFT)
        DECREMENT_CLIP(slider->currentValue, slider->deltaValue, slider->minSliderValue);

    if (direction == BM_DIR_RIGHT)
        INCREMENT_CLIP(slider->currentValue, slider->deltaValue, slider->maxSliderValue);

    BM_AE_setBassGain(slider->currentValue);
}


BM_Error BM_Scene_Settings_Audio_init(BM_Scene *scene)
{
    if (scene == NULL)
        return BM_NULLPTR;

    scene->draw = &BM_Scene_Settings_Audio_draw;
    scene->handleUserIO = &BM_Scene_Settings_Audio_handleUserIO;
    scene->processAudio = &BM_Scene_Settings_Audio_processAudio;
    scene->reset = &BM_Scene_Settings_Audio_reset;
    scene->update = &BM_Scene_Settings_Audio_update;

    //  Initialize selections
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].text = "Input Gain";
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].minSliderValue = BM_SCENE_SETTINGS_AUDIO_MIN_GAIN;
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].maxSliderValue = BM_SCENE_SETTINGS_AUDIO_MAX_GAIN;
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].deltaValue = BM_SCENE_SETTINGS_AUDIO_GAIN_DELTA;
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].sliderWidth = BM_SCENE_SETTINGS_AUDIO_SLIDER_WIDTH;
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].xPos = BM_SCENE_SETTINGS_AUDIO_SLIDER_X_OFFSET;
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].yPos = BM_SCENE_SETTINGS_AUDIO_SLIDER_Y_OFFSET;
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].onSliderChange = (void*)&BM_Scene_Settings_Audio_onInputGainSliderChange;
    _selections[BM_SCENE_SETTINGS_AUDIO_INPUT_GAIN].active = 1;

    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].text = "Treble";
    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].minSliderValue = BM_SCENE_SETTINGS_AUDIO_TONE_MIN;
    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].maxSliderValue = BM_SCENE_SETTINGS_AUDIO_TONE_MAX;
    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].deltaValue = BM_SCENE_SETTINGS_AUDIO_TONE_DELTA;
    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].sliderWidth = BM_SCENE_SETTINGS_AUDIO_SLIDER_WIDTH;
    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].xPos = BM_SCENE_SETTINGS_AUDIO_SLIDER_X_OFFSET;
    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].yPos = BM_SCENE_SETTINGS_AUDIO_SLIDER_Y_OFFSET + BM_GUIObject_Slider_getSliderHeight() + BM_SCENE_SETTINGS_AUDIO_SLIDER_SPACING;
    _selections[BM_SCENE_SETTINGS_AUDIO_TREBLE_GAIN].onSliderChange = (void*)&BM_Scene_Settings_Audio_onTrebleSliderChange;

    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].text = "Bass";
    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].minSliderValue = BM_SCENE_SETTINGS_AUDIO_TONE_MIN;
    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].maxSliderValue = BM_SCENE_SETTINGS_AUDIO_TONE_MAX;
    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].deltaValue = BM_SCENE_SETTINGS_AUDIO_TONE_DELTA;
    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].sliderWidth = BM_SCENE_SETTINGS_AUDIO_SLIDER_WIDTH;
    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].xPos = BM_SCENE_SETTINGS_AUDIO_SLIDER_X_OFFSET;
    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].yPos = BM_SCENE_SETTINGS_AUDIO_SLIDER_Y_OFFSET + (BM_GUIObject_Slider_getSliderHeight() + BM_SCENE_SETTINGS_AUDIO_SLIDER_SPACING) * 2;
    _selections[BM_SCENE_SETTINGS_AUDIO_BASS_GAIN].onSliderChange = (void*)&BM_Scene_Settings_Audio_onBassSliderChange;

    return BM_NO_ERR;
}


void BM_Scene_Settings_Audio_reset()
{
    _BButtonPressed = 0;
    _selections[_currentSelection].active = 0;
    _currentSelection = 0;
    _leftRightButtonPressed = 0;
}


void BM_Scene_Settings_Audio_update(void *data)
{
    BM_Controller_Menu_Data *sceneData = data;

    //  When exiting the settings menu, pop the settings scene off of the stack
    if (_BButtonPressed)
    {
        sceneData->sceneStackSize -= 1;
        BM_Scene_Settings_Audio_reset();

        return;
    }
}


void BM_Scene_Settings_Audio_draw()
{
        BM_RE_drawLightBackground();

    BM_RE_drawString(BM_SCENE_SETTINGS_AUDIO_TEXT_XPOS, BM_SCENE_SETTINGS_AUDIO_TEXT_YPOS, "Audio Settings");
    BM_Scene_Settings_Audio_drawSelections();
}


void BM_Scene_Settings_Audio_processAudio()
{

}


void BM_Scene_Settings_Audio_handleUserIO(const uint32_t buttonStates)
{
    //  Pressing the B or Menu Button will exit the settings menu back to the main menu
    int32_t buttonState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_B, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        _BButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_MENU, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
        _BButtonPressed = 1;

    //  Pressing left or right buttons should change the slider that is currently selected
    int32_t leftButtonState = 0; 
    int32_t rightButtonState = 0;

    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_LEFT, &leftButtonState));
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_RIGHT, &rightButtonState));
    if (!leftButtonState || !rightButtonState)
    {
        if (!_leftRightButtonPressed)
        {
            _leftRightButtonPressed = 1;

            if (!leftButtonState)
                _selections[_currentSelection].onSliderChange((struct BM_GUIObject_Slider*)&_selections[_currentSelection], BM_DIR_LEFT);
            else
                _selections[_currentSelection].onSliderChange((struct BM_GUIObject_Slider*)&_selections[_currentSelection], BM_DIR_RIGHT);
        }

        else
        {
            if (_sliderCounter <= BM_SCENE_SETTINGS_AUDIO_MAX_COUNTER_VAL)
                _sliderCounter++;
            else
            {
                if (!leftButtonState)
                    _selections[_currentSelection].onSliderChange((struct BM_GUIObject_Slider*)&_selections[_currentSelection], BM_DIR_LEFT);

                if (!rightButtonState)
                    _selections[_currentSelection].onSliderChange((struct BM_GUIObject_Slider*)&_selections[_currentSelection], BM_DIR_RIGHT);    
            }
        }
    }

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_LEFT, &leftButtonState, BM_USERIO_RISING));
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_RIGHT, &rightButtonState, BM_USERIO_RISING));
    if (leftButtonState || rightButtonState)
    {
        _sliderCounter = 0;
        _leftRightButtonPressed = 0;
    }

    //  Pressing the up/down buttons change the active selection
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_DOWN, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
    {
        _selections[_currentSelection].active = 0;
        INCREMENT_WRAP(_currentSelection, 1, BM_SCENE_SETTINGS_AUDIO_NUM_SELECTIONS);
        _selections[_currentSelection].active = 1;
    }

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_UP, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
    {
        _selections[_currentSelection].active = 0;
        DECREMENT_WRAP(_currentSelection, 1, 0, BM_SCENE_SETTINGS_AUDIO_NUM_SELECTIONS - 1);
        _selections[_currentSelection].active = 1;
    }
}