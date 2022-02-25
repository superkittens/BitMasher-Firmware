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

#include "BM_SceneRegister.h"


/*
 *  Register scenes to the active controller so that they may be played and selected through the menu sytem
 *  When creating your own scene, you will need to register your scene in this function here
 * 
 *  Note that a maximum of 4 (BM_CONTROLLER_ACTIVE_NUM_SCENES) scenes can be registered!
 *  When adding more scenes and 4 already exist, you will need to remove one scene.
 *  Having less than 4 scenes does not guarantee that the executable will be within memory limits!  
 */
BM_Error BM_SceneRegister_registerScenes()
{
    BM_ASSERT(BM_Controller_Active_registerScene(&BM_Scene_Clouds_init));
    BM_ASSERT(BM_Controller_Active_registerScene(&BM_Scene_Phone_init));
    BM_ASSERT(BM_Controller_Active_registerScene(&BM_Scene_Unicycle_init));
    BM_ASSERT(BM_Controller_Active_registerScene(&BM_Scene_Glitch_init));

    return BM_NO_ERR;
}