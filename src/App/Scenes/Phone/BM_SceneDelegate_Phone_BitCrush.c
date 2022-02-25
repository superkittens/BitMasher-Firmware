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


#include "BM_SceneDelegate_Phone_BitCrush.h"

#define BM_SCENEDELEGATE_PHONE_BC_OUTPUT_GAIN   0.02f
#define BM_SCENEDELEGATE_PHONE_BC_RNG_MAP_MIN   0.f
#define BM_SCENEDELEGATE_PHONE_BC_RNG_MAP_MAX   100.f
#define BM_SCENEDELEGATE_PHONE_BC_RNG_MIN       50.f
#define BM_SCENEDELEGATE_PHONE_BC_RNG_MAX       90.f
#define BM_SCENEDELEGATE_PHONE_BC_RNG_STEP      0.67f

#define BM_SCENEDELEGATE_PHONE_BC_START_DOWNSAMPLE  10.f

#define BM_SCENEDELEGATE_PHONE_BC_FILTER_FC     5000.f

typedef enum
{
    BM_SCENEDELEGATE_PHONE_BC_NORMAL = 0,
    BM_SCENEDELEGATE_PHONE_BC_INIT
}BM_SceneDelegate_Phone_BC_State;

static BM_Scene_Phone_AudioState        _currentAudioState;
static BM_Scene_Phone_EffectState       _currentState;
static BM_SceneDelegate_Phone_BC_State  _delegateState;

static MW_AFXUnit_BitCrush  _bc;
static float32_t            _downSamplingRate;
static float32_t            _BC_DOWN_SAMPLING_RATE_MAX = 60.f;
static float32_t            _BC_DOWN_SAMPLING_RATE_MIN = 1.f;
static float32_t            _BC_DOWN_SAMPLING_RATE_DELTA = 1;
static float32_t            _BC_BIT_DEPTH = 2;
static float32_t            _BC_GAIN_DB = 40;

static MW_AFXUnit_Biquad    _filter;

static BM_Sprite    _spyCat;
static BM_Sprite    _receiver;
static BM_Sprite    _speech;

static float32_t    _MAX_CAT_YPOS = -15.f;

static int32_t      _AButtonPressed = 0;
static int32_t      _upButtonPressed = 0;
static int32_t      _downButtonPressed = 0;

static int32_t      _drawPhoneNoise = 0;

static uint8_t      _SQUARE_BITMAP[4] = {0xf, 0xf, 0xf, 0xf};
static float32_t    _rngThreshold;



static void BM_SceneDelegate_Phone_BC_resetCat()
{
    //  Initial cat sprite settings
    BM_Sprite_setPosition(&_spyCat, 1, -81);
    BM_Sprite_setVelocity(&_spyCat, 0, 4);
    BM_Sprite_setPosition(&_receiver, 45, -70);
    BM_Sprite_setVelocity(&_receiver, 0, 4);
    BM_Sprite_setPosition(&_speech, 32, -7);
    BM_Sprite_setVelocity(&_speech, 0, 4);
}





BM_Error BM_SceneDelegate_Phone_BC_init()
{
    int32_t success = MW_AFXUnit_BitCrush_init(&_bc, _downSamplingRate, _BC_BIT_DEPTH, _BC_GAIN_DB);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;

    // _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_OFF;
    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_NORMAL;
    _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;
    _delegateState = BM_SCENEDELEGATE_PHONE_BC_INIT;

    success = MW_AFXUnit_Biquad_init(&_filter, MW_BIQUAD_LPF, FS, BM_SCENEDELEGATE_PHONE_BC_FILTER_FC, 0.707, 0, NULL, 0);
    if (!success)
        while(1);

    BM_SceneDelegate_Phone_BC_resetCat();

    //  Initialise sprites
    _spyCat.currentBitmap = BM_ASSETS_TELEPHONE_SPY_CAT;
    _spyCat.currentMask = BM_ASSETS_TELEPHONE_SPY_CAT_MASK;
    _spyCat.width = BM_ASSETS_TELEPHONE_SPY_CAT_WIDTH;
    _spyCat.height = BM_ASSETS_TELEPHONE_SPY_CAT_HEIGHT;

    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_SPY_CAT_PHONE_INACTIVE;
    _receiver.currentMask = BM_ASSETS_TELEPHONE_SPY_CAT_PHONE_MASK;
    _receiver.width = BM_ASSETS_TELEPHONE_SPY_CAT_PHONE_INACTIVE_WIDTH;
    _receiver.height = BM_ASSETS_TELEPHONE_SPY_CAT_PHONE_INACTIVE_HEIGHT;

    _speech.currentBitmap = BM_ASSETS_TELEPHONE_SPY_CAT_SPEECH;
    _speech.currentMask = BM_ASSETS_TELEPHONE_SPY_CAT_SPEECH_MASK;
    _speech.width = BM_ASSETS_TELEPHONE_SPY_CAT_SPEECH_WIDTH;
    _speech.height = BM_ASSETS_TELEPHONE_SPY_CAT_SPEECH_HEIGHT;

    _downSamplingRate = BM_SCENEDELEGATE_PHONE_BC_START_DOWNSAMPLE;
    _rngThreshold = BM_SCENEDELEGATE_PHONE_BC_RNG_MAX - (BM_SCENEDELEGATE_PHONE_BC_RNG_STEP * _downSamplingRate);

    BM_TRNG_enableRNG(1);

    return BM_NO_ERR;
}


