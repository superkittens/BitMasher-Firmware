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


#include "BM_Controller_Menu.h"

#define BM_SCENE_MENU_CAT_FRAME_CHANGE_VALUE    20
#define BM_SCENE_MENU_NOTE_CHANGE_VALUE_MIN     30
#define BM_SCENE_MENU_NOTE_CHANGE_VALUE_MAX     45
#define BM_SCENE_MENU_NUM_NOTES                 4

#define BM_SCENE_MENU_NUM_SUB_SCENES            2

#define BM_SCENE_MENU_SLEEP_INDEX               (_sceneData->entryPacket->numScenes + 1)
#define BM_SCENE_MENU_SETTINGS_INDEX            (_sceneData->entryPacket->numScenes)

static int32_t  _AButtonPressed = 0;
static int32_t  _BButtonPressed = 0;
static int32_t  _menuButtonPressed = 0;
static int32_t  _leftButtonPressed = 0;
static int32_t  _rightButtonPressed = 0;

static int32_t  _sceneSelection = 0;
static int32_t  _sleepSettingSelect = 0;

static int32_t  _firstCycle = 1;
static int32_t  _sceneChanged = 0;

static BM_Animation _cat;
static int32_t      _catFrameCounter = 0;

static BM_Sprite    _gramophone;

static int32_t      _musicNoteIndex = 0;
static int32_t      _musicNoteCounter = 0;
static int32_t      _musicNoteTimeout = BM_SCENE_MENU_NOTE_CHANGE_VALUE_MIN;
static BM_Sprite    _musicNotes[BM_SCENE_MENU_NUM_NOTES];

//  Menu frame, icons and arrows
static BM_Sprite    _leftArrow;
static BM_Sprite    _rightArrow;
static BM_Sprite    _iconFrame;
static BM_Sprite    _icon;

static BM_Controller_Menu_Data *_sceneData;


static uint16_t rs_rand(uint16_t max, uint16_t power_mask) { 
    uint16_t res = UINT16_C(0); 
    
    do { 
        res = (rand() & power_mask); 
    } while(res > max); 
    
    return res; 
}


