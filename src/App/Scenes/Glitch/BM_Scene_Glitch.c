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


#include "BM_Scene_Glitch.h"

#define BM_SCENE_GLITCH_NUM_ELEMENTS 1
#define BM_SCENE_GLITCH_CURSOR_VELOCITY 4

#define BM_SCENE_GLITCH_MAX_GRAIN_SIZE_SEC 0.165

typedef enum
{
    BM_SCENE_GLITCH_STATE_NORMAL = 0,
    BM_SCENE_GLITCH_STATE_GLITCH
}BM_Scene_Glitch_State;

typedef enum
{
    BM_SCENE_GLITCH_SCREEN_MAIN_SCREEN = 0,
    BM_SCENE_GLITCH_SCREEN_HELP,
    BM_SCENE_GLITCH_SCREEN_CREDITS
}BM_Scene_Glitch_Screen;


extern float32_t largeBuffer[];
MW_AFXUnit_Granular _granular;
float32_t           _grainLength = 0.05;
float32_t           _timeToChangeGrain = 0.07;

static uint8_t _glitchedFrame[(BM_DISPLAY_WIDTH / 8 ) * BM_DISPLAY_HEIGHT];

static int32_t _AButtonHeld = 0;
static int32_t _AButtonPrevState = 0;

static BM_Scene_Glitch_Screen  _currentScreen = BM_SCENE_GLITCH_SCREEN_MAIN_SCREEN;
static BM_Scene_Glitch_State   _currentState = BM_SCENE_GLITCH_STATE_NORMAL;

static BM_Scene_Glitch_Element _sceneElements[BM_SCENE_GLITCH_NUM_ELEMENTS];
static BM_Sprite               _cursor;




//  =====================================================================================   //
//  =====================================================================================   //



static BM_Error BM_Scene_Glitch_initElements()
{
    //  Initialize cursor
    _cursor.currentBitmap = BM_ASSETS_GLITCH_CURSOR_NORMAL;
    _cursor.currentMask = BM_ASSETS_GLITCH_CURSOR_NORMAL_MASK;
    _cursor.currentXPos = 64;
    _cursor.currentYPos = 70;
    _cursor.dx = 0;
    _cursor.dy = 0;
    _cursor.width = BM_ASSETS_GLITCH_CURSOR_NORMAL_WIDTH;
    _cursor.height = BM_ASSETS_GLITCH_CURSOR_NORMAL_HEIGHT;

    BM_Error err = BM_SceneDelegate_Glitch_Window_init(&_sceneElements[0]);
    if (err != BM_NO_ERR) return err;


    return BM_NO_ERR;
}


static void BM_Scene_Glitch_checkCursorBounds()
{
    if (_cursor.currentXPos < 0)
        _cursor.currentXPos = 0;

    if (_cursor.currentXPos + _cursor.width >= BM_DISPLAY_WIDTH)
        _cursor.currentXPos = BM_DISPLAY_WIDTH - _cursor.width;

    if (_cursor.currentYPos < 0)
        _cursor.currentYPos = 0;

    //  Allow the cursor to dip a bit below the bottom of the screen to let the user click the "start" button
    if (_cursor.currentYPos >= BM_DISPLAY_HEIGHT - _cursor.height + 20)
        _cursor.currentYPos = BM_DISPLAY_HEIGHT - _cursor.height + 20;
}


static int32_t BM_Scene_Glitch_cursorWithinElement(const BM_Sprite *element)
{
    if (element == NULL)
        return 0;

    if ((_cursor.currentXPos >= element->currentXPos) && (_cursor.currentXPos <= element->currentXPos + element->width)
        && (_cursor.currentYPos >= element->currentYPos) && (_cursor.currentYPos <= element->currentYPos + element->height))
        return 1;

    return 0;
}


