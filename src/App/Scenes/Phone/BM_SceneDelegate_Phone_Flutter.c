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


#include "BM_SceneDelegate_Phone_Flutter.h"


#define BM_SCENE_DELEGATE_PHONE_FLUTTER_DELAY_LENGTH    2048

#define BM_SCENE_DELEGATE_PHONE_FLUTTER_AUDIO_GAIN      0.15f
#define BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MAX         250.f
#define BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MIN         24.f
#define BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_DELTA       2.f
#define BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_NUM_STEPS   ((BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MAX - BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MIN) / BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_DELTA)

#define BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_MAX   5.f
#define BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_MIN   1.f
#define BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_DELTA ((BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_MAX - BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_MIN) / BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_NUM_STEPS)



static MW_AFXUnit_Flutter   _flutter;
static MW_AFXUnit_Leslie    _leslie;
static float32_t            _delayLine[BM_SCENE_DELEGATE_PHONE_FLUTTER_DELAY_LENGTH];
static float32_t            _flutterDepth = 25.f;
static float32_t            _flutterFreq = 15.f;
static float32_t            _rpm;
static float32_t            _prevRPM;

static int32_t              _propCounter = 0;
static float32_t            _propFrameChangeValue;


static BM_Scene_Phone_AudioState    _currentAudioState;
static BM_Scene_Phone_EffectState   _currentState;

static int32_t          _drawPhoneNoise = 0;
static int32_t          _drawMusicNotes = 0;

static BM_Sprite        _hipsterCat;
static BM_Sprite        _receiver;
static BM_Animation     _propellerSpin;
static BM_Sprite        _phoneNoise;
static BM_Sprite        _musicNotes;

static int32_t          _AButtonPressed = 0;
static int32_t          _upButtonPressed = 0;
static int32_t          _downButtonPressed = 0;


BM_Error BM_SceneDelegate_Phone_Flutter_init()
{
    _rpm = BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MIN;
    _prevRPM = _rpm;

    int32_t success = MW_AFXUnit_Leslie_init(&_leslie, _delayLine, BM_SCENE_DELEGATE_PHONE_FLUTTER_DELAY_LENGTH, FS, _rpm);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;

    //  Initialize sprites and animations
    _hipsterCat.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT;
    _hipsterCat.currentMask = NULL;
    _hipsterCat.currentXPos = 20;
    _hipsterCat.currentYPos = 17;
    _hipsterCat.width = BM_ASSETS_TELEPHONE_HIPSTER_CAT_WIDTH;
    _hipsterCat.height = BM_ASSETS_TELEPHONE_HIPSTER_CAT_HEIGHT;

    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_INACTIVE;
    _receiver.currentMask = NULL;
    _receiver.currentXPos = 14;
    _receiver.currentYPos = 57;
    _receiver.width = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_INACTIVE_WIDTH;
    _receiver.height = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_INACTIVE_HEIGHT;

    _phoneNoise.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_NOISE;
    _phoneNoise.currentMask = NULL;
    _phoneNoise.currentXPos = 12;
    _phoneNoise.currentYPos = 49;
    _phoneNoise.width = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_NOISE_WIDTH;
    _phoneNoise.height = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_NOISE_HEIGHT;

    _musicNotes.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT_MUSIC_NOTES;
    _musicNotes.currentMask = NULL;
    _musicNotes.currentXPos = 9;
    _musicNotes.currentYPos = 11;
    _musicNotes.width = BM_ASSETS_TELEPHONE_HIPSTER_CAT_MUSIC_NOTES_WIDTH;
    _musicNotes.height = BM_ASSETS_TELEPHONE_HIPSTER_CAT_MUSIC_NOTES_HEIGHT;

    _propellerSpin.frames = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PROPELLER;
    _propellerSpin.useMask = 0;
    _propellerSpin.numFrames = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PROPELLER_NUM_FRAMES;
    _propellerSpin.width = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PROPELLER_WIDTH;
    _propellerSpin.height = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PROPELLER_HEIGHT;
    BM_Animation_init(&_propellerSpin, 0);

    _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;
    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;

    _rpm = BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MIN;
    _propFrameChangeValue = BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_MAX;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Phone_Flutter_update()
{
    //  Update propellor animation (if applicable)
    _propCounter++;
    if (_propCounter >= (int32_t)_propFrameChangeValue)
    {
        _propCounter = 0;
        BM_Animation_update(&_propellerSpin);
    }

    switch (_currentState)
    {
        case BM_SCENE_PHONE_EFFECT_STATE_ACTIVE:
        {
            _drawPhoneNoise = 0;

            if (_AButtonPressed)
            {
                if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_NORMAL)
                    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_NORMAL;

                _hipsterCat.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT_TALK;
                _drawMusicNotes = 1;
            }
            else
            {
                if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX || _currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FX)
                    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX;

                _hipsterCat.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT;
                _drawMusicNotes = 0;
            }

            //  Modify flutter effect RPM value and propellor spin rate if up/down buttons are pressed
            if (_upButtonPressed)
            {
                INCREMENT_CLIP(_rpm, BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_DELTA, BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MAX);
                DECREMENT_CLIP(_propFrameChangeValue, BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_DELTA, BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_MIN);
                MW_AFXUnit_Leslie_changeParameters(&_leslie, _rpm);

                _upButtonPressed = 0;
            }

            if (_downButtonPressed)
            {
                DECREMENT_CLIP(_rpm, BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_DELTA, BM_SCENE_DELEGATE_PHONE_FLUTTER_RPM_MIN);
                INCREMENT_CLIP(_propFrameChangeValue, BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_DELTA, BM_SCENE_DELEGATE_PHONE_FLUTTER_PROP_RATE_MAX);
                MW_AFXUnit_Leslie_changeParameters(&_leslie, _rpm);

                _downButtonPressed = 0;
            }

            break;
        }

        case BM_SCENE_PHONE_EFFECT_STATE_INACTIVE:
        {
            _drawMusicNotes = 0;

            if (_AButtonPressed)
                _drawPhoneNoise = 1;
            else
                _drawPhoneNoise = 0;
                
            break;
        }
    }
}