BM_Error BM_Scene_Menu_init(BM_Scene *scene)
{
    scene->update = &BM_Scene_Menu_update;
    scene->draw = &BM_Scene_Menu_draw;
    scene->processAudio = &BM_Scene_Menu_processAudio;
    scene->handleUserIO = &BM_Scene_Menu_handleUserIO;
    scene->reset = &BM_Scene_Menu_reset;

    _cat.width = BM_ASSETS_MENU_CAT_WIDTH;
    _cat.height = BM_ASSETS_MENU_CAT_HEIGHT;
    _cat.frames = BM_ASSETS_MENU_CAT;
    _cat.useMask = 0;
    _cat.numFrames = BM_ASSETS_MENU_CAT_NUM_FRAMES;

    BM_Error err = BM_Animation_init(&_cat, 0);
    if (err != BM_NO_ERR)
        return err;

    _gramophone.width = BM_ASSETS_MENU_GRAMOPHONE_WIDTH;
    _gramophone.height = BM_ASSETS_MENU_GRAMOPHONE_HEIGHT;
    _gramophone.currentBitmap = BM_ASSETS_MENU_GRAMOPHONE;
    _gramophone.currentMask = 0;
    _gramophone.currentXPos = 85;
    _gramophone.currentYPos = 70;

    _leftArrow.width = BM_ASSETS_MENU_ARROW_WIDTH;
    _leftArrow.height = BM_ASSETS_MENU_ARROW_HEIGHT;
    _leftArrow.currentBitmap = BM_ASSETS_MENU_LEFT_ARROW_UNPRESSED;
    _leftArrow.currentMask = NULL;
    _leftArrow.currentXPos = 5;
    _leftArrow.currentYPos = 15;

    _rightArrow.width = BM_ASSETS_MENU_ARROW_WIDTH;
    _rightArrow.height = BM_ASSETS_MENU_ARROW_HEIGHT;
    _rightArrow.currentBitmap = BM_ASSETS_MENU_RIGHT_ARROW_UNPRESSED;
    _rightArrow.currentMask = NULL;
    _rightArrow.currentXPos = 97;
    _rightArrow.currentYPos = 15;

    _iconFrame.currentBitmap = BM_ASSETS_MENU_FRAME;
    _iconFrame.currentMask = NULL;
    _iconFrame.width = BM_ASSETS_MENU_FRAME_WIDTH;
    _iconFrame.height = BM_ASSETS_MENU_FRAME_HEIGHT;
    _iconFrame.currentXPos = 34;
    _iconFrame.currentYPos = 7;

    _icon.currentMask = NULL;
    _icon.width = BM_ASSETS_MENU_ICON_WIDTH;
    _icon.height = BM_ASSETS_MENU_ICON_HEIGHT;
    _icon.currentXPos = 38;
    _icon.currentYPos = 13;

    _musicNotes[0].width = BM_ASSETS_MENU_NOTE1_WIDTH;
    _musicNotes[0].height = BM_ASSETS_MENU_NOTE1_HEIGHT;
    _musicNotes[0].currentBitmap = BM_ASSETS_MENU_NOTE1;
    _musicNotes[0].currentMask = NULL;
    _musicNotes[0].currentXPos = 75;
    _musicNotes[0].currentYPos = 70;

    _musicNotes[1].width = BM_ASSETS_MENU_NOTE2_WIDTH;
    _musicNotes[1].height = BM_ASSETS_MENU_NOTE2_HEIGHT;
    _musicNotes[1].currentBitmap = BM_ASSETS_MENU_NOTE2;
    _musicNotes[1].currentMask = NULL;
    _musicNotes[1].currentXPos = 75;
    _musicNotes[1].currentYPos = 85;

    _musicNotes[2].width = BM_ASSETS_MENU_NOTE3_WIDTH;
    _musicNotes[2].height = BM_ASSETS_MENU_NOTE3_HEIGHT;
    _musicNotes[2].currentBitmap = BM_ASSETS_MENU_NOTE3;
    _musicNotes[2].currentMask = NULL;
    _musicNotes[2].currentXPos = 64;
    _musicNotes[2].currentYPos = 70;

    _musicNotes[3].width = BM_ASSETS_MENU_NOTE4_WIDTH;
    _musicNotes[3].height = BM_ASSETS_MENU_NOTE4_HEIGHT;
    _musicNotes[3].currentBitmap = BM_ASSETS_MENU_NOTE4;
    _musicNotes[3].currentMask = NULL;
    _musicNotes[3].currentXPos = 65;
    _musicNotes[3].currentYPos = 70;

    return BM_NO_ERR;
}


void BM_Scene_Menu_reset()
{
    _menuButtonPressed = 0;
    _AButtonPressed = 0;
    _BButtonPressed = 0;
    _menuButtonPressed = 0;
    _leftButtonPressed = 0;
    _rightButtonPressed = 0;
    _sceneSelection = 0;
    _sleepSettingSelect = 0;
    _firstCycle = 1;
    _sceneChanged = 0;
}