static void BM_Scene_Glitch_cursorInteractionCheck()
{
    for (int32_t i = 0; i < BM_SCENE_GLITCH_NUM_ELEMENTS; ++i)
    {
        BM_Scene_Glitch_Element *e = &_sceneElements[i];
        if (e->isActive)
        {
            if (BM_Scene_Glitch_cursorWithinElement(&e->sprite))
            {
                //  If the cursor is within the window, change the cursor
                if (i == 0)
                {
                    _cursor.currentBitmap = BM_ASSETS_GLITCH_CURSOR_DRAG;
                    _cursor.currentMask = BM_ASSETS_GLITCH_CURSOR_DRAG_MASK;
                    _cursor.width = BM_ASSETS_GLITCH_CURSOR_DRAG_WIDTH;
                    _cursor.height = BM_ASSETS_GLITCH_CURSOR_DRAG_HEIGHT;
                }

                //  Check to see if the user has clicked or dragged an element
                if (_AButtonHeld)
                {
                    //  Drag = Cursor moving while A pressed
                    if ((_cursor.currentXPos != _cursor.prevXPos) || (_cursor.currentYPos != _cursor.prevYPos))
                    {
                        e->dragged = 1;

                        if (e->onDragged != NULL)
                            e->onDragged(_cursor.dx, _cursor.dy);
                    }

                    //  Check for click
                    //  Click = Cursor NOT moving while A Pressed
                    if (!_AButtonPrevState && (e->sprite.currentXPos == e->sprite.prevXPos) && (e->sprite.currentYPos == e->sprite.prevYPos))
                    {
                        e->clicked = 1;

                        if (e->onClicked != NULL)
                            e->onClicked();
                    }
                }

                else
                {
                    if (e->clicked || e->dragged)
                    {
                        e->clicked = 0;
                        e->dragged = 0;
                        e->onRelease();
                    }
                }
            }
            else
            {
                _cursor.currentBitmap = BM_ASSETS_GLITCH_CURSOR_NORMAL;
                _cursor.currentMask = BM_ASSETS_GLITCH_CURSOR_NORMAL_MASK;
                _cursor.width = BM_ASSETS_GLITCH_CURSOR_NORMAL_WIDTH;
                _cursor.height = BM_ASSETS_GLITCH_CURSOR_NORMAL_HEIGHT;
            }
        }
    }
}


static void BM_Scene_Glitch_mapWindowToGranularParams()
{
    _grainLength = (_sceneElements[0].sprite.currentXPos / ((float32_t)BM_DISPLAY_WIDTH - _sceneElements[0].sprite.width - 1) * BM_SCENE_GLITCH_MAX_GRAIN_SIZE_SEC) + 0.03;
    _timeToChangeGrain = (_sceneElements[0].sprite.currentYPos / ((float32_t)BM_DISPLAY_HEIGHT) - _sceneElements[0].sprite.height - 1) * 0.3f;
}


//  =====================================================================================   //
//  =====================================================================================   //



BM_Error BM_Scene_Glitch_init(BM_Scene *scene)
{
    if (scene == NULL) return BM_NULLPTR;

    int32_t err = MW_AFXUnit_Granular_init(&_granular, largeBuffer, 32000, 0.05, 0.05, FS);
    if (!err)
        return BM_AFXUNIT_INIT_FAIL;

    BM_Error elementError = BM_Scene_Glitch_initElements();
    if (elementError != BM_NO_ERR)
        return elementError;

    scene->draw = &BM_Scene_Glitch_draw;
    scene->handleUserIO = &BM_Scene_Glitch_handleUserIO;
    scene->processAudio = &BM_Scene_Glitch_processAudio;
    scene->reset = &BM_Scene_Glitch_reset;
    scene->update = &BM_Scene_Glitch_update;
    scene->iconBitmap = BM_ASSETS_GLITCH_ICON;
    scene->sceneName = "Glitch";

    memcpy(_glitchedFrame, BM_ASSETS_GLITCH_BKGND, (BM_DISPLAY_WIDTH / 8) * BM_DISPLAY_HEIGHT);
    
    return BM_NO_ERR;
}


void BM_Scene_Glitch_reset()
{
    //  Reset position of all elements
    BM_Scene_Glitch_initElements();
    BM_SceneDelegate_Glitch_Window_init(&_sceneElements[0]);
    arm_fill_f32(0, largeBuffer, AUDIO_BUFFER_SIZE);

    memcpy(_glitchedFrame, BM_ASSETS_GLITCH_BKGND, (BM_DISPLAY_WIDTH / 8) * BM_DISPLAY_HEIGHT);

    _currentState = BM_SCENE_GLITCH_STATE_NORMAL;
    _currentScreen = BM_SCENE_GLITCH_SCREEN_MAIN_SCREEN;
}


void BM_Scene_Glitch_update(void *data)
{
    switch (_currentScreen)
    {
        case BM_SCENE_GLITCH_SCREEN_MAIN_SCREEN:
        {
            BM_Sprite_update(&_cursor);
            BM_Scene_Glitch_checkCursorBounds();
            BM_Scene_Glitch_cursorInteractionCheck();

            //  Check to see if the window is dragged
            if (_sceneElements[0].dragged)
                _currentState = BM_SCENE_GLITCH_STATE_GLITCH;

            if (_currentState == BM_SCENE_GLITCH_STATE_GLITCH)
            {
                BM_Scene_Glitch_mapWindowToGranularParams();
                MW_AFXUnit_Granular_changeGrainSize(&_granular, _grainLength);
                MW_AFXUnit_Granular_changeTimeToChangeGrain(&_granular, _timeToChangeGrain);
            }

            break;
        }

        default:
            break;
    }
}


