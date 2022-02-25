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
//
//  Graphical Sprite Assets under CC BY-SA 3.0
//  For more information, please refer to https://creativecommons.org/licenses/by-sa/3.0/legalcode
//
//  ------------------------------------------------------------------------------------------------  //


#include "BM_Assets_Menu.h"


int32_t BM_ASSETS_MENU_CAT_WIDTH = 69;
int32_t BM_ASSETS_MENU_CAT_HEIGHT = 52;
int32_t BM_ASSETS_MENU_CAT_NUM_FRAMES = 4;
uint8_t BM_ASSETS_MENU_CAT[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x80, 0x3f, 0x0, 0x3, 0x0, 0x0, 0x0,
0x0, 0x0, 0xe0, 0xff, 0xe3, 0x7, 0x0, 0x0, 0x0,
0x0, 0x0, 0xf8, 0xc0, 0x27, 0x1b, 0x0, 0x0, 0x0,
0x0, 0x0, 0x18, 0x0, 0x16, 0x7b, 0x0, 0x0, 0x0,
0x0, 0x0, 0xc, 0x0, 0x1e, 0xe2, 0x1, 0x0, 0x0,
0x0, 0x0, 0xc, 0x0, 0x9d, 0x80, 0x1, 0x0, 0x0,
0x0, 0x0, 0xc, 0x0, 0x91, 0x0, 0x3, 0x0, 0x0,
0x0, 0x0, 0xc, 0x80, 0xa0, 0x12, 0x7, 0x0, 0x0,
0x0, 0x0, 0x16, 0x80, 0xe0, 0x11, 0x6, 0x0, 0x0,
0x0, 0x0, 0x17, 0x80, 0x90, 0x10, 0xe, 0x0, 0x0,
0x0, 0x80, 0x16, 0x40, 0x90, 0x10, 0x1e, 0x0, 0x0,
0x0, 0x80, 0x26, 0x40, 0x48, 0x0, 0x36, 0x0, 0x0,
0x0, 0x80, 0x47, 0x20, 0x40, 0x0, 0x26, 0x0, 0x0,
0x0, 0x80, 0x83, 0x9f, 0x1, 0x0, 0x27, 0x0, 0x0,
0x0, 0xe0, 0x1, 0xc0, 0x1, 0x0, 0x17, 0x0, 0x0,
0x0, 0xfc, 0x1, 0xe0, 0x0, 0x0, 0xee, 0x3, 0x0,
0x0, 0x9e, 0x1, 0x70, 0x0, 0x0, 0xfc, 0xf, 0x0,
0x80, 0x87, 0x1, 0x38, 0x0, 0x0, 0x38, 0x1e, 0x0,
0xc0, 0x81, 0x1, 0x1e, 0x0, 0x20, 0x34, 0x38, 0x0,
0xc0, 0x80, 0x81, 0xf, 0x0, 0x20, 0x33, 0x70, 0x0,
0xf0, 0xe0, 0x81, 0x3, 0x3, 0xc0, 0x38, 0x62, 0x0,
0x78, 0xf0, 0x81, 0x81, 0x3, 0x0, 0x1e, 0xc2, 0x0,
0x18, 0xba, 0x83, 0xc1, 0x11, 0x80, 0xf, 0xc1, 0x1,
0xe, 0x19, 0x87, 0xe1, 0x38, 0xc0, 0xc3, 0x98, 0x7,
0xe, 0x18, 0xfe, 0xff, 0xff, 0xff, 0x0, 0x20, 0x1e,
0x27, 0x18, 0xfc, 0xff, 0xff, 0x7f, 0x0, 0x0, 0x1e,
0x7f, 0x18, 0x38, 0x0, 0x0, 0x0, 0x0, 0xf0, 0xf,
0xc3, 0x1b, 0x70, 0x0, 0x0, 0x0, 0xe0, 0xef, 0x3,
0x37, 0x1c, 0xe0, 0x7, 0xb8, 0xff, 0x1f, 0x90, 0x1,
0x4e, 0x38, 0x80, 0xff, 0x7, 0x0, 0x0, 0xcc, 0x1,
0x1c, 0x30, 0x0, 0x18, 0x0, 0x0, 0x80, 0xe0, 0x0,
0x78, 0xf0, 0x0, 0x1c, 0x80, 0x0, 0x80, 0x78, 0xe,
0xf0, 0xe1, 0x7, 0x1f, 0x0, 0x7, 0x20, 0x7e, 0x4,
0xca, 0xff, 0xff, 0xff, 0x3f, 0xe0, 0xc7, 0x7f, 0x0,
0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaf, 0x2,
0x74, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x9,
0xa8, 0xef, 0xfe, 0xeb, 0xaf, 0xbf, 0xea, 0xeb, 0x3,
0x7d, 0x5d, 0x55, 0x55, 0xd5, 0x5f, 0xf7, 0x55, 0x10,
0x0, 0x10, 0xde, 0x12, 0x0, 0x82, 0x8e, 0x1e, 0x0,
0x2, 0x42, 0x20, 0x0, 0xc8, 0x0, 0x0, 0x0, 0x1,
0x0, 0x20, 0x0, 0x20, 0x0, 0x20, 0x0, 0x0, 0x0,

0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x80, 0x1, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x40, 0x2, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x20, 0x4, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x20, 0x4, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x40, 0x2, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x40, 0x1, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x80, 0x3, 0x0, 0x0, 0x0,
0x0, 0x0, 0x80, 0x3f, 0xe0, 0x7, 0x0, 0x0, 0x0,
0x0, 0x0, 0xe0, 0xff, 0x23, 0x1b, 0x0, 0x0, 0x0,
0x0, 0x0, 0x78, 0xc0, 0x17, 0x7b, 0x0, 0x0, 0x0,
0x0, 0x0, 0x38, 0x0, 0x1e, 0xe2, 0x1, 0x0, 0x0,
0x0, 0x0, 0xc, 0x0, 0x9d, 0x80, 0x1, 0x0, 0x0,
0x0, 0x0, 0xc, 0x0, 0x91, 0x0, 0x3, 0x0, 0x0,
0x0, 0x0, 0xc, 0x80, 0xa0, 0x12, 0x7, 0x0, 0x0,
0x0, 0x0, 0x16, 0x80, 0xe0, 0x11, 0x6, 0x0, 0x0,
0x0, 0x0, 0x17, 0x80, 0x90, 0x10, 0xe, 0x0, 0x0,
0x0, 0x80, 0x16, 0x40, 0x90, 0x10, 0x1e, 0x0, 0x0,
0x0, 0x80, 0x26, 0x40, 0x48, 0x0, 0x36, 0x0, 0x0,
0x0, 0x80, 0x47, 0x20, 0x40, 0x0, 0x26, 0x0, 0x0,
0x0, 0x80, 0x83, 0x9f, 0x1, 0x0, 0x27, 0x0, 0x0,
0x0, 0xe0, 0x1, 0xc0, 0x1, 0x0, 0x17, 0x0, 0x0,
0x0, 0xfc, 0x1, 0xe0, 0x0, 0x0, 0xee, 0x3, 0x0,
0x0, 0x9e, 0x1, 0x70, 0x0, 0x0, 0xfc, 0xf, 0x0,
0x80, 0x87, 0x1, 0x38, 0x0, 0x0, 0x38, 0x1e, 0x0,
0xc0, 0x81, 0x1, 0x1e, 0x0, 0x20, 0x34, 0x38, 0x0,
0xc0, 0x80, 0x81, 0xf, 0x0, 0x20, 0x33, 0x70, 0x0,
0xf0, 0xe0, 0x81, 0x3, 0x3, 0xc0, 0x38, 0x62, 0x0,
0x78, 0xf0, 0x81, 0x81, 0x3, 0x0, 0x1e, 0xc2, 0x0,
0x18, 0xba, 0x83, 0xc1, 0x11, 0x80, 0xf, 0xc1, 0x1,
0xe, 0x19, 0x87, 0xe1, 0x38, 0xc0, 0xc3, 0x98, 0x7,
0xe, 0x18, 0xfe, 0xff, 0xff, 0xff, 0x0, 0x20, 0x1e,
0x27, 0x18, 0xfc, 0xff, 0xff, 0x7f, 0x0, 0x0, 0x1e,
0x7f, 0x18, 0x38, 0x0, 0x0, 0x0, 0x0, 0xf0, 0xf,
0xc3, 0x1b, 0x70, 0x0, 0x0, 0x0, 0xe0, 0xef, 0x3,
0x37, 0x1c, 0xe0, 0x7, 0xb8, 0xff, 0x1f, 0x90, 0x1,
0x4e, 0x38, 0x80, 0xff, 0x7, 0x0, 0x0, 0xcc, 0x1,
0x1c, 0x30, 0x0, 0x18, 0x0, 0x0, 0x80, 0xe0, 0x0,
0x78, 0xf0, 0x0, 0x1c, 0x80, 0x0, 0x80, 0x78, 0xe,
0xf0, 0xe1, 0x7, 0x1f, 0x0, 0x7, 0x20, 0x7e, 0x4,
0xca, 0xff, 0xff, 0xff, 0x3f, 0xe0, 0xc7, 0x7f, 0x0,
0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaf, 0x2,
0x74, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x9,
0xa8, 0xef, 0xfe, 0xeb, 0xaf, 0xbf, 0xea, 0xeb, 0x3,
0x7d, 0x5d, 0x55, 0x55, 0xd5, 0x5f, 0xf7, 0x55, 0x10,
0x0, 0x10, 0xde, 0x12, 0x0, 0x82, 0x8e, 0x1e, 0x0,
0x2, 0x42, 0x20, 0x0, 0xc8, 0x0, 0x0, 0x0, 0x1,
0x0, 0x20, 0x0, 0x20, 0x0, 0x20, 0x0, 0x0, 0x0,

0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x80, 0x1, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x40, 0x2, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x20, 0x4, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x10, 0x8, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x10, 0x8, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x10, 0x8, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x10, 0x8, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x20, 0x4, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x40, 0x2, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x40, 0x1, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x80, 0x3, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0xe0, 0x7, 0x0, 0x0, 0x0,
0x0, 0x0, 0x80, 0x3f, 0x20, 0x1b, 0x0, 0x0, 0x0,
0x0, 0x0, 0xe0, 0xff, 0x13, 0x7b, 0x0, 0x0, 0x0,
0x0, 0x0, 0xf8, 0xc0, 0x1f, 0xe2, 0x1, 0x0, 0x0,
0x0, 0x0, 0x1c, 0x0, 0x9d, 0x80, 0x1, 0x0, 0x0,
0x0, 0x0, 0xc, 0x0, 0x91, 0x0, 0x3, 0x0, 0x0,
0x0, 0x0, 0xc, 0x80, 0xa0, 0x12, 0x7, 0x0, 0x0,
0x0, 0x0, 0x16, 0x80, 0xe0, 0x11, 0x6, 0x0, 0x0,
0x0, 0x0, 0x17, 0x80, 0x90, 0x10, 0xe, 0x0, 0x0,
0x0, 0x80, 0x16, 0x40, 0x90, 0x10, 0x1e, 0x0, 0x0,
0x0, 0x80, 0x26, 0x40, 0x48, 0x0, 0x36, 0x0, 0x0,
0x0, 0x80, 0x47, 0x20, 0x40, 0x0, 0x26, 0x0, 0x0,
0x0, 0x80, 0x83, 0x9f, 0x1, 0x0, 0x27, 0x0, 0x0,
0x0, 0xe0, 0x1, 0xc0, 0x1, 0x0, 0x17, 0x0, 0x0,
0x0, 0xfc, 0x1, 0xe0, 0x0, 0x0, 0xee, 0x3, 0x0,
0x0, 0x9e, 0x1, 0x70, 0x0, 0x0, 0xfc, 0xf, 0x0,
0x80, 0x87, 0x1, 0x38, 0x0, 0x0, 0x38, 0x1e, 0x0,
0xc0, 0x81, 0x1, 0x1e, 0x0, 0x20, 0x34, 0x38, 0x0,
0xc0, 0x80, 0x81, 0xf, 0x0, 0x20, 0x33, 0x70, 0x0,
0xf0, 0xe0, 0x81, 0x3, 0x3, 0xc0, 0x38, 0x62, 0x0,
0x78, 0xf0, 0x81, 0x81, 0x3, 0x0, 0x1e, 0xc2, 0x0,
0x18, 0xba, 0x83, 0xc1, 0x11, 0x80, 0xf, 0xc1, 0x1,
0xe, 0x19, 0x87, 0xe1, 0x38, 0xc0, 0xc3, 0x98, 0x7,
0xe, 0x18, 0xfe, 0xff, 0xff, 0xff, 0x0, 0x20, 0x1e,
0x27, 0x18, 0xfc, 0xff, 0xff, 0x7f, 0x0, 0x0, 0x1e,
0x7f, 0x18, 0x38, 0x0, 0x0, 0x0, 0x0, 0xf0, 0xf,
0xc3, 0x1b, 0x70, 0x0, 0x0, 0x0, 0xe0, 0xef, 0x3,
0x37, 0x1c, 0xe0, 0x7, 0xb8, 0xff, 0x1f, 0x90, 0x1,
0x4e, 0x38, 0x80, 0xff, 0x7, 0x0, 0x0, 0xcc, 0x1,
0x1c, 0x30, 0x0, 0x18, 0x0, 0x0, 0x80, 0xe0, 0x0,
0x78, 0xf0, 0x0, 0x1c, 0x80, 0x0, 0x80, 0x78, 0xe,
0xf0, 0xe1, 0x7, 0x1f, 0x0, 0x7, 0x20, 0x7e, 0x4,
0xca, 0xff, 0xff, 0xff, 0x3f, 0xe0, 0xc7, 0x7f, 0x0,
0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaf, 0x2,
0x74, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x9,
0xa8, 0xef, 0xfe, 0xeb, 0xaf, 0xbf, 0xea, 0xeb, 0x3,
0x7d, 0x5d, 0x55, 0x55, 0xd5, 0x5f, 0xf7, 0x55, 0x10,
0x0, 0x10, 0xde, 0x12, 0x0, 0x82, 0x8e, 0x1e, 0x0,
0x2, 0x42, 0x20, 0x0, 0xc8, 0x0, 0x0, 0x0, 0x1,
0x0, 0x20, 0x0, 0x20, 0x0, 0x20, 0x0, 0x0, 0x0,

0x0, 0x0, 0x0, 0x0, 0xe0, 0x7, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x10, 0x8, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x8, 0x10, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x4, 0x20, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x4, 0x20, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x4, 0x20, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x4, 0x20, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x4, 0x20, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x8, 0x10, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x10, 0x8, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x60, 0x4, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x80, 0x3, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0xe0, 0x7, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x20, 0x1b, 0x0, 0x0, 0x0,
0x0, 0x0, 0x80, 0x3f, 0x10, 0x7b, 0x0, 0x0, 0x0,
0x0, 0x0, 0xe0, 0xff, 0x1f, 0xe2, 0x1, 0x0, 0x0,
0x0, 0x0, 0xf8, 0xc0, 0x9f, 0x80, 0x1, 0x0, 0x0,
0x0, 0x0, 0x1c, 0x0, 0x91, 0x0, 0x3, 0x0, 0x0,
0x0, 0x0, 0xc, 0x80, 0xa0, 0x12, 0x7, 0x0, 0x0,
0x0, 0x0, 0x16, 0x80, 0xe0, 0x11, 0x6, 0x0, 0x0,
0x0, 0x0, 0x17, 0x80, 0x90, 0x10, 0xe, 0x0, 0x0,
0x0, 0x80, 0x16, 0x40, 0x90, 0x10, 0x1e, 0x0, 0x0,
0x0, 0x80, 0x26, 0x40, 0x48, 0x0, 0x36, 0x0, 0x0,
0x0, 0x80, 0x47, 0x20, 0x40, 0x0, 0x26, 0x0, 0x0,
0x0, 0x80, 0x83, 0x9f, 0x1, 0x0, 0x27, 0x0, 0x0,
0x0, 0xe0, 0x1, 0xc0, 0x1, 0x0, 0x17, 0x0, 0x0,
0x0, 0xfc, 0x1, 0xe0, 0x0, 0x0, 0xee, 0x3, 0x0,
0x0, 0x9e, 0x1, 0x70, 0x0, 0x0, 0xfc, 0xf, 0x0,
0x80, 0x87, 0x1, 0x38, 0x0, 0x0, 0x38, 0x1e, 0x0,
0xc0, 0x81, 0x1, 0x1e, 0x0, 0x20, 0x34, 0x38, 0x0,
0xc0, 0x80, 0x81, 0xf, 0x0, 0x20, 0x33, 0x70, 0x0,
0xf0, 0xe0, 0x81, 0x3, 0x3, 0xc0, 0x38, 0x62, 0x0,
0x78, 0xf0, 0x81, 0x81, 0x3, 0x0, 0x1e, 0xc2, 0x0,
0x18, 0xba, 0x83, 0xc1, 0x11, 0x80, 0xf, 0xc1, 0x1,
0xe, 0x19, 0x87, 0xe1, 0x38, 0xc0, 0xc3, 0x98, 0x7,
0xe, 0x18, 0xfe, 0xff, 0xff, 0xff, 0x0, 0x20, 0x1e,
0x27, 0x18, 0xfc, 0xff, 0xff, 0x7f, 0x0, 0x0, 0x1e,
0x7f, 0x18, 0x38, 0x0, 0x0, 0x0, 0x0, 0xf0, 0xf,
0xc3, 0x1b, 0x70, 0x0, 0x0, 0x0, 0xe0, 0xef, 0x3,
0x37, 0x1c, 0xe0, 0x7, 0xb8, 0xff, 0x1f, 0x90, 0x1,
0x4e, 0x38, 0x80, 0xff, 0x7, 0x0, 0x0, 0xcc, 0x1,
0x1c, 0x30, 0x0, 0x18, 0x0, 0x0, 0x80, 0xe0, 0x0,
0x78, 0xf0, 0x0, 0x1c, 0x80, 0x0, 0x80, 0x78, 0xe,
0xf0, 0xe1, 0x7, 0x1f, 0x0, 0x7, 0x20, 0x7e, 0x4,
0xca, 0xff, 0xff, 0xff, 0x3f, 0xe0, 0xc7, 0x7f, 0x0,
0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xaf, 0x2,
0x74, 0xfd, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0x9,
0xa8, 0xef, 0xfe, 0xeb, 0xaf, 0xbf, 0xea, 0xeb, 0x3,
0x7d, 0x5d, 0x55, 0x55, 0xd5, 0x5f, 0xf7, 0x55, 0x10,
0x0, 0x10, 0xde, 0x12, 0x0, 0x82, 0x8e, 0x1e, 0x0,
0x2, 0x42, 0x20, 0x0, 0xc8, 0x0, 0x0, 0x0, 0x1,
0x0, 0x20, 0x0, 0x20, 0x0, 0x20, 0x0, 0x0, 0x0
};


