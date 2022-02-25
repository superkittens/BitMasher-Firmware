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


#include "BM_SceneDelegate_Phone_PhoneBook.h"

static int32_t _selectorPosition = 0;


void BM_SceneDelegate_Phone_Phonebook_draw()
{
    BM_RE_drawBackground(BM_ASSETS_TELEPHONE_PHONEBOOK_BKGND);

    //  Placeholder selector
    if (_selectorPosition == 0)
        BM_RE_drawSprite(BM_ASSETS_TELEPHONE_PHONE_BOOK_SELECTOR,
                    NULL,
                    44,
                    35,
                    BM_ASSETS_TELEPHONE_PHONE_BOOK_SELECTOR_WIDTH,
                    BM_ASSETS_TELEPHONE_PHONE_BOOK_SELECTOR_HEIGHT);
    
    if (_selectorPosition == 1)
        BM_RE_drawSprite(BM_ASSETS_TELEPHONE_PHONE_BOOK_SELECTOR,
                    NULL,
                    44,
                    73,
                    BM_ASSETS_TELEPHONE_PHONE_BOOK_SELECTOR_WIDTH,
                    BM_ASSETS_TELEPHONE_PHONE_BOOK_SELECTOR_HEIGHT);
}


void BM_SceneDelegate_Phone_Phonebook_reset()
{
    _selectorPosition = 0;
}


void BM_SceneDelegate_Phone_Phonebook_handleUserIO(const uint32_t buttonStates)
{
    uint32_t buttonEdgeState = 0;
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_UP, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
    {
        _selectorPosition -= 1;
        if (_selectorPosition < 0)
            _selectorPosition = 1;
    }
    
    BM_ASSERT(BM_UserIO_getButtonEdgeState(BM_USERIO_DOWN, &buttonEdgeState, BM_USERIO_FALLING));
    if (buttonEdgeState)
        _selectorPosition = (_selectorPosition + 1) % 2;
}


int32_t BM_SceneDelegate_Phone_Phonebook_onAPressed()
{
    return _selectorPosition;
}