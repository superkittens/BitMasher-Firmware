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


#include "BM_Scene_Clouds.h"

#define BM_SCENE_CLOUDS_MIN_LPF_MAKEUP_GAIN 1.f
#define BM_SCENE_CLOUDS_MAX_LPF_MAKEUP_GAIN 2.0f
#define BM_SCENE_CLOUDS_MIN_HPF_MAKEUP_GAIN 1.f
#define BM_SCENE_CLOUDS_MAX_HPF_MAKEUP_GAIN 1.5f

#define BM_SCENE_CLOUDS_FILTER_Q 2.f

#define BM_SCENE_CLOUDS_LPF_MIN_FC 200.f
#define BM_SCENE_CLOUDS_LPF_MAX_FC 10000.f
#define BM_SCENE_CLOUDS_HPF_MIN_FC 10.f
#define BM_SCENE_CLOUDS_HPF_MAX_FC 1000.f


//  Define each cloud's filter parameters
static MW_AFXUnit_SVFilter  _filter0, _filter1;
static float32_t            _FC_SCALE_VALUES[2];

static float32_t            _currentLPFMakeupGain = BM_SCENE_CLOUDS_MIN_LPF_MAKEUP_GAIN;
static float32_t            _currentHPFMakeupGain = BM_SCENE_CLOUDS_MIN_HPF_MAKEUP_GAIN;

static enum
{
    BM_SCENE_CLOUD_STATE_RUN = 0,
    BM_SCENE_CLOUD_STATE_LOCKOUT
};

static int32_t _currentSceneState = BM_SCENE_CLOUD_STATE_RUN;


// ------------------------------------------------------------------- //
// ------------------------------------------------------------------- //


static void BM_Scene_Clouds_collisionCheck()
{
    BM_Scene_Cloud_Sprite_PC *pc = BM_SceneDelegate_Clouds_PC_getPC();
    BM_Scene_Clouds_Cloud *clouds = BM_SceneDelegate_Clouds_Cloud_getClouds();

    #ifdef __DEBUG
    if (pc == NULL) while(1);
    if (clouds == NULL) while(1);
    #endif

    for (uint32_t i = 0; i < 2; ++i)
    {
        BM_DirectionPair collisionDirections = BM_CM_detectCollisions(&pc->spriteAttributes, &clouds[i].spriteAttributes);

        if (collisionDirections.first == BM_DIR_DOWN)
        {
            BM_SceneDelegate_Clouds_PC_landed();
            BM_SceneDelegate_Clouds_Cloud_pcContact(&clouds[i], &pc->spriteAttributes);
        }

        if (collisionDirections.first == BM_DIR_LEFT)
            pc->spriteAttributes.currentXPos = (int32_t)clouds[i].spriteAttributes.currentXPos + clouds[i].spriteAttributes.colliderBoundsOriginDx + clouds[i].spriteAttributes.colliderBoundsWidth - pc->spriteAttributes.colliderBoundsOriginDx;

        if (collisionDirections.first == BM_DIR_RIGHT)
            pc->spriteAttributes.currentXPos = (int32_t)clouds[i].spriteAttributes.currentXPos + clouds[i].spriteAttributes.colliderBoundsOriginDx - pc->spriteAttributes.colliderBoundsWidth - pc->spriteAttributes.colliderBoundsOriginDx;

        if (collisionDirections.first == BM_DIR_NONE)
        {
            if (clouds[i].pc != NULL)
            {
                BM_SceneDelegate_Clouds_Cloud_pcLeaveContact(&clouds[i]);
                BM_SceneDelegate_Clouds_PC_fall();
            }
        }
    }
}