int32_t BM_ASSETS_MENU_GRAMOPHONE_WIDTH = 38;
int32_t BM_ASSETS_MENU_GRAMOPHONE_HEIGHT = 54;
uint8_t BM_ASSETS_MENU_GRAMOPHONE[] =
{
    0x0, 0xe0, 0x3, 0x0, 0x0,
0x0, 0xf0, 0x7, 0x0, 0x0,
0x0, 0x78, 0x7, 0x0, 0x0,
0x0, 0x3c, 0xe, 0x0, 0x0,
0x0, 0x2e, 0xc, 0x0, 0x0,
0x0, 0x2e, 0x1d, 0x0, 0x0,
0x0, 0x47, 0x19, 0x0, 0x0,
0x80, 0x43, 0x1a, 0x0, 0x0,
0xc0, 0x81, 0x1a, 0x0, 0x0,
0xc0, 0x80, 0x3b, 0x0, 0x0,
0xc0, 0xc7, 0x3b, 0x0, 0x0,
0xe0, 0xf8, 0x39, 0x0, 0x0,
0x60, 0xe0, 0x7d, 0x0, 0x0,
0x60, 0xe0, 0x6c, 0x0, 0x0,
0x60, 0xf0, 0xce, 0x0, 0x0,
0x60, 0x7c, 0xce, 0x1, 0x0,
0x60, 0x32, 0x87, 0x7, 0x0,
0x60, 0xa2, 0x7, 0x7, 0x0,
0x60, 0x91, 0xf, 0xe, 0x0,
0xc0, 0xc9, 0x1f, 0xc, 0x0,
0xc0, 0xe0, 0x75, 0x1c, 0x0,
0x80, 0xf1, 0xdf, 0x18, 0x0,
0x80, 0xff, 0xbf, 0x39, 0x0,
0x0, 0x1f, 0xf0, 0x30, 0x0,
0x0, 0x0, 0xe0, 0x31, 0x0,
0x0, 0x0, 0x80, 0x33, 0x0,
0x0, 0x0, 0x0, 0x33, 0x0,
0x0, 0x0, 0x0, 0x33, 0x0,
0x0, 0x0, 0x0, 0x33, 0x0,
0x0, 0x0, 0x80, 0x33, 0x0,
0x0, 0x0, 0xc0, 0x39, 0x0,
0xf0, 0xff, 0xff, 0x1c, 0x0,
0x10, 0x0, 0x70, 0x2e, 0x0,
0x8, 0xfe, 0x3f, 0x26, 0x0,
0xe8, 0xff, 0x3f, 0x47, 0x0,
0xfc, 0x1f, 0xfc, 0x5f, 0x0,
0xfc, 0x87, 0xb0, 0x7f, 0x0,
0xfa, 0x1f, 0xbc, 0x9f, 0x0,
0xe2, 0xff, 0xff, 0x87, 0x30,
0x1, 0xfe, 0x7f, 0x0, 0x31,
0x1, 0x0, 0x0, 0x0, 0x31,
0xff, 0xff, 0xff, 0xff, 0x31,
0x1, 0x0, 0x0, 0x0, 0x3f,
0xff, 0xff, 0xff, 0xff, 0x3f,
0x55, 0x55, 0x55, 0x55, 0x31,
0xef, 0xee, 0xee, 0xee, 0x1,
0x55, 0x55, 0x55, 0x55, 0x1,
0xef, 0xee, 0xee, 0xee, 0x1,
0xff, 0xff, 0xff, 0xff, 0x1,
0x1, 0x0, 0x0, 0x0, 0x1,
0xff, 0xff, 0xff, 0xff, 0x1,
0xfb, 0xff, 0xff, 0xbf, 0x0,
0xfa, 0xff, 0xff, 0xbf, 0x0,
0xe, 0x0, 0x0, 0xe0, 0x0
};