void BM_Scene_Glitch_draw()
{
    switch (_currentScreen)
    {
        case BM_SCENE_GLITCH_SCREEN_MAIN_SCREEN:
        {
            if (_currentState == BM_SCENE_GLITCH_STATE_NORMAL)
                BM_RE_drawBackground(BM_ASSETS_GLITCH_BKGND);

            if (_currentState == BM_SCENE_GLITCH_STATE_GLITCH)
                BM_RE_drawBackground(_glitchedFrame);

            for (int32_t i = 0; i < BM_SCENE_GLITCH_NUM_ELEMENTS; ++i)
            {
                BM_RE_drawSprite(_sceneElements[i].sprite.currentBitmap,
                                _sceneElements[i].sprite.currentMask,
                                _sceneElements[i].sprite.currentXPos,
                                _sceneElements[i].sprite.currentYPos,
                                _sceneElements[i].sprite.width,
                                _sceneElements[i].sprite.height);
            }

            //  To make the screen look 'glitched' copy the current frame buffer so that we can use
            //  it for the next update cycle's background
            if (_currentState == BM_SCENE_GLITCH_STATE_GLITCH)
                memcpy(_glitchedFrame, BM_RE_getFrameBuffer(), (BM_DISPLAY_WIDTH / 8) * BM_DISPLAY_HEIGHT);

            BM_RE_drawSprite(_cursor.currentBitmap,
                            _cursor.currentMask,
                            _cursor.currentXPos,
                            _cursor.currentYPos,
                            _cursor.width,
                            _cursor.height);

            break;
        }

        default:
            break;
    }
}


void BM_Scene_Glitch_handleUserIO(const uint32_t buttonStates)
{
    uint32_t buttonState = 0;
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_A, &buttonState));
    if (!buttonState)
    {
        _AButtonPrevState = _AButtonHeld;
        _AButtonHeld = 1;
    }
    else
    {
        _AButtonPrevState = _AButtonHeld;
        _AButtonHeld = 0;
    }

    buttonState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_B, &buttonState, BM_USERIO_FALLING));
    if (buttonState)
    {
        _currentState = BM_SCENE_GLITCH_STATE_NORMAL;
        memcpy(_glitchedFrame, BM_ASSETS_GLITCH_BKGND, (BM_DISPLAY_WIDTH / 8) * BM_DISPLAY_HEIGHT);
    }

    //  Handle D-Pad presses
    uint32_t leftButtonEdgeState = 0;
    uint32_t rightButtonEdgeState = 0;
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_LEFT, &leftButtonEdgeState));
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_RIGHT, &rightButtonEdgeState));

    if (!leftButtonEdgeState)
        BM_Sprite_setVelocity(&_cursor, -BM_SCENE_GLITCH_CURSOR_VELOCITY, _cursor.dy);
    
    if (!rightButtonEdgeState)
        BM_Sprite_setVelocity(&_cursor, BM_SCENE_GLITCH_CURSOR_VELOCITY, _cursor.dy);

    if (leftButtonEdgeState && rightButtonEdgeState)
        BM_Sprite_setVelocity(&_cursor, 0, _cursor.dy);


    uint32_t upButtonEdgeState = 0;
    uint32_t downButtonEdgeState = 0;
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_UP, &upButtonEdgeState));
    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_DOWN, &downButtonEdgeState));

    if (!upButtonEdgeState)
        BM_Sprite_setVelocity(&_cursor, _cursor.dx, -BM_SCENE_GLITCH_CURSOR_VELOCITY);

    if (!downButtonEdgeState)
        BM_Sprite_setVelocity(&_cursor, _cursor.dx, BM_SCENE_GLITCH_CURSOR_VELOCITY);

    if (upButtonEdgeState && downButtonEdgeState)
        BM_Sprite_setVelocity(&_cursor, _cursor.dx, 0);
}


void BM_Scene_Glitch_processAudio(float32_t *inputBuffer, const size_t bufferSize)
{
    if (_currentState == BM_SCENE_GLITCH_STATE_GLITCH)
    {
        MW_AFXUnit_Granular_process(&_granular, inputBuffer, bufferSize);

        //  Add some make up gain
        arm_scale_f32(inputBuffer, 1.8, inputBuffer, bufferSize);
    }
}