static void BM_Scene_Clouds_mapAudioParameters()
{
    BM_Scene_Clouds_Cloud *clouds = BM_SceneDelegate_Clouds_Cloud_getClouds();

    #ifdef __DEBUG
    if (clouds == NULL) while(1);
    #endif

    //  Calculate LPF cutoff frequency based on the yPos of the cloud
    float32_t fcExponent = log2f(10000.f) - (_FC_SCALE_VALUES[0] * (clouds[0].spriteAttributes.currentYPos - (clouds[0].topYPos - clouds[0].lfoAmplitude)));
    float32_t fc = powf(2, fcExponent);

    MW_AFXUnit_SVFilter_changeParameters(&_filter0, MW_AFXUNIT_SVFILTER_LPF, FS, fc, BM_SCENE_CLOUDS_FILTER_Q);

    //  Calculate HPF cutoff frequency based on the yPos of the cloud
    fcExponent = (_FC_SCALE_VALUES[1] * (clouds[1].spriteAttributes.currentYPos - (clouds[1].topYPos - clouds[1].lfoAmplitude)));
    fc = powf(2, fcExponent);

    MW_AFXUnit_SVFilter_changeParameters(&_filter1, MW_AFXUNIT_SVFILTER_HPF, FS, fc, BM_SCENE_CLOUDS_FILTER_Q);

    //  Calculate makeup gains for each filter
    _currentLPFMakeupGain = MW_AFXUnit_Utils_mapToRange(clouds[0].spriteAttributes.currentYPos,
                                                        clouds[0].topYPos,
                                                        clouds[0].bottomYPos,
                                                        BM_SCENE_CLOUDS_MIN_LPF_MAKEUP_GAIN,
                                                        BM_SCENE_CLOUDS_MAX_LPF_MAKEUP_GAIN);

    _currentHPFMakeupGain = MW_AFXUnit_Utils_mapToRange(clouds[1].spriteAttributes.currentYPos,
                                                        clouds[1].topYPos,
                                                        clouds[1].bottomYPos,
                                                        BM_SCENE_CLOUDS_MIN_HPF_MAKEUP_GAIN,
                                                        BM_SCENE_CLOUDS_MAX_HPF_MAKEUP_GAIN);
}


// ------------------------------------------------------------------- //
// ------------------------------------------------------------------- //



BM_Error BM_Scene_Clouds_init(BM_Scene *scene)
{
    if (scene == NULL)
        return BM_NULLPTR;

    BM_Error error;

    error = BM_SceneDelegate_Cloud_PC_init();
    if (error != BM_NO_ERR)
        return error;

    error = BM_SceneDelegate_Clouds_Cloud_init();
    if (error != BM_NO_ERR)
        return error;
    
    //  Initialize star twinkle
    BM_SceneDelegate_Twinkle_init();

    //  Initialize Audio Graph
    int32_t success = MW_AFXUnit_SVFilter_init(&_filter0, MW_AFXUNIT_SVFILTER_LPF, FS, BM_SCENE_CLOUDS_LPF_MAX_FC, BM_SCENE_CLOUDS_FILTER_Q);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;

    success = MW_AFXUnit_SVFilter_init(&_filter1, MW_AFXUNIT_SVFILTER_HPF, FS, BM_SCENE_CLOUDS_HPF_MIN_FC, BM_SCENE_CLOUDS_FILTER_Q);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;

    //  Calculate the scaling value (logarithmic) for the filter cutoff frequencies
    BM_Scene_Clouds_Cloud *clouds = BM_SceneDelegate_Clouds_Cloud_getClouds();
    if (clouds == NULL)
        return BM_NULLPTR;
        
    _FC_SCALE_VALUES[0] = (log2f(BM_SCENE_CLOUDS_LPF_MAX_FC) - log2f(BM_SCENE_CLOUDS_LPF_MIN_FC)) / ((float32_t)(clouds[0].bottomYPos - clouds[0].topYPos));
    _FC_SCALE_VALUES[1] = log2f(BM_SCENE_CLOUDS_HPF_MAX_FC)  / ((float32_t)(clouds[1].bottomYPos - clouds[1].topYPos));


    scene->update = &BM_Scene_Clouds_update;
    scene->draw = &BM_Scene_Clouds_draw;
    scene->processAudio = &BM_Scene_Clouds_processAudio;
    scene->handleUserIO = &BM_Scene_Clouds_handleUserIO;
    scene->reset = &BM_Scene_Clouds_reset;
    scene->iconBitmap = BM_ASSETS_CLOUDS_ICON;
    scene->sceneName = "Clouds";

    return BM_NO_ERR;
}