int32_t BM_ASSETS_MENU_ARROW_WIDTH = 25;
int32_t BM_ASSETS_MENU_ARROW_HEIGHT = 41;
uint8_t BM_ASSETS_MENU_LEFT_ARROW_UNPRESSED[] =
{
    0x0, 0x0, 0x7e, 0x0,
0x0, 0x0, 0xff, 0x0,
0x0, 0x80, 0xc3, 0x1,
0x0, 0xc0, 0x99, 0x1,
0x0, 0xe0, 0xbc, 0x1,
0x0, 0x70, 0xbe, 0x1,
0x0, 0x38, 0xab, 0x1,
0x0, 0x9c, 0xbd, 0x1,
0x0, 0xce, 0xae, 0x1,
0x0, 0x67, 0xaf, 0x1,
0x80, 0xb3, 0xaf, 0x1,
0xc0, 0xd9, 0xaf, 0x1,
0xe0, 0xec, 0xaf, 0x1,
0x70, 0xf6, 0xbf, 0x1,
0x38, 0xfb, 0xbf, 0x1,
0x9c, 0xfd, 0xaf, 0x1,
0xce, 0xff, 0xaf, 0x1,
0x67, 0xff, 0xaf, 0x1,
0xf3, 0xff, 0xaf, 0x1,
0xfb, 0xff, 0xaf, 0x1,
0xeb, 0xff, 0xaf, 0x1,
0xfb, 0xff, 0xbf, 0x1,
0xf3, 0xff, 0xbf, 0x1,
0xe7, 0xff, 0xbf, 0x1,
0xce, 0xff, 0xbf, 0x1,
0x9c, 0xff, 0xaf, 0x1,
0x38, 0xff, 0xaf, 0x1,
0x70, 0xfe, 0xbf, 0x1,
0xe0, 0xfc, 0xbf, 0x1,
0xc0, 0xf9, 0xaf, 0x1,
0x80, 0xf3, 0xaf, 0x1,
0x0, 0xe7, 0xbf, 0x1,
0x0, 0xce, 0xbf, 0x1,
0x0, 0x9c, 0xbf, 0x1,
0x0, 0x38, 0xaf, 0x1,
0x0, 0x70, 0xbe, 0x1,
0x0, 0xe0, 0xbc, 0x1,
0x0, 0xc0, 0x99, 0x1,
0x0, 0x80, 0xc3, 0x1,
0x0, 0x0, 0xff, 0x0,
0x0, 0x0, 0x7e, 0x0
};

