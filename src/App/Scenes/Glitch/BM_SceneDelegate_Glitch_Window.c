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


#include "BM_SceneDelegate_Glitch_Window.h"

static BM_Scene_Glitch_Element *_window;

static void BM_SceneDelegate_Glitch_Window_checkWindowBounds()
{
    if (_window->sprite.currentXPos < 0)
        _window->sprite.currentXPos = 0;

    if (_window->sprite.currentXPos >= BM_DISPLAY_WIDTH - _window->sprite.width)
        _window->sprite.currentXPos = BM_DISPLAY_WIDTH - _window->sprite.width;

    if (_window->sprite.currentYPos < 0)
        _window->sprite.currentYPos = 0;

    if (_window->sprite.currentYPos >= BM_DISPLAY_HEIGHT - _window->sprite.height - 14)
        _window->sprite.currentYPos = BM_DISPLAY_HEIGHT - _window->sprite.height - 14;
}


BM_Error BM_SceneDelegate_Glitch_Window_init(BM_Scene_Glitch_Element *e)
{
    if (e == NULL) return BM_NULLPTR;

    _window = e;

    //  Initialize window
    _window->sprite.currentBitmap = BM_ASSETS_GLITCH_WINDOW;
    _window->sprite.currentMask = NULL;
    _window->sprite.currentXPos = 20;
    _window->sprite.currentYPos = 20;
    _window->sprite.dx = 0;
    _window->sprite.dy = 0;
    _window->sprite.width = BM_ASSETS_GLITCH_WINDOW_WIDTH; 
    _window->sprite.height = BM_ASSETS_GLITCH_WINDOW_HEIGHT;
    _window->onClicked = NULL;
    _window->onDragged = &BM_Scene_Glitch_onWindowDragged;
    _window->onRelease = &BM_Scene_Glitch_onWindowRelease;
    _window->isActive = 1;

    return BM_NO_ERR;
}


void BM_SceneDelegate_Glitch_Window_update()
{

}


//  Element-specific callback functions
void BM_Scene_Glitch_onWindowDragged(float32_t dx, float32_t dy)
{
    BM_Sprite_setPosition(  &_window->sprite, 
                            _window->sprite.currentXPos + dx,
                            _window->sprite.currentYPos + dy);

    BM_SceneDelegate_Glitch_Window_checkWindowBounds();
}


void BM_Scene_Glitch_onWindowRelease()
{
    BM_Sprite_setVelocity(&_window->sprite, 0, 0);
}