void BM_SceneDelegate_Phone_BC_update()
{
    switch (_delegateState)
    {
    //  Lower the cat down from the top of the screen
    case BM_SCENEDELEGATE_PHONE_BC_INIT:
    {
        BM_Sprite_update(&_spyCat);
        BM_Sprite_update(&_receiver);
        BM_Sprite_update(&_speech);

        //  If the cat is fully lowered, stop movement and change state
        if (_spyCat.currentYPos > _MAX_CAT_YPOS)
        {
            BM_Sprite_setVelocity(&_spyCat, 0, 0);
            BM_Sprite_setVelocity(&_receiver, 0, 0);
            BM_Sprite_setVelocity(&_speech, 0, 0);
            _delegateState = BM_SCENEDELEGATE_PHONE_BC_NORMAL;
        }

        break;
    }
    }

    switch (_currentState)
    {
    case BM_SCENE_PHONE_EFFECT_STATE_ACTIVE:
    {
        if (_AButtonPressed)
        {
            if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_NORMAL)
                _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_NORMAL;

            _spyCat.currentBitmap = BM_ASSETS_TELEPHONE_SPY_CAT_TALK;
            _spyCat.currentMask = BM_ASSETS_TELEPHONE_SPY_CAT_TALK_MASK;
            _drawPhoneNoise = 1;
        }
        else
        {
            if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FX || _currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX)
                _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX;

            _spyCat.currentBitmap = BM_ASSETS_TELEPHONE_SPY_CAT;
            _spyCat.currentMask = BM_ASSETS_TELEPHONE_SPY_CAT_MASK;
            _drawPhoneNoise = 0;
        }

        if (_upButtonPressed)
        {
            DECREMENT_CLIP(_downSamplingRate, _BC_DOWN_SAMPLING_RATE_DELTA, _BC_DOWN_SAMPLING_RATE_MIN);
            INCREMENT_CLIP(_rngThreshold, BM_SCENEDELEGATE_PHONE_BC_RNG_STEP, BM_SCENEDELEGATE_PHONE_BC_RNG_MAX);
            MW_AFXUnit_BitCrush_changeParameters(&_bc, _downSamplingRate, _BC_BIT_DEPTH, _BC_GAIN_DB);

            _upButtonPressed = 0;
        }

        if (_downButtonPressed)
        {
            INCREMENT_CLIP(_downSamplingRate, _BC_DOWN_SAMPLING_RATE_DELTA, _BC_DOWN_SAMPLING_RATE_MAX);
            DECREMENT_CLIP(_rngThreshold, BM_SCENEDELEGATE_PHONE_BC_RNG_STEP, BM_SCENEDELEGATE_PHONE_BC_RNG_MIN);
            MW_AFXUnit_BitCrush_changeParameters(&_bc, _downSamplingRate, _BC_BIT_DEPTH, _BC_GAIN_DB);

            _downButtonPressed = 0;
        }

        break;
    }

    case BM_SCENE_PHONE_EFFECT_STATE_INACTIVE:
    {
        _drawPhoneNoise = 0;
        break;
    }

    default:
        break;
    }
}