uint8_t BM_ASSETS_MENU_LEFT_ARROW_PRESSED[] = 
{
    0x0, 0x0, 0x7e, 0x0,
0x0, 0x0, 0xff, 0x0,
0x0, 0x80, 0xff, 0x1,
0x0, 0xc0, 0xbf, 0x1,
0x0, 0xe0, 0xff, 0x1,
0x0, 0xf0, 0xbf, 0x1,
0x0, 0xf8, 0xff, 0x1,
0x0, 0xfc, 0xbf, 0x1,
0x0, 0xfe, 0xff, 0x1,
0x0, 0xff, 0xbf, 0x1,
0x80, 0xff, 0xff, 0x1,
0xc0, 0xff, 0xbf, 0x1,
0xe0, 0xff, 0xff, 0x1,
0xf0, 0xff, 0xbf, 0x1,
0xf8, 0xff, 0xff, 0x1,
0xfc, 0xff, 0xbf, 0x1,
0xfe, 0xff, 0xff, 0x1,
0xff, 0xff, 0xbf, 0x1,
0xff, 0xff, 0xff, 0x1,
0xff, 0xff, 0xbf, 0x1,
0xff, 0xff, 0xff, 0x1,
0xff, 0xff, 0xbf, 0x1,
0xf7, 0xff, 0xff, 0x1,
0xef, 0xff, 0xbf, 0x1,
0xde, 0xff, 0xff, 0x1,
0xbc, 0xff, 0xbf, 0x1,
0x78, 0xff, 0xff, 0x1,
0xf0, 0xfe, 0xbf, 0x1,
0xe0, 0xfd, 0xff, 0x1,
0xc0, 0xfb, 0xbf, 0x1,
0x80, 0xf7, 0xff, 0x1,
0x0, 0xef, 0xbf, 0x1,
0x0, 0xde, 0xff, 0x1,
0x0, 0xbc, 0xbf, 0x1,
0x0, 0x78, 0xff, 0x1,
0x0, 0xf0, 0xbe, 0x1,
0x0, 0xe0, 0xfd, 0x1,
0x0, 0xc0, 0xbb, 0x1,
0x0, 0x80, 0xd7, 0x1,
0x0, 0x0, 0xff, 0x0,
0x0, 0x0, 0x7e, 0x0
};

