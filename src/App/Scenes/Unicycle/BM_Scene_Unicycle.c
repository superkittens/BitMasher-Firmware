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


#include "BM_Scene_Unicycle.h"

#define BM_SCENE_UNICYCLE_STOP_G            1.f
#define BM_SCENE_UNICYCLE_ACCEL_G           -1.f
#define BM_SCENE_UNICYCLE_FWD_G             0.f
#define BM_SCENE_UNICYCLE_REV_G             2.f
#define BM_SCENE_UNICYCLE_BRAKE_ACCEL_DG    0.05f
#define BM_SCENE_UNICYCLE_NORMAL_DG         0.05f

#define BM_SCENE_UNICYCLE_DOPPLER_BUFFER_SIZE   32000

static MW_AFXUnit_Doppler _doppler;

//  "g" values for doppler effect
static float32_t    _targetG;
static float32_t    _currentDg = 0.f;
static float32_t    _currentG = BM_SCENE_UNICYCLE_STOP_G;

static BM_Scene_Unicycle_Motion _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;
static BM_Scene_Unicycle_Motion _prevMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;

static int32_t      _buttonPressed = 0;

extern float32_t largeBuffer[];


static void BM_Scene_Unicycle_setDopplerTargets()
{
    switch (_currentMotion)
    {
        case BM_SCENE_UNICYCLE_MOTION_BRAKE:
        {
            _targetG = BM_SCENE_UNICYCLE_STOP_G;

            if (_currentG < BM_SCENE_UNICYCLE_STOP_G)
                _currentDg = BM_SCENE_UNICYCLE_BRAKE_ACCEL_DG;

            if (_currentG > BM_SCENE_UNICYCLE_STOP_G)
                _currentDg = -BM_SCENE_UNICYCLE_BRAKE_ACCEL_DG;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_ACCELERATE:
        {
            _targetG = BM_SCENE_UNICYCLE_ACCEL_G;
            _currentDg = -BM_SCENE_UNICYCLE_BRAKE_ACCEL_DG;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_FORWARD:
        {
            _targetG = BM_SCENE_UNICYCLE_FWD_G;

            if (_currentG < BM_SCENE_UNICYCLE_FWD_G)
                _currentDg = BM_SCENE_UNICYCLE_NORMAL_DG;
            
            if (_currentG > BM_SCENE_UNICYCLE_FWD_G)
                _currentDg = -BM_SCENE_UNICYCLE_NORMAL_DG ;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_REVERSE:
        {
            _targetG = BM_SCENE_UNICYCLE_REV_G;
            _currentDg = BM_SCENE_UNICYCLE_NORMAL_DG;

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_STOPPING:
        {
            _targetG = BM_SCENE_UNICYCLE_STOP_G;

            if (_currentG < BM_SCENE_UNICYCLE_STOP_G)
                _currentDg = BM_SCENE_UNICYCLE_NORMAL_DG;
            
            if (_currentG > BM_SCENE_UNICYCLE_STOP_G)
                _currentDg = -BM_SCENE_UNICYCLE_NORMAL_DG;

            break;
        }

        default:
            break;
    }
}


static void BM_Scene_Unicycle_updateG()
{
    _currentG += _currentDg;

    switch (_currentMotion)
    {
        case BM_SCENE_UNICYCLE_MOTION_BRAKE:
        case BM_SCENE_UNICYCLE_MOTION_STOPPING:
        {
            if ((_currentDg >= 0 && (_currentG >= BM_SCENE_UNICYCLE_STOP_G)) ||
                (_currentDg <= 0 && (_currentG <= BM_SCENE_UNICYCLE_STOP_G)))
            {
                _currentDg = 0;
                _currentG = BM_SCENE_UNICYCLE_STOP_G;
                _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;
            }

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_ACCELERATE:
        {
            if (_currentG <= BM_SCENE_UNICYCLE_ACCEL_G)
            {
                _currentDg = 0;
                _currentG = BM_SCENE_UNICYCLE_ACCEL_G;
            }

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_FORWARD:
        {
            if ((_currentDg > 0 && (_currentG >= BM_SCENE_UNICYCLE_FWD_G)) ||
                (_currentDg < 0 && (_currentG <= BM_SCENE_UNICYCLE_FWD_G)))
            {
                _currentDg = 0;
                _currentG = BM_SCENE_UNICYCLE_FWD_G;
            }

            break;
        }

        case BM_SCENE_UNICYCLE_MOTION_REVERSE:
        {
            if (_currentG >= BM_SCENE_UNICYCLE_REV_G)
            {
                _currentDg = 0;
                _currentG = BM_SCENE_UNICYCLE_REV_G;
            }

            break;
        }
    }
}


static void BM_Scene_Unicycle_setMotionTargets()
{
    float32_t targetG = 0.f;

    switch (_currentMotion)
    {
        case BM_SCENE_UNICYCLE_MOTION_FORWARD:
            targetG = BM_SCENE_UNICYCLE_FWD_G;
            break;

        case BM_SCENE_UNICYCLE_MOTION_REVERSE:
            targetG = BM_SCENE_UNICYCLE_REV_G;
            break;

        case BM_SCENE_UNICYCLE_MOTION_ACCELERATE:
            targetG = BM_SCENE_UNICYCLE_ACCEL_G;
            break;

        case BM_SCENE_UNICYCLE_MOTION_STOPPING:
        case BM_SCENE_UNICYCLE_MOTION_BRAKE:
            targetG = BM_SCENE_UNICYCLE_STOP_G;
            break;
    }

    if (_currentG != targetG)
    {
        float32_t numSteps = (1.f / _currentDg) * (_currentG - targetG);
        if (numSteps < 0)
            numSteps *= -1.f;

        BM_SceneDelegate_Unicycle_Buildings_setMotionTargets(numSteps);
        BM_SceneDelegate_Unicycle_Clouds_setMotionTargets(numSteps);
        BM_SceneDelegate_Unicycle_Cat_setMotionTargets(numSteps);
        BM_SceneDelegate_Unicycle_Road_setMotionTargets(numSteps);
    }
}



//  ================================================================================================    //
//  ================================================================================================    //



BM_Error BM_Scene_Unicycle_init(BM_Scene *scene)
{
    if (scene == NULL)
        return BM_NULLPTR;

    BM_Error err = BM_SceneDelegate_Unicycle_Cat_init();
    if (err != BM_NO_ERR)
        return err;

    err = BM_SceneDelegate_Unicycle_Clouds_init();
    if (err != BM_NO_ERR)
        return err;

    err = BM_SceneDelegate_Unicycle_Road_init();
    if (err != BM_NO_ERR)
        return err;

    err = BM_SceneDelegate_Unicycle_Buildings_init();
    if (err != BM_NO_ERR)
        return err;

    int32_t success = MW_AFXUnit_Doppler_init(&_doppler, largeBuffer, BM_SCENE_UNICYCLE_DOPPLER_BUFFER_SIZE, FS);
    if (!success)
        return BM_AFXUNIT_INIT_FAIL;


    scene->draw = &BM_Scene_Unicycle_draw;
    scene->update = &BM_Scene_Unicycle_update;
    scene->processAudio = &BM_Scene_Unicycle_processAudio;
    scene->handleUserIO = &BM_Scene_Unicycle_handleUserIO;
    scene->reset = &BM_Scene_Unicycle_reset;
    scene->iconBitmap = BM_ASSETS_UNICYCLE_ICON;
    scene->sceneName = "Unicycle";

    return BM_NO_ERR;
}


void BM_Scene_Unicycle_reset()
{
    MW_AFXUnit_Doppler_reset(&_doppler);
    
    _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPED;
    _currentDg = 0;
    _currentG = BM_SCENE_UNICYCLE_STOP_G;
    _targetG = BM_SCENE_UNICYCLE_STOP_G;

    BM_SceneDelegate_Unicycle_Buildings_reset();
    BM_SceneDelegate_Unicycle_Cat_reset();
    BM_SceneDelegate_Unicycle_Clouds_reset();
    BM_SceneDelegate_Unicycle_Road_reset();
}


void BM_Scene_Unicycle_update(void *data)
{
    if (_prevMotion != _currentMotion)
    {
        BM_SceneDelegate_Unicycle_Buildings_setMotionState(_currentMotion);
        BM_SceneDelegate_Unicycle_Cat_setMotionState(_currentMotion);
        BM_SceneDelegate_Unicycle_Clouds_setMotionState(_currentMotion);
        BM_SceneDelegate_Unicycle_Road_setMotionState(_currentMotion);

        BM_Scene_Unicycle_setMotionTargets();

        _prevMotion = _currentMotion;
    }

    BM_SceneDelegate_Unicycle_Buildings_update();
    BM_SceneDelegate_Unicycle_Cat_update();
    BM_SceneDelegate_Unicycle_Clouds_update();
    BM_SceneDelegate_Unicycle_Road_update();

    BM_Scene_Unicycle_updateG();
    MW_AFXUnit_Doppler_changeParameters(&_doppler, _currentG);
}


void BM_Scene_Unicycle_draw()
{
    BM_RE_drawBackground(BM_ASSETS_UNICYCLE_BKGND);

    BM_SceneDelegate_Unicycle_Buildings_draw();
    BM_SceneDelegate_Unicycle_Road_draw();
    BM_SceneDelegate_Unicycle_Clouds_draw();
    BM_SceneDelegate_Unicycle_Cat_draw();
}


void BM_Scene_Unicycle_processAudio(float32_t *inputBuffer, const size_t bufferSize)
{
    MW_AFXUnit_Doppler_process(&_doppler, inputBuffer, bufferSize);
}


void BM_Scene_Unicycle_handleUserIO(const uint32_t buttonStates)
{
    uint32_t states = 0;
    BM_ASSERT(BM_UserIO_getAllButtonEdgeStates(&states, BM_USERIO_FALLING));
    if (states)
    {
        _buttonPressed = 1;
    }

    else
    {
        BM_ASSERT(BM_UserIO_getAllButtonStates(&states))
        if (states == 0x7F)
            _buttonPressed = 0;
    }

    if (_buttonPressed)
    {
        //  The do-while loop exists because of how some button presses override all other button presses
        //  For instance, if the B button is pressed, no other button presses matter.  The cat MUST brake
        do
        {
            //  A and B buttons take highest priority.
            //  B overrides A
            uint32_t buttonEdgeState = 0;
            BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_B, &buttonEdgeState));
            if (!buttonEdgeState)
            {
                _currentMotion = BM_SCENE_UNICYCLE_MOTION_BRAKE;
                break;
            }

            BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_A, &buttonEdgeState));
            if (!buttonEdgeState)
            {
                _currentMotion = BM_SCENE_UNICYCLE_MOTION_ACCELERATE;
                break;
            }

            //  Now take care of the D-pad buttons
            BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_RIGHT, &buttonEdgeState));
            if (!buttonEdgeState)
            {
                _currentMotion = BM_SCENE_UNICYCLE_MOTION_FORWARD;
                break;
            }

            BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_LEFT, &buttonEdgeState));
            if (!buttonEdgeState)
            {
                _currentMotion = BM_SCENE_UNICYCLE_MOTION_REVERSE;
                break;
            }

            //  Check to see if the buttons that we care about (A, B, LEFT, RIGHT) are all high (released)
            //  We don't care about the UP, DOWN, MENU and UNASSIGNED so we mask those out
            if (((buttonEdgeState & 0x33) == 0x33) && _currentMotion != BM_SCENE_UNICYCLE_MOTION_STOPPED)
                _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPING;

        } while (0);
    }
    else
    {
        if (_currentMotion != BM_SCENE_UNICYCLE_MOTION_STOPPED)
                _currentMotion = BM_SCENE_UNICYCLE_MOTION_STOPPING;
    }

    BM_Scene_Unicycle_setDopplerTargets();   
}
