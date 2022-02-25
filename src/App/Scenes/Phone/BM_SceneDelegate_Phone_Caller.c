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


#include "BM_SceneDelegate_Phone_Caller.h"

#define BM_SCENE_DELEGATE_PHONE_CALLER_MIN_FILTER_FC    10.f
#define BM_SCENE_DELEGATE_PHONE_CALLER_MAX_FILTER_FC    10000.f
#define BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_DELTA     50.f
#define BM_SCENE_DELEGATE_PHONE_CALLER_LPF_START_FC     2500.f
#define BM_SCENE_DELEGATE_PHONE_CALLER_HPF_START_FC     510.f
#define BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_Q         6.f
#define BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_GAIN      1.f

#define BM_SCENE_DELEGATE_PHONE_CALLER_CAT_XPOS         85
#define BM_SCENE_DELEGATE_PHONE_CALLER_CAT_YPOS         80

static BM_Scene_Phone_AudioState _currentAudioState;
static BM_Scene_Phone_EffectState _currentState;
static MW_AFXUnit_SVFilter _lpFilter;
static MW_AFXUnit_SVFilter _hpFilter;
static float32_t _lpfFc = BM_SCENE_DELEGATE_PHONE_CALLER_LPF_START_FC;
static float32_t _hpfFc = BM_SCENE_DELEGATE_PHONE_CALLER_HPF_START_FC;
static float32_t _lpfQ = BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_Q;
static float32_t _hpfQ = BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_Q;

static int32_t      _AButtonPressed = 0;
static int32_t      _upButtonPressed = 0;
static int32_t      _downButtonPressed = 0;

static int32_t      _drawPhoneNoise = 0;
static int32_t      _drawMusicNotes = 0;

static BM_Sprite    _callerCat;
static BM_Sprite    _receiver;
static BM_Sprite    _phoneNoise;
static BM_Sprite    _musicNotes;
static BM_Sprite    _payphoneFace;
static BM_Animation _spotlightAnimation;

//  Indicate spotlight direction (fade in: 1, fade out: 0, nothing: -1)
static int32_t      _shineSpotlight;
static int32_t      _spotlightCounter;