uint8_t BM_ASSETS_MENU_RIGHT_ARROW_UNPRESSED[] =
{
    0xfc, 0x0, 0x0, 0x0,
0xfe, 0x1, 0x0, 0x0,
0x87, 0x3, 0x0, 0x0,
0x33, 0x7, 0x0, 0x0,
0x7b, 0xe, 0x0, 0x0,
0xfb, 0x1c, 0x0, 0x0,
0xab, 0x39, 0x0, 0x0,
0x7b, 0x73, 0x0, 0x0,
0xeb, 0xe6, 0x0, 0x0,
0xeb, 0xcd, 0x1, 0x0,
0xeb, 0x9b, 0x3, 0x0,
0xeb, 0x37, 0x7, 0x0,
0xeb, 0x6f, 0xe, 0x0,
0xfb, 0xdf, 0x1c, 0x0,
0xfb, 0xbf, 0x39, 0x0,
0xeb, 0x7f, 0x73, 0x0,
0xeb, 0xff, 0xe7, 0x0,
0xeb, 0xff, 0xcd, 0x1,
0xeb, 0xff, 0x9f, 0x1,
0xeb, 0xff, 0xbf, 0x1,
0xeb, 0xff, 0xaf, 0x1,
0xfb, 0xff, 0xbf, 0x1,
0xfb, 0xff, 0x9f, 0x1,
0xfb, 0xff, 0xcf, 0x1,
0xfb, 0xff, 0xe7, 0x0,
0xeb, 0xff, 0x73, 0x0,
0xeb, 0xff, 0x39, 0x0,
0xfb, 0xff, 0x1c, 0x0,
0xfb, 0x7f, 0xe, 0x0,
0xeb, 0x3f, 0x7, 0x0,
0xeb, 0x9f, 0x3, 0x0,
0xfb, 0xcf, 0x1, 0x0,
0xfb, 0xe7, 0x0, 0x0,
0xfb, 0x73, 0x0, 0x0,
0xeb, 0x39, 0x0, 0x0,
0xfb, 0x1c, 0x0, 0x0,
0x7b, 0xe, 0x0, 0x0,
0x33, 0x7, 0x0, 0x0,
0x87, 0x3, 0x0, 0x0,
0xfe, 0x1, 0x0, 0x0,
0xfc, 0x0, 0x0, 0x0
};

