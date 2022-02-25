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



#include "BM_GUIElements.h"

#define BM_GUIOBJECT_TEXT_HEIGHT                6

#define BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_X     0
#define BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_Y     (BM_GUIOBJECT_TEXT_HEIGHT + 4)
#define BM_GUIOBJECT_SLIDER_SLIDER_HEIGHT       10

#define BM_GUIOBJECT_SELECTION_TEXT_X_OFFSET    10
#define BM_GUIOBJECT_SELECTION_TEXT_Y_OFFSET    0
#define BM_GUIOBJECT_SELECTION_BOX_WIDTH        6



void BM_GUIObject_Slider_draw(const BM_GUIObject_Slider * const slider)
{
    if (slider == NULL)
        return;

    BM_RE_drawString(slider->xPos, slider->yPos, slider->text);
    BM_RE_drawRectangleNotFilled(   slider->xPos + BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_X, 
                                    slider->yPos + BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_Y, 
                                    slider->sliderWidth, 
                                    BM_GUIOBJECT_SLIDER_SLIDER_HEIGHT);

    //  Get the normalised slider value and map to actual pixel value
    float32_t normalizedValue = MW_AFXUnit_Utils_mapToRange(slider->currentValue, slider->minSliderValue, slider->maxSliderValue, 0.f, 1.f);

    //  First, clip the slider value if applicable
    if (normalizedValue < 0)
        normalizedValue = 0;
    
    if (normalizedValue > 1.f)
        normalizedValue = 1.f;

    int32_t sliderValueInPixels = normalizedValue * slider->sliderWidth;
    BM_RE_drawRectangleFilled(  slider->xPos + BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_X, 
                                slider->yPos + BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_Y, 
                                sliderValueInPixels,
                                BM_GUIOBJECT_SLIDER_SLIDER_HEIGHT);
}


const int32_t BM_GUIObject_Slider_getSliderLength()
{
    return BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_Y + BM_GUIOBJECT_SLIDER_SLIDER_HEIGHT;
}


const int32_t BM_GUIObject_Slider_getSliderHeight()
{
    return BM_GUIOBJECT_SLIDER_SLIDER_HEIGHT + BM_GUIOBJECT_SLIDER_SLIDER_OFFSET_Y;
}


void BM_GUIObject_Selection_draw(const BM_GUIObject_Selection * const selection)
{
    if (selection == NULL)
        return;

    BM_RE_drawString(selection->xPos + BM_GUIOBJECT_SELECTION_TEXT_X_OFFSET,
                selection->yPos + BM_GUIOBJECT_SELECTION_TEXT_Y_OFFSET,
                selection->text);

    if (selection->active)
        BM_RE_drawRectangleFilled(  selection->xPos,
                                    selection->yPos,
                                    BM_GUIOBJECT_SELECTION_BOX_WIDTH,
                                    BM_GUIOBJECT_SELECTION_BOX_WIDTH);

    else
        BM_RE_drawRectangleNotFilled(   selection->xPos,
                                        selection->yPos,
                                        BM_GUIOBJECT_SELECTION_BOX_WIDTH,
                                        BM_GUIOBJECT_SELECTION_BOX_WIDTH);
}   