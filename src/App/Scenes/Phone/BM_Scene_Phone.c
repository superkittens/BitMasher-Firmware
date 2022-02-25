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


#include "BM_Scene_Phone.h"

//  Internal state machines
static enum
{
    BM_SCENE_PHONE_STATE_NORMAL = 0,
    BM_SCENE_PHONE_STATE_PHONEBOOK
}BM_Scene_Phone_State;

static void (*_currentEffectUpdate)(void);
static void (*_currentEffectProcessAudio)(float32_t *buffer, const size_t bufferSize);
static void (*_currentEffectDraw)(void);
static void (*_currentEffectHandleUserIO)(const uint32_t buttonStates);
static void (*_currentEffectSetActive)(void);
static void (*_currentEffectSetInactive)(void);
static void (*_currentEffectReset)(void);
static BM_Scene_Phone_AudioState (*_currentEffectGetAudioState)(void);

static int32_t         _currentState = BM_SCENE_PHONE_STATE_NORMAL;
static BM_Direction    _activeEffect = BM_DIR_RIGHT;

static int32_t         _triggerPhoneBookTransition = 0;
static int32_t         _triggerEffectActivation = 0;
static BM_Direction    _currentlyActiveEffect = BM_DIR_RIGHT;




static void BM_Scene_Phone_onPhonebookExit()
{
    if (_currentlyActiveEffect == BM_DIR_RIGHT)
        BM_SceneDelegate_Phone_Caller_setActive();

    else
        _currentEffectSetActive();

    _currentState = BM_SCENE_PHONE_STATE_NORMAL;
}


static void BM_Scene_Phone_prepareNewEffect(const int32_t effect)
{
    switch (effect)
    {
        //  If the user selects flutter
        case 0:
        {
            _currentEffectUpdate = &BM_SceneDelegate_Phone_Flutter_update;
            _currentEffectDraw = &BM_SceneDelegate_Phone_Flutter_draw;
            _currentEffectProcessAudio = &BM_SceneDelegate_Phone_Flutter_processAudio;
            _currentEffectHandleUserIO = &BM_SceneDelegate_Phone_Flutter_handleUserIO;
            _currentEffectSetActive = &BM_SceneDelegate_Phone_Flutter_setActive;
            _currentEffectSetInactive = &BM_SceneDelegate_Phone_Flutter_setInactive;
            _currentEffectReset = &BM_SceneDelegate_Phone_Flutter_reset;
            _currentEffectGetAudioState = &BM_SceneDelegate_Phone_Flutter_getAudioState;

            break;
        }

        case 1:
        {
            _currentEffectUpdate = &BM_SceneDelegate_Phone_BC_update;
            _currentEffectDraw = &BM_SceneDelegate_Phone_BC_draw;
            _currentEffectProcessAudio = &BM_SceneDelegate_Phone_BC_processAudio;
            _currentEffectHandleUserIO = &BM_SceneDelegate_Phone_BC_handleUserIO;
            _currentEffectSetActive = &BM_SceneDelegate_Phone_BC_setActive;
            _currentEffectSetInactive = &BM_SceneDelegate_Phone_BC_setInactive;
            _currentEffectReset = &BM_SceneDelegate_Phone_BC_reset;
            _currentEffectGetAudioState = &BM_SceneDelegate_Phone_BC_getAudioState;

            break;
        }

        default:
        {
            _currentEffectUpdate = &BM_SceneDelegate_Phone_Flutter_update;
            _currentEffectDraw = &BM_SceneDelegate_Phone_Flutter_draw;
            _currentEffectProcessAudio = &BM_SceneDelegate_Phone_Flutter_processAudio;
            _currentEffectHandleUserIO = &BM_SceneDelegate_Phone_Flutter_handleUserIO;
            _currentEffectSetActive = &BM_SceneDelegate_Phone_Flutter_setActive;
            _currentEffectSetInactive = &BM_SceneDelegate_Phone_Flutter_setInactive;
            _currentEffectReset = &BM_SceneDelegate_Phone_Flutter_reset;
            _currentEffectGetAudioState = &BM_SceneDelegate_Phone_Flutter_getAudioState;
        }
    }
}