uint8_t BM_ASSETS_MENU_RIGHT_ARROW_PRESSED[] =
{
    0xfc, 0x0, 0x0, 0x0,
0xfe, 0x1, 0x0, 0x0,
0xff, 0x3, 0x0, 0x0,
0xfb, 0x7, 0x0, 0x0,
0xff, 0xf, 0x0, 0x0,
0xfb, 0x1f, 0x0, 0x0,
0xff, 0x3f, 0x0, 0x0,
0xfb, 0x7f, 0x0, 0x0,
0xff, 0xff, 0x0, 0x0,
0xfb, 0xff, 0x1, 0x0,
0xff, 0xff, 0x3, 0x0,
0xfb, 0xff, 0x7, 0x0,
0xff, 0xff, 0xf, 0x0,
0xfb, 0xff, 0x1f, 0x0,
0xff, 0xff, 0x3f, 0x0,
0xfb, 0xff, 0x7f, 0x0,
0xff, 0xff, 0xff, 0x0,
0xfb, 0xff, 0xff, 0x1,
0xff, 0xff, 0xff, 0x1,
0xfb, 0xff, 0xff, 0x1,
0xff, 0xff, 0xff, 0x1,
0xfb, 0xff, 0xff, 0x1,
0xff, 0xff, 0xdf, 0x1,
0xfb, 0xff, 0xef, 0x1,
0xff, 0xff, 0xf7, 0x0,
0xfb, 0xff, 0x7b, 0x0,
0xff, 0xff, 0x3d, 0x0,
0xfb, 0xff, 0x1e, 0x0,
0xff, 0x7f, 0xf, 0x0,
0xfb, 0xbf, 0x7, 0x0,
0xff, 0xdf, 0x3, 0x0,
0xfb, 0xef, 0x1, 0x0,
0xff, 0xf7, 0x0, 0x0,
0xfb, 0x7b, 0x0, 0x0,
0xff, 0x3d, 0x0, 0x0,
0xfb, 0x1e, 0x0, 0x0,
0x7f, 0xf, 0x0, 0x0,
0xbb, 0x7, 0x0, 0x0,
0xd7, 0x3, 0x0, 0x0,
0xfe, 0x1, 0x0, 0x0,
0xfc, 0x0, 0x0, 0x0
};


