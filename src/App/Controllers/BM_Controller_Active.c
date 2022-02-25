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


#include "BM_Controller_Active.h"

#define BM_CONTROLLER_ACTIVE_TIMEOUT_MIN  1


static BM_SystemState   *_currentSystemState;
static float32_t        _audioFloatBuffer[AUDIO_BUFFER_SIZE];
static BM_Scene         _scenes[BM_CONTROLLER_ACTIVE_NUM_SCENES];
static uint32_t         _numScenes = 0;
static uint32_t         _sceneIndex = 0;
static int32_t          _sceneChange = 0;


static void BM_Controller_Active_enterMenu()
{
  BM_Controller_Menu_Entry_Packet params;
  params.currentScene = _sceneIndex;
  params.currentSystemState = _currentSystemState;
  params.scenes = _scenes;
  params.newScene = -1;
  params.numScenes = BM_CONTROLLER_ACTIVE_NUM_SCENES;

  BM_Error err = BM_Controller_Menu_enter(&params);
  if (err != BM_NO_ERR)
    *_currentSystemState = BM_STATE_ERROR;

  if (params.newScene >= 0)
  {
    if (params.newScene < BM_CONTROLLER_ACTIVE_NUM_SCENES)
    {
      _sceneIndex = params.newScene;
      _sceneChange = 1;
    }
    else
      *_currentSystemState = BM_STATE_ERROR;
  }
}


/*
 *  Handle controller level IO inputs
 *  ie. Scene, background and system state changes
 *
 *  IO Inputs specific to a scene's internals is handled by its own handleUserIO() function
 *  pointed to by the sceneHandleUserIO function pointer
 */
static void BM_Controller_Active_handleUserIO()
{
  //  Enter into the menu (and also package some necessary information for the menu controller)
  uint32_t menuButtonEdge = 0;
  BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_MENU, &menuButtonEdge, BM_USERIO_FALLING));

  if (menuButtonEdge)
    BM_Controller_Active_enterMenu();

  uint32_t buttonStates = 0;
  BM_ASSERT(BM_UserIO_getAllButtonStates(&buttonStates));
  if (buttonStates != 0x7F)
    BM_RTCModule_resetCounter();
}


static void BM_Controller_Active_checkTimeout()
{
  const int32_t numMinutesSinceLastButtonPress = BM_RTCModule_getCounterValue();
  if (numMinutesSinceLastButtonPress >= BM_CONTROLLER_ACTIVE_TIMEOUT_MIN)
    *_currentSystemState = BM_STATE_SLEEP;
}


static int32_t BM_Controller_Active_mainLoop()
{
  while(*_currentSystemState == BM_STATE_ACTIVE)
  {
    BM_ServiceQueueItem item;
    BM_Error error = BM_ServiceQueue_getItemFromServiceQueue(&item);

    if (error == BM_NO_ERR)
    {
      switch(item.itemType)
      {
        case BM_SERVICE_UI:
        {
          BM_DebugServices_set(DBP0);
          BM_DebugServices_startPerformanceTimer(1);
          BM_UserIO_update();
          BM_Controller_Active_handleUserIO();

          //  TODO:  Set an appropriate time to sleep
          // BM_Controller_Active_checkTimeout();

          if (_sceneChange)
          {
            _sceneChange = 0;
            break;
          }

          _scenes[_sceneIndex].handleUserIO(0);
          _scenes[_sceneIndex].update(NULL);
          _scenes[_sceneIndex].draw();

          BM_RE_updateDisplay(true);

          BM_DebugServices_clear(DBP0);
          BM_DebugServices_startPerformanceTimer(0);

          int32_t time = 0;
          BM_DebugServices_getTimeElapsed(&time);
          if (time > 12000)
            BM_DebugServices_setAlertLED();

          break;
        }

        case BM_SERVICE_AUDIO:
        {
          BM_DebugServices_set(DBP1);

          BM_AE_bufferCurrentlyProcessing = 1;
          int16_t *data = item.data;

          for (int i = 0; i < AUDIO_BUFFER_SIZE; ++i)
            _audioFloatBuffer[i] = (float32_t)data[i] / 32768.f;

          _scenes[_sceneIndex].processAudio(_audioFloatBuffer, AUDIO_BUFFER_SIZE);

          for (int i = 0; i < AUDIO_BUFFER_SIZE; ++i)
            data[i] = (int16_t)(_audioFloatBuffer[i] * 32768.f);

          BM_AE_bufferCurrentlyProcessing = 0;
          BM_DebugServices_clear(DBP1);

          break;
        }

        default:
          return 0;
      }
    }
  }

  return 1;
}


/*
 * Initialize playable character (PC), scenes and backgrounds
 * Also initialize the graphics render and audio engine;
 */
int32_t BM_Controller_Active_init()
{
  BM_RE_init(FRAME_RATE);
  BM_AE_init(FS, 0);

  BM_Error error = BM_Controller_Menu_init();
  if (error != BM_NO_ERR) 
    while(1);

  return 1;
}

/*
 *  Adds a scene to the array of available scenes to play
 */
BM_Error BM_Controller_Active_registerScene(BM_Error (*sceneInitCallback)(BM_Scene* scene))
{
  if (sceneInitCallback == NULL)
    return BM_NULLPTR;

  if (_numScenes >= BM_CONTROLLER_ACTIVE_NUM_SCENES)
    return BM_CONTROLLER_MAX_SCENES_REACHED;

  BM_Error error = sceneInitCallback(&_scenes[_numScenes]);
  if (error == BM_NO_ERR)
    _numScenes++;

  return error;
}


/*
 * Start the graphics rendering and audio engines
 * Then enter the main sequence loop
 */
int32_t BM_Controller_Active_enter(BM_SystemState *systemState)
{
  _currentSystemState = systemState;

  BM_RE_startVSYNC();
  BM_AE_startAudioEngine();

  BM_Controller_Active_enterMenu();
  BM_Controller_Active_mainLoop();

  BM_AE_stopAudioEngine();
  BM_RE_stopRenderEngine();
  
  return 1;
}