void BM_Scene_Menu_update(void *data)
{
    _sceneData = data;

    //  We want to show the scene that was active before the menu button was pressed
    //  This block sets _sceneSelection to that scene for us
    if (_firstCycle)
    {
        _sceneSelection = _sceneData->entryPacket->currentScene;
        _firstCycle = 0;
    }

    //  Wrap _sceneSelection if the user presses left or right too many times
    if (_sceneSelection < 0)
        _sceneSelection = (_sceneData->entryPacket->numScenes + BM_SCENE_MENU_NUM_SUB_SCENES) - 1;
    
    if (_sceneSelection >= _sceneData->entryPacket->numScenes + BM_SCENE_MENU_NUM_SUB_SCENES)
        _sceneSelection = 0;


    //  Update cat animation
    _catFrameCounter++;
    if (_catFrameCounter > BM_SCENE_MENU_CAT_FRAME_CHANGE_VALUE)
    {
        _catFrameCounter = 0;
        BM_Animation_update(&_cat);

        //  Need to set animation direction
        if ((BM_Animation_getDirection(&_cat) == 0) && (BM_Animation_getCurrentFrameNumber(&_cat) == (BM_ASSETS_MENU_CAT_NUM_FRAMES - 1)))
            BM_Animation_setPlaybackDirection(&_cat, 1);

        if ((BM_Animation_getDirection(&_cat) == 1) && (BM_Animation_getCurrentFrameNumber(&_cat) == 0))
            BM_Animation_setPlaybackDirection(&_cat, 0);
    }

    //  Update which note to display (if the counter expires)
    _musicNoteCounter++;
    if (_musicNoteCounter > _musicNoteTimeout)
    {
        _musicNoteCounter = 0;
        // _musicNoteTimeout = MW_AFXUnit_Utils_mapToRange(rand(), 0, __RAND_MAX, BM_SCENE_MENU_NOTE_CHANGE_VALUE_MIN, BM_SCENE_MENU_NOTE_CHANGE_VALUE_MAX);
        // _musicNoteIndex = MW_AFXUnit_Utils_mapToRange(rand(), 0, __RAND_MAX, 0, BM_SCENE_MENU_NUM_NOTES);

        _musicNoteIndex = rs_rand(3, 3);

        if (_musicNoteIndex >= BM_SCENE_MENU_NUM_NOTES)
            _musicNoteIndex = BM_SCENE_MENU_NUM_NOTES - 1;
    }

    //  Determine if we need to exit the main menu
    if (_AButtonPressed)
    {
        //  If a scene is changing, setup to enter the new scene
        if (_sceneData->entryPacket->currentScene != _sceneSelection)
        {
            //  If the user is switching to a different scene
            if (_sceneSelection >= 0 && _sceneSelection < _sceneData->entryPacket->numScenes)
            {
                *(_sceneData->entryPacket->currentSystemState) = BM_STATE_ACTIVE;
                _sceneData->entryPacket->newScene = _sceneSelection;
                _sceneData->entryPacket->scenes[_sceneData->entryPacket->currentScene].reset();
            }
        }
        else
            *(_sceneData->entryPacket->currentSystemState) = BM_STATE_ACTIVE;

        //  If the user selected sleep
        if (_sceneSelection == BM_SCENE_MENU_SLEEP_INDEX)
            *(_sceneData->entryPacket->currentSystemState) = BM_STATE_SLEEP;

        //  If the user selected settings
        if (_sceneSelection == BM_SCENE_MENU_SETTINGS_INDEX)
        {
            //  Push the settings scene onto the scene stack
            _sceneData->sceneStackSize += 1;
            if (_sceneData->sceneStackSize > _sceneData->MAX_STACK_SIZE)
                while (1);

            BM_Error err = BM_Scene_Settings_init(&_sceneData->sceneStack[_sceneData->sceneStackSize - 1]);
            if (err != BM_NO_ERR)
                while (1);
        }

        BM_Scene_Menu_reset();
    }

    if (_BButtonPressed || _menuButtonPressed)
    {
        *(_sceneData->entryPacket->currentSystemState) = BM_STATE_ACTIVE;
        BM_Scene_Menu_reset();
    }
}