//  ================================================================================================    //
//  ================================================================================================    //



BM_Error BM_Scene_Phone_init(BM_Scene *scene)
{
    if (scene == NULL)
        return BM_NULLPTR;

    BM_Error err;
    err = BM_SceneDelegate_Phone_Caller_init();
    if (err != BM_NO_ERR)
        return err;

    err = BM_SceneDelegate_Phone_Flutter_init();
    if (err != BM_NO_ERR)
        return err;

    err = BM_SceneDelegate_Phone_BC_init();
    if (err != BM_NO_ERR)
        return err;

    scene->update = &BM_Scene_Phone_update;
    scene->draw = &BM_Scene_Phone_draw;
    scene->processAudio = &BM_Scene_Phone_processAudio;
    scene->handleUserIO = &BM_Scene_Phone_handleUserIO;
    scene->reset = &BM_Scene_Phone_reset;
    scene->iconBitmap = BM_ASSETS_TELEPHONE_ICON;
    scene->sceneName = "Phone";

    //  Start at flutter effect in case the user presses B when first starting the Phone scene
    _currentEffectUpdate = &BM_SceneDelegate_Phone_Flutter_update;
    _currentEffectDraw = &BM_SceneDelegate_Phone_Flutter_draw;
    _currentEffectProcessAudio = &BM_SceneDelegate_Phone_Flutter_processAudio;
    _currentEffectHandleUserIO = &BM_SceneDelegate_Phone_Flutter_handleUserIO;
    _currentEffectSetActive = &BM_SceneDelegate_Phone_Flutter_setActive;
    _currentEffectSetInactive = &BM_SceneDelegate_Phone_Flutter_setInactive;
    _currentEffectReset = &BM_SceneDelegate_Phone_Flutter_reset;
    _currentEffectGetAudioState = &BM_SceneDelegate_Phone_Flutter_getAudioState;
    
    return BM_NO_ERR;
}


void BM_Scene_Phone_reset()
{
    _currentEffectUpdate = &BM_SceneDelegate_Phone_Flutter_update;
    _currentEffectDraw = &BM_SceneDelegate_Phone_Flutter_draw;
    _currentEffectProcessAudio = &BM_SceneDelegate_Phone_Flutter_processAudio;
    _currentEffectHandleUserIO = &BM_SceneDelegate_Phone_Flutter_handleUserIO;
    _currentEffectSetActive = &BM_SceneDelegate_Phone_Flutter_setActive;
    _currentEffectSetInactive = &BM_SceneDelegate_Phone_Flutter_setInactive;
    _currentEffectReset = &BM_SceneDelegate_Phone_Flutter_reset;
    _currentEffectGetAudioState = &BM_SceneDelegate_Phone_Flutter_getAudioState;

    _currentlyActiveEffect = BM_DIR_RIGHT;

    BM_SceneDelegate_Phone_BC_reset();
    BM_SceneDelegate_Phone_Caller_reset();
    BM_SceneDelegate_Phone_Flutter_reset();
    BM_SceneDelegate_Phone_Phonebook_reset();

    BM_SceneDelegate_Phone_Caller_setActive();
}