void BM_Scene_Clouds_reset()
{
    _currentSceneState = BM_SCENE_CLOUD_STATE_RUN;
    BM_SceneDelegate_Cloud_PC_init();
    BM_SceneDelegate_Clouds_Cloud_init();
}


void BM_Scene_Clouds_update(void *data)
{
    BM_Scene_Cloud_Sprite_PC *pc = BM_SceneDelegate_Clouds_PC_getPC();
    BM_Scene_Clouds_Cloud *clouds = BM_SceneDelegate_Clouds_Cloud_getClouds();

    #ifdef __DEBUG
    if (pc == NULL) while(1);
    if (clouds == NULL) while(1);
    #endif

    //  Check to see if the PC has fallen off screen
    if (pc->spriteAttributes.currentYPos >= BM_DISPLAY_HEIGHT && _currentSceneState != BM_SCENE_CLOUD_STATE_LOCKOUT)
    {
        BM_SceneDelegate_Clouds_PC_onOffScreen();
        
        //  Check to see if all the clouds are in their normal state
        //  If so, start the retrieval sequence
        if (clouds[0].currentState == BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL 
            && clouds[1].currentState == BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL)
            {
                BM_SceneDelegate_Clouds_Cloud_startRetrieval(&clouds[1], &pc->spriteAttributes);
                _currentSceneState = BM_SCENE_CLOUD_STATE_LOCKOUT;
            }
    }

    //  If the retrieval sequence is in progress, check to see if it is finished
    if (_currentSceneState == BM_SCENE_CLOUD_STATE_LOCKOUT)
    {
        if (clouds[1].currentState == BM_SCENE_CLOUDS_CLOUD_STATE_NORMAL)
            _currentSceneState = BM_SCENE_CLOUD_STATE_RUN;

        BM_SceneDelegate_Clouds_PC_update();
        BM_SceneDelegate_Clouds_Cloud_update();
    }

    else
    {
        BM_Scene_Clouds_collisionCheck();

        BM_SceneDelegate_Clouds_PC_update();
        BM_SceneDelegate_Clouds_Cloud_update();
    }

    BM_Scene_Clouds_mapAudioParameters();
    BM_SceneDelegate_Twinkle_update();
}


void BM_Scene_Clouds_draw()
{
    BM_RE_drawBackground(BM_ASSETS_CLOUDS_BKGNDS_NIGHT);
    BM_SceneDelegate_Twinkle_draw();

    BM_SceneDelegate_Clouds_PC_draw();
    BM_SceneDelegate_Clouds_Cloud_draw();
}


void BM_Scene_Clouds_processAudio(float32_t *inputBuffer, size_t bufferSize)
{
    BM_Scene_Clouds_Cloud *clouds = BM_SceneDelegate_Clouds_Cloud_getClouds();

    #ifdef __DEBUG
    if (clouds == NULL) while(1);
    #endif

    MW_AFXUnit_SVFilter_process(&_filter0, inputBuffer, bufferSize);
    arm_scale_f32(inputBuffer, _currentLPFMakeupGain, inputBuffer, bufferSize);

    MW_AFXUnit_SVFilter_process(&_filter1, inputBuffer, bufferSize);
    arm_scale_f32(inputBuffer, _currentHPFMakeupGain, inputBuffer, bufferSize);

}


void BM_Scene_Clouds_handleUserIO(const uint32_t buttonStates)
{
    if (_currentSceneState == BM_SCENE_CLOUD_STATE_RUN)
        BM_SceneDelegate_Clouds_PC_handleUserIO(buttonStates);
}