void BM_SceneDelegate_Phone_Flutter_draw()
{
    BM_RE_drawBackground(BM_ASSETS_TELEPHONE_HIPSTER_BKGND);

    BM_RE_drawSprite(_receiver.currentBitmap,
                    NULL,
                    _receiver.currentXPos,
                    _receiver.currentYPos,
                    _receiver.width,
                    _receiver.height);

    BM_RE_drawSprite(_hipsterCat.currentBitmap,
                    NULL,
                    _hipsterCat.currentXPos,
                    _hipsterCat.currentYPos,
                    _hipsterCat.width,
                    _hipsterCat.height);

    BM_RE_drawSprite(BM_Animation_getCurrentFrame(&_propellerSpin),
                    NULL,
                    25,
                    11,
                    _propellerSpin.width,
                    _propellerSpin.height);

    if (_drawPhoneNoise)
        BM_RE_drawSprite(_phoneNoise.currentBitmap,
                        NULL,
                        _phoneNoise.currentXPos,
                        _phoneNoise.currentYPos,
                        _phoneNoise.width,
                        _phoneNoise.height);

    if (_drawMusicNotes)
        BM_RE_drawSprite(_musicNotes.currentBitmap,
                        _musicNotes.currentMask,
                        _musicNotes.currentXPos,
                        _musicNotes.currentYPos,
                        _musicNotes.width,
                        _musicNotes.height);
}


void BM_SceneDelegate_Phone_Flutter_processAudio(float32_t *inputBuffer, const size_t bufferSize)
{
    switch (_currentAudioState)
    {
        case BM_SCENE_PHONE_AUDIO_STATE_NORMAL:
        {
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL:
        {
            arm_mult_f32(inputBuffer, BM_FADE_IN_ENVELOPE, inputBuffer, bufferSize);
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_NORMAL;
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_NORMAL:
        {
            arm_mult_f32(inputBuffer, BM_FADE_OUT_ENVELOPE, inputBuffer, bufferSize);
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX;
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FX:
        {
            arm_scale_f32(inputBuffer, BM_SCENE_DELEGATE_PHONE_FLUTTER_AUDIO_GAIN, inputBuffer, bufferSize);
            MW_AFXUnit_Leslie_process(&_leslie, inputBuffer, bufferSize);
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX:
        {
            arm_scale_f32(inputBuffer, BM_SCENE_DELEGATE_PHONE_FLUTTER_AUDIO_GAIN, inputBuffer, bufferSize);
            MW_AFXUnit_Leslie_process(&_leslie, inputBuffer, bufferSize);
            arm_mult_f32(inputBuffer, BM_FADE_IN_ENVELOPE, inputBuffer, bufferSize);
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FX;
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX:
        {
            arm_scale_f32(inputBuffer, BM_SCENE_DELEGATE_PHONE_FLUTTER_AUDIO_GAIN, inputBuffer, bufferSize);
            MW_AFXUnit_Leslie_process(&_leslie, inputBuffer, bufferSize);
            arm_mult_f32(inputBuffer, BM_FADE_OUT_ENVELOPE, inputBuffer, bufferSize);
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;
            break;
        }
    }
}


void BM_SceneDelegate_Phone_Flutter_handleUserIO(const uint32_t buttonStates)
{
    uint32_t buttonEdgeState = 0;

    //  Detect user pressing button
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
        _AButtonPressed = 1;
    
    //  Detect user releasing button
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonEdgeState, BM_USERIO_RISING));
    if (buttonEdgeState)
        _AButtonPressed = 0;

    //  Handle D-Pad Up and Down button presses
    //  These buttons are mapped to the RPM value and propellor spin rate 
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_UP, &buttonEdgeState));
    if (!buttonEdgeState)
        _upButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_DOWN, &buttonEdgeState));
    if (!buttonEdgeState)
        _downButtonPressed = 1;
}

void BM_SceneDelegate_Phone_Flutter_setActive()
{
    if (_currentState != BM_SCENE_PHONE_EFFECT_STATE_ACTIVE)
        _currentState = BM_SCENE_PHONE_EFFECT_STATE_ACTIVE;

    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_ACTIVE;
}


void BM_SceneDelegate_Phone_Flutter_setInactive()
{
    if (_currentState != BM_SCENE_PHONE_EFFECT_STATE_INACTIVE)
    {
        _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;

        if ((_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX) || (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FX)) 
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX;

        _receiver.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_INACTIVE;
        _hipsterCat.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT;
    }
}


void BM_SceneDelegate_Phone_Flutter_reset()
{
    _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;
    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;
    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_HIPSTER_CAT_PHONE_INACTIVE;
    _AButtonPressed = 0;
    _upButtonPressed = 0;
    _downButtonPressed = 0;
}

BM_Scene_Phone_AudioState BM_SceneDelegate_Phone_Flutter_getAudioState()
{
    return _currentAudioState;
}