void BM_SceneDelegate_Phone_BC_draw()
{
    BM_RE_drawLightBackground();

    BM_RE_drawSprite(_spyCat.currentBitmap, 
                    _spyCat.currentMask,
                    _spyCat.currentXPos,
                    _spyCat.currentYPos,
                    _spyCat.width,
                    _spyCat.height
                    );

    //  Draw phone
    BM_RE_drawSprite(_receiver.currentBitmap,
                    _receiver.currentMask,
                    _receiver.currentXPos,
                    _receiver.currentYPos,
                    _receiver.width,
                    _receiver.height);

    //  Draw static noise
    for (int32_t row = 0; row < BM_DISPLAY_HEIGHT - 3; row += 4)
    {
        for (int32_t col = 0; col < 62; col += 4)
        {
            uint32_t number = rand();
            //  This for some reason causes glitches in the audio
            //  Timing is still okay though.  Is there something weird going on in the hardware?
            // BM_TRNG_getRandomNumber(&number);
            float32_t numberFloat = MW_AFXUnit_Utils_mapToRange((float32_t)number, 0.f, (float32_t)0x7fffffff, BM_SCENEDELEGATE_PHONE_BC_RNG_MAP_MIN, BM_SCENEDELEGATE_PHONE_BC_RNG_MAP_MAX);

            if (numberFloat > _rngThreshold)
                BM_RE_drawSprite(_SQUARE_BITMAP, NULL, col, row, 4, 4);
        }
    }
    
    BM_RE_drawSprite(BM_ASSETS_TELEPHONE_SPY_BKGND,
                    BM_ASSETS_TELEPHONE_SPY_BKGND_MASK,
                    0,
                    0,
                    BM_DISPLAY_WIDTH,
                    BM_DISPLAY_HEIGHT);


    if (_drawPhoneNoise)
        BM_RE_drawSprite(_speech.currentBitmap,
                        _speech.currentMask,
                        _speech.currentXPos,
                        _speech.currentYPos,
                        _speech.width,
                        _speech.height);
}


void BM_SceneDelegate_Phone_BC_processAudio(float32_t *inputBuffer, const size_t bufferSize)
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
            MW_AFXUnit_BitCrush_process(&_bc, inputBuffer, bufferSize);
            MW_AFXUnit_Biquad_process(&_filter, inputBuffer, bufferSize);
            arm_scale_f32(inputBuffer, BM_SCENEDELEGATE_PHONE_BC_OUTPUT_GAIN, inputBuffer, bufferSize);
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX:
        {
            MW_AFXUnit_BitCrush_process(&_bc, inputBuffer, bufferSize);
            MW_AFXUnit_Biquad_process(&_filter, inputBuffer, bufferSize);
            arm_scale_f32(inputBuffer, BM_SCENEDELEGATE_PHONE_BC_OUTPUT_GAIN, inputBuffer, bufferSize);
            arm_mult_f32(inputBuffer, BM_FADE_IN_ENVELOPE, inputBuffer, bufferSize);

            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FX;
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX:
        {
            MW_AFXUnit_BitCrush_process(&_bc, inputBuffer, bufferSize);
            MW_AFXUnit_Biquad_process(&_filter, inputBuffer, bufferSize);
            arm_scale_f32(inputBuffer, BM_SCENEDELEGATE_PHONE_BC_OUTPUT_GAIN, inputBuffer, bufferSize);
            arm_mult_f32(inputBuffer, BM_FADE_OUT_ENVELOPE, inputBuffer, bufferSize);

            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;
            break;
        }
    }
}


void BM_SceneDelegate_Phone_BC_handleUserIO(const uint32_t buttonStates)
{
    uint32_t buttonState = 0;
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_A, &buttonState));
    if (!buttonState)
        _AButtonPressed = 1;
    else
        _AButtonPressed = 0;

    //  Handle D-Pad Up and Down button presses
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_UP, &buttonState));
    if (!buttonState)
        _upButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_DOWN, &buttonState));
    if (!buttonState)
        _downButtonPressed = 1;
}


void BM_SceneDelegate_Phone_BC_setActive()
{
    if (_currentState != BM_SCENE_PHONE_EFFECT_STATE_ACTIVE)
        _currentState = BM_SCENE_PHONE_EFFECT_STATE_ACTIVE;
    
    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_SPY_CAT_PHONE_ACTIVE;
}


void BM_SceneDelegate_Phone_BC_setInactive()
{
    if (_currentState != BM_SCENE_PHONE_EFFECT_STATE_INACTIVE)
    {
        _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;

        if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FX || BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL)
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX;

        _receiver.currentBitmap = BM_ASSETS_TELEPHONE_SPY_CAT_PHONE_INACTIVE;
    }
}


void BM_SceneDelegate_Phone_BC_reset()
{
    _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;
    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;
    _delegateState = BM_SCENEDELEGATE_PHONE_BC_INIT;
    _AButtonPressed = 0;
    _upButtonPressed = 0;
    _downButtonPressed = 0;

    BM_SceneDelegate_Phone_BC_resetCat();
}


BM_Scene_Phone_AudioState BM_SceneDelegate_Phone_BC_getAudioState()
{
    return _currentAudioState;
}
