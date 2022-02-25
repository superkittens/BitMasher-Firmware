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

#define BM_CONTROLLER_MENU_SCENE_STACK_SIZE 3

BM_Scene    _sceneStack[BM_CONTROLLER_MENU_SCENE_STACK_SIZE];

BM_Controller_Menu_Data _sceneData;



static void BM_Controller_Menu_handleUserIO(const uint32_t buttonStates)
{

}


static int32_t BM_Controller_Menu_mainLoop()
{
    while(*(_sceneData.entryPacket->currentSystemState) == BM_STATE_MENU)
    {
        BM_ServiceQueueItem item;
        BM_Error err = BM_ServiceQueue_getItemFromServiceQueue(&item);

        int32_t topOfStack = _sceneData.sceneStackSize - 1;

        if (err == BM_NO_ERR)
        {
            switch (item.itemType)
            {
                case BM_SERVICE_UI:
                {
                    BM_UserIO_update();
                    BM_Controller_Menu_handleUserIO(0);

                    _sceneStack[topOfStack].handleUserIO(0);
                    _sceneStack[topOfStack].update(&_sceneData);

                    _sceneStack[topOfStack].draw();

                    BM_RE_updateDisplay(false);
                    break;
                }

                case BM_SERVICE_AUDIO:
                {
                    
                    break;
                }

                default:
                    break;  // TODO:  Write an error handler here
            }
        }
    }

    return 1;
}



//  ------------------------------------------------------------------------------------  //
//  ------------------------------------------------------------------------------------  //



BM_Error BM_Controller_Menu_init()
{
    _sceneData.sceneStack = _sceneStack;
    _sceneData.sceneStackSize = 1;
    _sceneData.MAX_STACK_SIZE = BM_CONTROLLER_MENU_SCENE_STACK_SIZE;

    BM_Error err = BM_Scene_Menu_init(&_sceneStack[0]);
    if (err != BM_NO_ERR) while(1);

    return BM_NO_ERR;
}


BM_Error BM_Controller_Menu_enter(BM_Controller_Menu_Entry_Packet *params)
{
    if (params == NULL) return BM_NULLPTR;

    _sceneData.entryPacket = params;
    *params->currentSystemState = BM_STATE_MENU;

    int32_t err = BM_Controller_Menu_mainLoop();
    if (!err)
        return BM_CONTROLLER_FAIL;

    return BM_NO_ERR;
}