int32_t BM_ASSETS_MENU_NOTE1_WIDTH = 8;
int32_t BM_ASSETS_MENU_NOTE1_HEIGHT = 12;
uint8_t BM_ASSETS_MENU_NOTE1[] =
{
    0x30,
0x50,
0x90,
0x90,
0x50,
0x50,
0x10,
0x10,
0x1e,
0x1f,
0x1f,
0xe
};

int32_t BM_ASSETS_MENU_NOTE2_WIDTH = 5;
int32_t BM_ASSETS_MENU_NOTE2_HEIGHT = 11;
uint8_t BM_ASSETS_MENU_NOTE2[] =
{
    0xe,
0x1f,
0x1f,
0xf,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1,
0x1
};

int32_t BM_ASSETS_MENU_NOTE3_WIDTH = 13;
int32_t BM_ASSETS_MENU_NOTE3_HEIGHT = 14;
uint8_t BM_ASSETS_MENU_NOTE3[] =
{
    0x0, 0x1c,
0xc0, 0x13,
0x30, 0x1c,
0x90, 0x13,
0x70, 0x10,
0x10, 0x10,
0x10, 0x10,
0x10, 0x10,
0x10, 0x1e,
0x10, 0x1f,
0x1e, 0x1f,
0x1f, 0xe,
0x1f, 0x0,
0xe, 0x0
};

int32_t BM_ASSETS_MENU_NOTE4_WIDTH = 13;
int32_t BM_ASSETS_MENU_NOTE4_HEIGHT = 14;
uint8_t BM_ASSETS_MENU_NOTE4[] =
{
    0x0, 0xe,
0x0, 0x1f,
0xe, 0x1f,
0x1f, 0xf,
0x1f, 0x1,
0xf, 0x1,
0x1, 0x1,
0x1, 0x1,
0x1, 0x1,
0x1, 0x1,
0xe1, 0x1,
0xff, 0x1,
0x7f, 0x0,
0x7, 0x0
};


int32_t BM_ASSETS_MENU_FRAME_WIDTH = 60;
int32_t BM_ASSETS_MENU_FRAME_HEIGHT = 60;
uint8_t BM_ASSETS_MENU_FRAME[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf,
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf
};


int32_t BM_ASSETS_MENU_ICON_WIDTH = 51;
int32_t BM_ASSETS_MENU_ICON_HEIGHT = 46;


uint8_t BM_ASSETS_MENU_ICON_SLEEP[] =
{
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0,
0x0, 0x0, 0x38, 0xe7, 0x0, 0x0, 0x0,
0x0, 0x0, 0x3c, 0xe7, 0x1, 0x0, 0x0,
0x0, 0x0, 0x1e, 0xc7, 0x3, 0x0, 0x0,
0x0, 0x0, 0x7, 0x7, 0x7, 0x0, 0x0,
0x0, 0x80, 0x3, 0x7, 0xe, 0x0, 0x0,
0x0, 0xc0, 0x1, 0x7, 0x1c, 0x0, 0x0,
0x0, 0xe0, 0x0, 0x7, 0x38, 0x0, 0x0,
0x0, 0x70, 0x0, 0x7, 0x70, 0x0, 0x0,
0x0, 0x70, 0x0, 0x7, 0x70, 0x0, 0x0,
0x0, 0x30, 0x0, 0x7, 0x60, 0x0, 0x0,
0x0, 0x38, 0x0, 0x7, 0xe0, 0x0, 0x0,
0x0, 0x18, 0x0, 0x0, 0xc0, 0x0, 0x0,
0x0, 0x18, 0x0, 0x0, 0xc0, 0x0, 0x0,
0x0, 0x18, 0x0, 0x0, 0xc0, 0x0, 0x0,
0x0, 0x18, 0x0, 0x0, 0xc0, 0x0, 0x0,
0x0, 0x18, 0x0, 0x0, 0xc0, 0x0, 0x0,
0x0, 0x38, 0x0, 0x0, 0xe0, 0x0, 0x0,
0x0, 0x30, 0x0, 0x0, 0x60, 0x0, 0x0,
0x0, 0x70, 0x0, 0x0, 0x70, 0x0, 0x0,
0x0, 0x70, 0x0, 0x0, 0x70, 0x0, 0x0,
0x0, 0xe0, 0x0, 0x0, 0x38, 0x0, 0x0,
0x0, 0xc0, 0x1, 0x0, 0x1c, 0x0, 0x0,
0x0, 0x80, 0x3, 0x0, 0xe, 0x0, 0x0,
0x0, 0x0, 0x7, 0x0, 0x7, 0x0, 0x0,
0x0, 0x0, 0x1e, 0xc0, 0x3, 0x0, 0x0,
0x0, 0x0, 0x7c, 0xf0, 0x1, 0x0, 0x0,
0x0, 0x0, 0xf8, 0xff, 0x0, 0x0, 0x0,
0x0, 0x0, 0xc0, 0x1f, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0xfe, 0xe7, 0x7f, 0xfe, 0x7, 0x0,
0x0, 0xfe, 0xe7, 0x7f, 0xfe, 0x7, 0x0,
0x0, 0x80, 0x3, 0x38, 0x80, 0x3, 0x0,
0x0, 0xc0, 0x1, 0x1c, 0xc0, 0x1, 0x0,
0x0, 0xe0, 0x0, 0xe, 0xe0, 0x0, 0x0,
0x0, 0x70, 0x0, 0x7, 0x70, 0x0, 0x0,
0x0, 0x38, 0x80, 0x3, 0x38, 0x0, 0x0,
0x0, 0x1c, 0xc0, 0x1, 0x1c, 0x0, 0x0,
0x0, 0xfe, 0xe7, 0x7f, 0xfe, 0x7, 0x0,
0x0, 0xfe, 0xe7, 0x7f, 0xfe, 0x7, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};