void BM_Scene_Phone_update(void *data)
{
    if (_currentState == BM_SCENE_PHONE_STATE_NORMAL)
    {
        _currentEffectUpdate();
        BM_SceneDelegate_Phone_Caller_update();
    }

    //  Make sure that all the effects have moved to inactive state and audio is off 
    //  before moving to the phone book state
    if (_triggerPhoneBookTransition)
    {
        _currentState = BM_SCENE_PHONE_STATE_PHONEBOOK;
        _triggerPhoneBookTransition = 0;
    }

    //  If the user triggered an effect change, we need to make sure that the audio is fully off for 
    //  the current effect and reset before switching over to the new effect
    if (_triggerEffectActivation)
    {
        //  If we are changing to the left effect
        if (_currentlyActiveEffect == BM_DIR_RIGHT)
        {
            if (BM_SceneDelegate_Phone_Caller_getAudioState() == BM_SCENE_PHONE_AUDIO_STATE_NORMAL)
            {
                _currentEffectSetActive();
                BM_SceneDelegate_Phone_Caller_setInactive();

                _currentlyActiveEffect = BM_DIR_LEFT;
                _triggerEffectActivation = 0;
            }
        }

        //  If we are changing to the right effect
        else
        {
            if (_currentEffectGetAudioState() == BM_SCENE_PHONE_AUDIO_STATE_NORMAL)
            {
                BM_SceneDelegate_Phone_Caller_setActive();

                _currentlyActiveEffect = BM_DIR_RIGHT;
                _triggerEffectActivation = 0;
            }
        }
    }

}


void BM_Scene_Phone_draw()
{
    if (_currentState == BM_SCENE_PHONE_STATE_PHONEBOOK)
        BM_SceneDelegate_Phone_Phonebook_draw();
    else
        _currentEffectDraw();

    BM_SceneDelegate_Phone_Caller_draw();
}


void BM_Scene_Phone_processAudio(float32_t* inputBuffer, const size_t bufferSize)
{
    //  Note that when the scene is in phone book mode, none of these functions should
    //  actually be processing audio (both internal states are inactive)
    //  TODO:  Ensure only one effect is active at one time
    if (_currentlyActiveEffect == BM_DIR_LEFT)
        _currentEffectProcessAudio(inputBuffer, bufferSize);
    else
        BM_SceneDelegate_Phone_Caller_processAudio(inputBuffer, bufferSize);
}


void BM_Scene_Phone_handleUserIO(const uint32_t buttonStates)
{
    switch (_currentState)
    {
        case BM_SCENE_PHONE_STATE_NORMAL:
        {
            //  Check for effect changes (d-pad)
            uint32_t buttonEdgeState = 0;
            BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_LEFT, &buttonEdgeState, BM_USERIO_FALLING));
            if (buttonEdgeState && (_currentlyActiveEffect == BM_DIR_RIGHT))
            {
                _triggerEffectActivation = 1;
                BM_SceneDelegate_Phone_Caller_setInactive();
            }

            buttonEdgeState = 0;
            BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_RIGHT, &buttonEdgeState, BM_USERIO_FALLING));
            if (buttonEdgeState && (_currentlyActiveEffect == BM_DIR_LEFT))
            {
                _triggerEffectActivation = 1;
                _currentEffectSetInactive();
            }

            //  If B was pressed (phone book)
            buttonEdgeState = 0;
            BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_B, &buttonEdgeState, BM_USERIO_FALLING));
            if (buttonEdgeState)
            {
                _triggerPhoneBookTransition = 1;
                BM_SceneDelegate_Phone_Caller_setInactive();
                BM_SceneDelegate_Phone_Caller_reset();
                _currentEffectSetInactive();
                _currentEffectReset();
                break;
            }

            BM_SceneDelegate_Phone_Caller_handleUserIO(buttonStates);
            _currentEffectHandleUserIO(buttonStates);

            break;
        }

        case BM_SCENE_PHONE_STATE_PHONEBOOK:
        {
            BM_SceneDelegate_Phone_Phonebook_handleUserIO(buttonStates);

            //  Change to new chosen effet
            uint32_t buttonEdgeState = 0;
            BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonEdgeState, BM_USERIO_FALLING));
            if (buttonEdgeState)
            {
                int32_t effectSelected = BM_SceneDelegate_Phone_Phonebook_onAPressed();
                BM_Scene_Phone_prepareNewEffect(effectSelected);
                BM_Scene_Phone_onPhonebookExit();
            }

            //  Exit phone book
            BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_B, &buttonEdgeState, BM_USERIO_FALLING));
            if (buttonEdgeState)
            {
                BM_Scene_Phone_onPhonebookExit();
            }

            break;
        }
    }
}