void BM_Scene_Menu_draw()
{
    BM_RE_drawLightBackground();       

    BM_RE_drawSprite(_leftArrow.currentBitmap,
                    _leftArrow.currentMask,
                    _leftArrow.currentXPos,
                    _leftArrow.currentYPos,
                    _leftArrow.width,
                    _leftArrow.height);

    BM_RE_drawSprite(_rightArrow.currentBitmap,
                    _rightArrow.currentMask,
                    _rightArrow.currentXPos,
                    _rightArrow.currentYPos,
                    _rightArrow.width,
                    _rightArrow.height);

    BM_RE_drawSprite(_iconFrame.currentBitmap,
                    _iconFrame.currentMask,
                    _iconFrame.currentXPos,
                    _iconFrame.currentYPos,
                    _iconFrame.width,
                    _iconFrame.height);

    BM_RE_drawSprite(BM_Animation_getCurrentFrame(&_cat),
                    NULL,
                    10,
                    75,
                    _cat.width,
                    _cat.height);

    BM_RE_drawSprite(_gramophone.currentBitmap,
                    _gramophone.currentMask,
                    _gramophone.currentXPos,
                    _gramophone.currentYPos,
                    _gramophone.width,
                    _gramophone.height);

    BM_RE_drawSprite(_musicNotes[_musicNoteIndex].currentBitmap,
                    _musicNotes[_musicNoteIndex].currentMask,
                    _musicNotes[_musicNoteIndex].currentXPos,
                    _musicNotes[_musicNoteIndex].currentYPos,
                    _musicNotes[_musicNoteIndex].width,
                    _musicNotes[_musicNoteIndex].height);

    if (_sceneSelection < _sceneData->entryPacket->numScenes)
    {
        _icon.currentBitmap = _sceneData->entryPacket->scenes[_sceneSelection].iconBitmap;

        //  If no icon exists for a scene, write the scene name
        if (_icon.currentBitmap == NULL)
        {
            BM_RE_drawRectangleNotFilled(_icon.currentXPos, _icon.currentYPos, BM_ASSETS_MENU_ICON_WIDTH, BM_ASSETS_MENU_ICON_HEIGHT);
            BM_RE_drawString(_icon.currentXPos + 1, _icon.currentYPos + 1, _sceneData->entryPacket->scenes[_sceneSelection].sceneName);
        }

    } 
    else
    {
        if (_sceneSelection == BM_SCENE_MENU_SETTINGS_INDEX)
            _icon.currentBitmap = BM_ASSETS_MENU_ICON_SETTINGS;

        if (_sceneSelection == BM_SCENE_MENU_SLEEP_INDEX)
            _icon.currentBitmap = BM_ASSETS_MENU_ICON_SLEEP;
    }

    BM_RE_drawSprite(_icon.currentBitmap,
                    _icon.currentMask,
                    _icon.currentXPos,
                    _icon.currentYPos,
                    _icon.width,
                    _icon.height);
}


void BM_Scene_Menu_processAudio()
{

}


void BM_Scene_Menu_handleUserIO(const uint32_t buttonStates)
{
    uint32_t buttonEdgeState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_MENU, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
        _BButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_A, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
        _AButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_B, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
        _menuButtonPressed = 1;

    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_LEFT, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
    {
        _leftButtonPressed = 1;
        _sceneSelection -= 1;
    }

    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_LEFT, &buttonEdgeState));
    if (!buttonEdgeState)
        _leftArrow.currentBitmap = BM_ASSETS_MENU_LEFT_ARROW_PRESSED;

    else
        _leftArrow.currentBitmap = BM_ASSETS_MENU_LEFT_ARROW_UNPRESSED;


    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_RIGHT, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
    {
        _rightButtonPressed = 1;
        _sceneSelection += 1;
    }

    BM_ASSERT(BM_UserIO_getButtonState(BM_USERIO_RIGHT, &buttonEdgeState));
    if (!buttonEdgeState)
        _rightArrow.currentBitmap = BM_ASSETS_MENU_RIGHT_ARROW_PRESSED;

    else
        _rightArrow.currentBitmap = BM_ASSETS_MENU_RIGHT_ARROW_UNPRESSED;
}