BM_Error BM_SceneDelegate_Phone_Caller_init()
{
    int32_t success = MW_AFXUnit_SVFilter_init(&_lpFilter, MW_AFXUNIT_SVFILTER_LPF, FS, _lpfFc, _lpfQ);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;

    success = MW_AFXUnit_SVFilter_init(&_hpFilter, MW_AFXUNIT_SVFILTER_HPF, FS, _hpfFc, _hpfQ);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;


    //  Initialize all sprites
    _callerCat.currentBitmap = BM_ASSETS_CAT_FACE_LEFT;
    _callerCat.width = BM_ASSETS_CAT_FACE_LEFT_WIDTH;
    _callerCat.height = BM_ASSETS_CAT_FACE_LEFT_HEIGHT;
    _callerCat.currentXPos = BM_SCENE_DELEGATE_PHONE_CALLER_CAT_XPOS;
    _callerCat.currentYPos = BM_SCENE_DELEGATE_PHONE_CALLER_CAT_YPOS;

    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_CALLER_PHONE_ACTIVE;
    _receiver.currentMask = BM_ASSETS_TELEPHONE_CALLER_PHONE_MASK;
    _receiver.width = BM_ASSETS_TELEPHONE_CALLER_PHONE_ACTIVE_WIDTH;
    _receiver.height = BM_ASSETS_TELEPHONE_CALLER_PHONE_ACTIVE_HEIGHT;
    _receiver.currentXPos = 68;
    _receiver.currentYPos = 80;

    _phoneNoise.currentBitmap = BM_ASSETS_TELEPHONE_CALLER_PHONE_NOISE;
    _phoneNoise.currentMask = BM_ASSETS_TELEPHONE_CALLER_PHONE_NOISE_MASK;
    _phoneNoise.currentXPos = 78;
    _phoneNoise.currentYPos = 71;
    _phoneNoise.width = BM_ASSETS_TELEPHONE_CALLER_PHONE_NOISE_WIDTH;
    _phoneNoise.height = BM_ASSETS_TELEPHONE_CALLER_PHONE_NOISE_HEIGHT;

    _musicNotes.currentBitmap = BM_ASSETS_TELEPHONE_CALLER_MUSIC_NOTES;
    _musicNotes.currentMask = BM_ASSETS_TELEPHONE_CALLER_MUSIC_NOTES_MASK;
    _musicNotes.currentXPos = 78;
    _musicNotes.currentYPos = 70;
    _musicNotes.width = BM_ASSETS_TELEPHONE_CALLER_MUSIC_NOTES_WIDTH;
    _musicNotes.height = BM_ASSETS_TELEPHONE_CALLER_MUSIC_NOTES_HEIGHT;

    _payphoneFace.currentBitmap = BM_ASSETS_TELEPHONE_PAYPHONE_FACE;
    _payphoneFace.currentMask = BM_ASSETS_TELEPHONE_PAYPHONE_FACE_MASK;
    _payphoneFace.currentXPos = 91;
    _payphoneFace.currentYPos = 35;
    _payphoneFace.width = BM_ASSETS_TELEPHONE_PAYPHONE_FACE_WIDTH;
    _payphoneFace.height = BM_ASSETS_TELEPHONE_PAYPHONE_FACE_HEIGHT;

    _spotlightAnimation.frames = BM_ASSETS_TELEPHONE_SPOTLIGHT;
    _spotlightAnimation.frameMasks = BM_ASSETS_TELEPHONE_SPOTLIGHT_MASK;
    _spotlightAnimation.useMask = 1;
    _spotlightAnimation.numFrames = BM_ASSETS_TELEPHONE_SPOTLIGHT_NUM_FRAMES;
    _spotlightAnimation.width = BM_DISPLAY_WIDTH;
    _spotlightAnimation.height = BM_DISPLAY_HEIGHT;

    BM_Animation_init(&_spotlightAnimation, 0);

    _shineSpotlight = -1;
    _spotlightCounter = 0;

    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;
    _currentState = BM_SCENE_PHONE_EFFECT_STATE_ACTIVE;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Phone_Caller_update()
{
    switch (_currentState)
    {
        case BM_SCENE_PHONE_EFFECT_STATE_ACTIVE:
        {
            _drawPhoneNoise = 0;

            if (_AButtonPressed)
            {
                if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_NORMAL)
                    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_NORMAL;

                _callerCat.currentBitmap = BM_ASSETS_CAT_TALK;
                _callerCat.currentMask = BM_ASSETS_CAT_TALK_MASK;
                _drawMusicNotes = 1;
                _shineSpotlight = 1;
                BM_Animation_setPlaybackDirection(&_spotlightAnimation, 0);
            }
            else
            {
                if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX || _currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FX)
                    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX;

                _drawMusicNotes = 0;
                _callerCat.currentBitmap = BM_ASSETS_CAT_FACE_LEFT;
                _shineSpotlight = 0;
                BM_Animation_setPlaybackDirection(&_spotlightAnimation, 1);
            }

            //  Change cutoff frequency of up or down button is pressed
            if (_upButtonPressed)
            {
                INCREMENT_CLIP(_hpfFc, BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_DELTA, BM_SCENE_DELEGATE_PHONE_CALLER_MAX_FILTER_FC);
                MW_AFXUnit_SVFilter_changeParameters(&_hpFilter, MW_AFXUNIT_SVFILTER_HPF, FS, _hpfFc, BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_Q);
                _upButtonPressed = 0;
            }

            if (_downButtonPressed)
            {
                DECREMENT_CLIP(_hpfFc, BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_DELTA, BM_SCENE_DELEGATE_PHONE_CALLER_MIN_FILTER_FC);
                MW_AFXUnit_SVFilter_changeParameters(&_hpFilter, MW_AFXUNIT_SVFILTER_HPF, FS, _hpfFc, BM_SCENE_DELEGATE_PHONE_CALLER_FILTER_Q);
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

    //  Update spotlight frames if needed
    if (_shineSpotlight >= 0)
    {

        _spotlightCounter = 0;

        //  Advance spotlight forward one frame
        if (_shineSpotlight == 1)
        {
            if (BM_Animation_getCurrentFrameNumber(&_spotlightAnimation) < BM_ASSETS_TELEPHONE_SPOTLIGHT_NUM_FRAMES - 1)
                BM_Animation_update(&_spotlightAnimation);
        }
        //  Advance spotlight backward one frame
        if (_shineSpotlight == 0)
        {
            if (BM_Animation_getCurrentFrameNumber(&_spotlightAnimation) == 0)
                _shineSpotlight = -1;
            else
                BM_Animation_update(&_spotlightAnimation);
        }
    }
}


void BM_SceneDelegate_Phone_Caller_draw()
{
    if (_shineSpotlight >= 0)
        BM_RE_drawSprite(BM_Animation_getCurrentFrame(&_spotlightAnimation),
                        BM_Animation_getCurrentFrameMask(&_spotlightAnimation),
                        0,
                        0,
                        BM_DISPLAY_WIDTH,
                        BM_DISPLAY_HEIGHT);

    BM_RE_drawSprite(_callerCat.currentBitmap,
                        _callerCat.currentMask,
                        _callerCat.currentXPos,
                        _callerCat.currentYPos,
                        _callerCat.width,
                        _callerCat.height);

    BM_RE_drawSprite(_receiver.currentBitmap,
                    _receiver.currentMask,
                    _receiver.currentXPos,
                    _receiver.currentYPos,
                    _receiver.width,
                    _receiver.height);

    BM_RE_drawSprite(_payphoneFace.currentBitmap,
                    _payphoneFace.currentMask,
                    _payphoneFace.currentXPos,
                    _payphoneFace.currentYPos,
                    _payphoneFace.width,
                    _payphoneFace.height);

    if (_drawPhoneNoise)
        BM_RE_drawSprite(_phoneNoise.currentBitmap,
                        _phoneNoise.currentMask,
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


void BM_SceneDelegate_Phone_Caller_processAudio(float32_t *inputBuffer, const size_t bufferSize) 
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
            MW_AFXUnit_SVFilter_process(&_lpFilter, inputBuffer, bufferSize);
            MW_AFXUnit_SVFilter_process(&_hpFilter, inputBuffer, bufferSize);
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX:
        {
            MW_AFXUnit_SVFilter_process(&_lpFilter, inputBuffer, bufferSize);
            MW_AFXUnit_SVFilter_process(&_hpFilter, inputBuffer, bufferSize);
            arm_mult_f32(inputBuffer, BM_FADE_IN_ENVELOPE, inputBuffer, bufferSize);
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FX;
            break;
        }

        case BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX:
        {
            MW_AFXUnit_SVFilter_process(&_lpFilter, inputBuffer, bufferSize);
            MW_AFXUnit_SVFilter_process(&_hpFilter, inputBuffer, bufferSize);
            arm_mult_f32(inputBuffer, BM_FADE_OUT_ENVELOPE, inputBuffer, bufferSize);
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;
            break;
        }
    }
}


void BM_SceneDelegate_Phone_Caller_handleUserIO(const uint32_t buttonStates)
{
    uint32_t buttonEdgeState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
        _AButtonPressed = 1;
    
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonEdgeState, BM_USERIO_RISING));
    if (buttonEdgeState)
        _AButtonPressed = 0;

    //  Up and down buttons change fc in the BPF
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_UP, &buttonEdgeState));
    if (!buttonEdgeState)
        _upButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_DOWN, &buttonEdgeState));
    if (!buttonEdgeState)
        _downButtonPressed = 1;
}