uint8_t BM_ASSETS_MENU_ICON_SETTINGS[] =
{
    0x0, 0x0, 0xc0, 0xf, 0x0, 0x0, 0x0,
0x0, 0x0, 0xe0, 0x1f, 0x0, 0x0, 0x0,
0x0, 0x0, 0x60, 0x18, 0x0, 0x0, 0x0,
0x0, 0x0, 0x60, 0x18, 0x0, 0x0, 0x0,
0x0, 0x0, 0x60, 0x18, 0x0, 0x0, 0x0,
0x0, 0xe, 0x70, 0x38, 0xc0, 0x1, 0x0,
0x0, 0x1f, 0x3c, 0xf0, 0xe0, 0x3, 0x0,
0x80, 0x3b, 0x1f, 0xe0, 0x73, 0x7, 0x0,
0xc0, 0xf1, 0x7, 0x80, 0x3f, 0xe, 0x0,
0xc0, 0xe0, 0x1, 0x0, 0x1e, 0xc, 0x0,
0xc0, 0x1, 0x0, 0x0, 0x0, 0xe, 0x0,
0x80, 0x3, 0x0, 0x0, 0x0, 0x7, 0x0,
0x0, 0x7, 0x0, 0x0, 0x80, 0x3, 0x0,
0x0, 0x6, 0x0, 0x0, 0x80, 0x1, 0x0,
0x0, 0x6, 0xc0, 0xf, 0x80, 0x1, 0x0,
0x0, 0x6, 0xf0, 0x3f, 0x80, 0x1, 0x0,
0x0, 0x7, 0x78, 0x78, 0x80, 0x3, 0x0,
0x0, 0x3, 0x9c, 0xe7, 0x0, 0x3, 0x0,
0x80, 0x3, 0xce, 0xcf, 0x1, 0x7, 0x0,
0xf8, 0x1, 0xe6, 0x9f, 0x1, 0x7e, 0x0,
0xfc, 0x0, 0xf7, 0xbc, 0x3, 0xfe, 0x0,
0xc, 0x0, 0x7b, 0x78, 0x3, 0xc0, 0x0,
0xc, 0x0, 0x3b, 0x70, 0x3, 0xc0, 0x0,
0xc, 0x0, 0x3b, 0x70, 0x3, 0xc0, 0x0,
0xc, 0x0, 0x7b, 0x78, 0x3, 0xc0, 0x0,
0xfc, 0x0, 0xf7, 0xbc, 0x3, 0xfe, 0x0,
0xf8, 0x1, 0xe6, 0x9f, 0x1, 0x7e, 0x0,
0x80, 0x3, 0xce, 0xcf, 0x1, 0x7, 0x0,
0x0, 0x3, 0x9c, 0xe7, 0x0, 0x3, 0x0,
0x0, 0x7, 0x78, 0x78, 0x80, 0x3, 0x0,
0x0, 0x6, 0xf0, 0x3f, 0x80, 0x1, 0x0,
0x0, 0x6, 0xc0, 0xf, 0x80, 0x1, 0x0,
0x0, 0x6, 0x0, 0x0, 0x80, 0x1, 0x0,
0x0, 0x7, 0x0, 0x0, 0x80, 0x3, 0x0,
0x80, 0x3, 0x0, 0x0, 0x0, 0x7, 0x0,
0xc0, 0x1, 0x0, 0x0, 0x0, 0xe, 0x0,
0xc0, 0xe0, 0x1, 0x0, 0x1e, 0xc, 0x0,
0xc0, 0xf1, 0x7, 0x80, 0x3f, 0xe, 0x0,
0x80, 0x3b, 0x1f, 0xe0, 0x73, 0x7, 0x0,
0x0, 0x1f, 0x3c, 0xf0, 0xe0, 0x3, 0x0,
0x0, 0xe, 0x70, 0x38, 0xc0, 0x1, 0x0,
0x0, 0x0, 0x60, 0x18, 0x0, 0x0, 0x0,
0x0, 0x0, 0x60, 0x18, 0x0, 0x0, 0x0,
0x0, 0x0, 0x60, 0x18, 0x0, 0x0, 0x0,
0x0, 0x0, 0xe0, 0x1f, 0x0, 0x0, 0x0,
0x0, 0x0, 0xc0, 0xf, 0x0, 0x0, 0x0
};