void BM_SceneDelegate_Phone_Caller_setActive()
{
    if (_currentState != BM_SCENE_PHONE_EFFECT_STATE_ACTIVE)
        _currentState = BM_SCENE_PHONE_EFFECT_STATE_ACTIVE;
    
    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_CALLER_PHONE_ACTIVE;
}


void BM_SceneDelegate_Phone_Caller_setInactive()
{
    if (_currentState != BM_SCENE_PHONE_EFFECT_STATE_INACTIVE)
    {
        _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;

        if (_currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_FX || _currentAudioState == BM_SCENE_PHONE_AUDIO_STATE_FX)
            _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_OUT_FX;
        
        _receiver.currentBitmap = BM_ASSETS_TELEPHONE_CALLER_PHONE_INACTIVE;
        _callerCat.currentBitmap = BM_ASSETS_CAT_FACE_LEFT;
        
        _shineSpotlight = 0;
    }
}


void BM_SceneDelegate_Phone_Caller_reset()
{
    _currentState = BM_SCENE_PHONE_EFFECT_STATE_INACTIVE;
    _currentAudioState = BM_SCENE_PHONE_AUDIO_STATE_FADE_IN_NORMAL;
    _receiver.currentBitmap = BM_ASSETS_TELEPHONE_CALLER_PHONE_INACTIVE;
    _AButtonPressed = 0;
    _upButtonPressed = 0;
    _downButtonPressed = 0;
    _shineSpotlight = -1;
    _spotlightCounter = 0;

    BM_Animation_reset(&_spotlightAnimation);
}


BM_Scene_Phone_AudioState BM_SceneDelegate_Phone_Caller_getAudioState()
{
    return _currentAudioState;
}
