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


#include "BM_Assets_Cats.h"

int32_t BM_ASSETS_CAT_FRONT_WIDTH = 32;
int32_t BM_ASSETS_CAT_FRONT_HEIGHT = 38;
uint8_t BM_ASSETS_CAT_FRONT[] =
{
    0xe0, 0x1, 0x80, 0x7,
    0xf0, 0x3, 0xc0, 0xf,
    0x38, 0x7, 0xe0, 0x1c,
    0x58, 0x6, 0x60, 0x1a,
    0x8c, 0xe4, 0x27, 0x31,
    0xe, 0xfd, 0xbf, 0x70,
    0x6, 0x18, 0x18, 0x60,
    0x6, 0x0, 0x0, 0x60,
    0x7, 0xf, 0xf0, 0xe0,
    0x7, 0x6, 0x60, 0xe0,
    0x3, 0x6, 0x60, 0xc0,
    0x3, 0x0, 0x0, 0xc0,
    0x3, 0xcc, 0x33, 0xc0,
    0x3, 0x82, 0x41, 0xc0,
    0x83, 0x1f, 0xf8, 0xc1,
    0x63, 0x2, 0x40, 0xc6,
    0x3, 0x86, 0x61, 0xc0,
    0x86, 0x79, 0x9e, 0x61,
    0x46, 0x80, 0x1, 0x62,
    0xf, 0x0, 0x0, 0xf0,
    0x3b, 0x0, 0x0, 0xce,
    0x3, 0xe0, 0x7, 0xc0,
    0x3, 0x18, 0x18, 0xc0,
    0x83, 0x6, 0x60, 0xc1,
    0x83, 0x1, 0x80, 0xc1,
    0x3, 0x3, 0xc0, 0xc0,
    0x7, 0x6, 0x60, 0xe0,
    0x6, 0x4, 0x20, 0x60,
    0xc, 0x4, 0x20, 0x30,
    0xc, 0x4, 0x20, 0x38,
    0x1c, 0x6, 0x60, 0x3c,
    0xfc, 0x3, 0xc0, 0x3f,
    0xec, 0xd, 0xb0, 0x37,
    0xc, 0xf0, 0xf, 0x30,
    0xc, 0xe0, 0x7, 0x30,
    0x18, 0x30, 0xc, 0x18,
    0xf8, 0x3f, 0xfc, 0x1f,
    0xe0, 0xf, 0xf0, 0x7
};

uint8_t BM_ASSETS_CAT_FRONT_MASK[] = {
    0x1f, 0xfe, 0x7f, 0xf8,
    0xf, 0xfc, 0x3f, 0xf0,
    0x7, 0xf8, 0x1f, 0xe0,
    0x7, 0xf8, 0x1f, 0xe0,
    0x3, 0x18, 0x18, 0xc0,
    0x1, 0x0, 0x0, 0x80,
    0x1, 0x0, 0x0, 0x80,
    0x1, 0x0, 0x0, 0x80,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x1, 0x0, 0x0, 0x80,
    0x1, 0x0, 0x0, 0x80,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0,
    0x1, 0x0, 0x0, 0x80,
    0x3, 0x0, 0x0, 0xc0,
    0x3, 0x0, 0x0, 0xc0,
    0x3, 0x0, 0x0, 0xc0,
    0x3, 0x0, 0x0, 0xc0,
    0x3, 0x0, 0x0, 0xc0,
    0x3, 0x0, 0x0, 0xc0,
    0x3, 0x0, 0x0, 0xc0,
    0x7, 0xc0, 0x3, 0xe0,
    0x7, 0xc0, 0x3, 0xe0,
    0x1f, 0xf0, 0xf, 0xf8};

int32_t BM_ASSETS_CAT_WALK_LEFT_WIDTH = 36;
int32_t BM_ASSETS_CAT_WALK_LEFT_HEIGHT = 39;
uint8_t BM_ASSETS_CAT_WALK_LEFT_NUM_FRAMES = 2;
uint8_t BM_ASSETS_CAT_WALK_LEFT[] = {
    0x0, 0x38, 0x1c, 0x0, 0x0,
    0x0, 0x4c, 0x3f, 0x0, 0x0,
    0x0, 0x86, 0x73, 0x0, 0x0,
    0x80, 0xff, 0x65, 0x0, 0x0,
    0xc0, 0xff, 0xe8, 0x0, 0x0,
    0xf0, 0x60, 0xc8, 0x0, 0x0,
    0x30, 0x0, 0xd0, 0x1, 0x0,
    0x18, 0x0, 0x90, 0x1, 0x0,
    0x98, 0x7, 0x8c, 0x1, 0x0,
    0xc, 0x3, 0x80, 0x3, 0x0,
    0xc, 0x3, 0x80, 0x3, 0x0,
    0x2, 0x0, 0x0, 0x3, 0x0,
    0x9f, 0x0, 0x0, 0x3, 0x0,
    0xf, 0x1, 0x0, 0x3, 0x0,
    0xe2, 0x7, 0x0, 0x3, 0x0,
    0x2, 0x19, 0x0, 0x3, 0x0,
    0x86, 0x1, 0x80, 0x1, 0x0,
    0x78, 0x6, 0xc0, 0x1, 0x0,
    0x30, 0x8, 0x80, 0xf3, 0x1,
    0x30, 0x0, 0x0, 0xfb, 0x3,
    0x18, 0x0, 0x0, 0x1b, 0x7,
    0x78, 0x60, 0x18, 0x1b, 0x6,
    0x98, 0x63, 0x18, 0x3b, 0xe,
    0x18, 0x6c, 0x18, 0x33, 0xe,
    0xc, 0x70, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x1b, 0xc,
    0xc, 0x60, 0x18, 0x1b, 0xc,
    0xc, 0xe0, 0x1c, 0xf, 0xe,
    0xc, 0xc0, 0xf, 0x6, 0x7,
    0x1c, 0x0, 0x0, 0xc0, 0x3,
    0x38, 0x0, 0x0, 0xf8, 0x1,
    0xf0, 0x10, 0x80, 0x7f, 0x0,
    0xe0, 0x23, 0x0, 0x7, 0x0,
    0xe0, 0xff, 0x1, 0x3, 0x0,
    0xe0, 0xff, 0xff, 0x3, 0x0,
    0xe0, 0x1f, 0xff, 0x3, 0x0,

    0x0, 0x38, 0x1c, 0x0, 0x0,
    0x0, 0x4c, 0x3f, 0x0, 0x0,
    0x0, 0x86, 0x73, 0x0, 0x0,
    0x80, 0xff, 0x65, 0x0, 0x0,
    0xc0, 0xff, 0xe8, 0x0, 0x0,
    0xf0, 0x60, 0xc8, 0x0, 0x0,
    0x30, 0x0, 0xd0, 0x1, 0x0,
    0x18, 0x0, 0x90, 0x1, 0x0,
    0x98, 0x7, 0x8c, 0x1, 0x0,
    0xc, 0x3, 0x80, 0x3, 0x0,
    0xc, 0x3, 0x80, 0x3, 0x0,
    0x2, 0x0, 0x0, 0x3, 0x0,
    0x9f, 0x0, 0x0, 0x3, 0x0,
    0xf, 0x1, 0x0, 0x3, 0x0,
    0xe2, 0x7, 0x0, 0x3, 0x0,
    0x2, 0x19, 0x0, 0x3, 0x0,
    0x86, 0x1, 0x80, 0x1, 0x0,
    0x78, 0x6, 0xc0, 0x1, 0x0,
    0x30, 0x8, 0x80, 0xf3, 0x1,
    0x30, 0x0, 0x0, 0xfb, 0x3,
    0x18, 0x0, 0x0, 0x1b, 0x7,
    0x78, 0x60, 0x18, 0x1b, 0x6,
    0x98, 0x63, 0x18, 0x3b, 0xe,
    0x18, 0x6c, 0x18, 0x33, 0xe,
    0xc, 0x70, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x33, 0xc,
    0xc, 0x60, 0x18, 0x1b, 0xc,
    0xc, 0x60, 0x18, 0x1b, 0xc,
    0xc, 0xe0, 0x1c, 0xf, 0xe,
    0xc, 0xc0, 0xf, 0x6, 0x7,
    0x1c, 0x0, 0x0, 0xc0, 0x3,
    0x38, 0x0, 0x0, 0xf8, 0x1,
    0x30, 0x0, 0xc0, 0x7f, 0x0,
    0x18, 0x0, 0xf0, 0x7, 0x0,
    0x18, 0x80, 0xff, 0x0, 0x0,
    0x38, 0xc0, 0xff, 0x1, 0x0,
    0xf8, 0xff, 0xff, 0x1, 0x0};

uint8_t BM_ASSETS_CAT_WALK_LEFT_MASK[] = {
    0xff, 0xc7, 0xe3, 0xff, 0xf,
    0xff, 0x83, 0xc0, 0xff, 0xf,
    0xff, 0x1, 0x80, 0xff, 0xf,
    0x7f, 0x0, 0x80, 0xff, 0xf,
    0x3f, 0x0, 0x0, 0xff, 0xf,
    0xf, 0x0, 0x0, 0xff, 0xf,
    0xf, 0x0, 0x0, 0xfe, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf,
    0x3, 0x0, 0x0, 0xfc, 0xf,
    0x3, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfc, 0xf,
    0x0, 0x0, 0x0, 0xfc, 0xf,
    0x0, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfe, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf,
    0xf, 0x0, 0x0, 0xc, 0xe,
    0xf, 0x0, 0x0, 0x4, 0xc,
    0x7, 0x0, 0x0, 0x4, 0x8,
    0x7, 0x0, 0x0, 0x4, 0x8,
    0x7, 0x0, 0x0, 0x4, 0x0,
    0x7, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0x4, 0x0,
    0x3, 0x0, 0x0, 0x4, 0x0,
    0x3, 0x0, 0x0, 0x0, 0x0,
    0x3, 0x0, 0x0, 0x0, 0x8,
    0x3, 0x0, 0x0, 0x0, 0xc,
    0x7, 0x0, 0x0, 0x0, 0xe,
    0xf, 0x0, 0x0, 0x80, 0xf,
    0x1f, 0x0, 0x0, 0xf8, 0xf,
    0x1f, 0x0, 0x0, 0xfc, 0xf,
    0x1f, 0x0, 0x0, 0xfc, 0xf,
    0x1f, 0xe0, 0x0, 0xfc, 0xf,
    
    0xff, 0xc7, 0xe3, 0xff, 0xf,
    0xff, 0x83, 0xc0, 0xff, 0xf,
    0xff, 0x1, 0x80, 0xff, 0xf,
    0x7f, 0x0, 0x80, 0xff, 0xf,
    0x3f, 0x0, 0x0, 0xff, 0xf,
    0xf, 0x0, 0x0, 0xff, 0xf,
    0xf, 0x0, 0x0, 0xfe, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf,
    0x3, 0x0, 0x0, 0xfc, 0xf,
    0x3, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfc, 0xf,
    0x0, 0x0, 0x0, 0xfc, 0xf,
    0x0, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfc, 0xf,
    0x1, 0x0, 0x0, 0xfe, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf,
    0xf, 0x0, 0x0, 0xc, 0xe,
    0xf, 0x0, 0x0, 0x4, 0xc,
    0x7, 0x0, 0x0, 0x4, 0x8,
    0x7, 0x0, 0x0, 0x4, 0x8,
    0x7, 0x0, 0x0, 0x4, 0x0,
    0x7, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0xc, 0x0,
    0x3, 0x0, 0x0, 0x4, 0x0,
    0x3, 0x0, 0x0, 0x4, 0x0,
    0x3, 0x0, 0x0, 0x0, 0x0,
    0x3, 0x0, 0x0, 0x0, 0x8,
    0x3, 0x0, 0x0, 0x0, 0xc,
    0x7, 0x0, 0x0, 0x0, 0xe,
    0xf, 0x0, 0x0, 0x80, 0xf,
    0x7, 0x0, 0x0, 0xf8, 0xf,
    0x7, 0x0, 0x0, 0xff, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf,
    0x7, 0x0, 0x0, 0xfe, 0xf};

int32_t BM_ASSETS_CAT_WALK_RIGHT_WIDTH = 36;
int32_t BM_ASSETS_CAT_WALK_RIGHT_HEIGHT = 39;
uint8_t BM_ASSETS_CAT_WALK_RIGHT_NUM_FRAMES = 2;
uint8_t BM_ASSETS_CAT_WALK_RIGHT[] = {
    0x0, 0x80, 0xc3, 0x1, 0x0,
    0x0, 0xc0, 0x2f, 0x3, 0x0,
    0x0, 0xe0, 0x1c, 0x6, 0x0,
    0x0, 0x60, 0xfa, 0x1f, 0x0,
    0x0, 0x70, 0xf1, 0x3f, 0x0,
    0x0, 0x30, 0x61, 0xf0, 0x0,
    0x0, 0xb8, 0x0, 0xc0, 0x0,
    0x0, 0x98, 0x0, 0x80, 0x1,
    0x0, 0x18, 0x3, 0x9e, 0x1,
    0x0, 0x1c, 0x0, 0xc, 0x3,
    0x0, 0x1c, 0x0, 0xc, 0x3,
    0x0, 0xc, 0x0, 0x0, 0x4,
    0x0, 0xc, 0x0, 0x90, 0xf,
    0x0, 0xc, 0x0, 0x8, 0xf,
    0x0, 0xc, 0x0, 0x7e, 0x4,
    0x0, 0xc, 0x80, 0x9, 0x4,
    0x0, 0x18, 0x0, 0x18, 0x6,
    0x0, 0x38, 0x0, 0xe6, 0x1,
    0xf8, 0x1c, 0x0, 0xc1, 0x0,
    0xfc, 0xd, 0x0, 0xc0, 0x0,
    0x8e, 0xd, 0x0, 0x80, 0x1,
    0x86, 0x8d, 0x61, 0xe0, 0x1,
    0xc7, 0x8d, 0x61, 0x9c, 0x1,
    0xc7, 0x8c, 0x61, 0x83, 0x1,
    0xc3, 0x8c, 0xe1, 0x0, 0x3,
    0xc3, 0x8c, 0x61, 0x0, 0x3,
    0xc3, 0x8c, 0x61, 0x0, 0x3,
    0xc3, 0x8c, 0x61, 0x0, 0x3,
    0x83, 0x8d, 0x61, 0x0, 0x3,
    0x83, 0x8d, 0x61, 0x0, 0x3,
    0x7, 0x8f, 0x73, 0x0, 0x3,
    0xe, 0x6, 0x3f, 0x0, 0x3,
    0x3c, 0x0, 0x0, 0x80, 0x3,
    0xf8, 0x1, 0x0, 0xc0, 0x1,
    0xe0, 0x1f, 0x80, 0xf0, 0x0,
    0x0, 0xe, 0x40, 0x7c, 0x0,
    0x0, 0xc, 0xf8, 0x7f, 0x0,
    0x0, 0xfc, 0xff, 0x7f, 0x0,
    0x0, 0xfc, 0x8f, 0x7f, 0x0,

    0x0, 0x80, 0xc3, 0x1, 0x0,
    0x0, 0xc0, 0x2f, 0x3, 0x0,
    0x0, 0xe0, 0x1c, 0x6, 0x0,
    0x0, 0x60, 0xfa, 0x1f, 0x0,
    0x0, 0x70, 0xf1, 0x3f, 0x0,
    0x0, 0x30, 0x61, 0xf0, 0x0,
    0x0, 0xb8, 0x0, 0xc0, 0x0,
    0x0, 0x98, 0x0, 0x80, 0x1,
    0x0, 0x18, 0x3, 0x9e, 0x1,
    0x0, 0x1c, 0x0, 0xc, 0x3,
    0x0, 0x1c, 0x0, 0xc, 0x3,
    0x0, 0xc, 0x0, 0x0, 0x4,
    0x0, 0xc, 0x0, 0x90, 0xf,
    0x0, 0xc, 0x0, 0x8, 0xf,
    0x0, 0xc, 0x0, 0x7e, 0x4,
    0x0, 0xc, 0x80, 0x9, 0x4,
    0x0, 0x18, 0x0, 0x18, 0x6,
    0x0, 0x38, 0x0, 0xe6, 0x1,
    0xf8, 0x1c, 0x0, 0xc1, 0x0,
    0xfc, 0xd, 0x0, 0xc0, 0x0,
    0x8e, 0xd, 0x0, 0x80, 0x1,
    0x86, 0x8d, 0x61, 0xe0, 0x1,
    0xc7, 0x8d, 0x61, 0x9c, 0x1,
    0xc7, 0x8c, 0x61, 0x83, 0x1,
    0xc3, 0x8c, 0xe1, 0x0, 0x3,
    0xc3, 0x8c, 0x61, 0x0, 0x3,
    0xc3, 0x8c, 0x61, 0x0, 0x3,
    0xc3, 0x8c, 0x61, 0x0, 0x3,
    0x83, 0x8d, 0x61, 0x0, 0x3,
    0x83, 0x8d, 0x61, 0x0, 0x3,
    0x7, 0x8f, 0x73, 0x0, 0x3,
    0xe, 0x6, 0x3f, 0x0, 0x3,
    0x3c, 0x0, 0x0, 0x80, 0x3,
    0xf8, 0x1, 0x0, 0xc0, 0x1,
    0xe0, 0x3f, 0x0, 0xc0, 0x0,
    0x0, 0xfe, 0x0, 0x80, 0x1,
    0x0, 0xf0, 0x1f, 0x80, 0x1,
    0x0, 0xf8, 0x3f, 0xc0, 0x1,
    0x0, 0xf8, 0xff, 0xff, 0x1};

uint8_t BM_ASSETS_CAT_WALK_RIGHT_MASK[] = {
    0xff, 0x7f, 0x3c, 0xfe, 0xf,
0xff, 0x3f, 0x10, 0xfc, 0xf,
0xff, 0x1f, 0x0, 0xf8, 0xf,
0xff, 0x1f, 0x0, 0xe0, 0xf,
0xff, 0xf, 0x0, 0xc0, 0xf,
0xff, 0xf, 0x0, 0x0, 0xf,
0xff, 0x7, 0x0, 0x0, 0xf,
0xff, 0x7, 0x0, 0x0, 0xe,
0xff, 0x7, 0x0, 0x0, 0xe,
0xff, 0x3, 0x0, 0x0, 0xc,
0xff, 0x3, 0x0, 0x0, 0xc,
0xff, 0x3, 0x0, 0x0, 0x8,
0xff, 0x3, 0x0, 0x0, 0x0,
0xff, 0x3, 0x0, 0x0, 0x0,
0xff, 0x3, 0x0, 0x0, 0x8,
0xff, 0x3, 0x0, 0x0, 0x8,
0xff, 0x7, 0x0, 0x0, 0x8,
0xff, 0x7, 0x0, 0x0, 0xe,
0x7, 0x3, 0x0, 0x0, 0xf,
0x3, 0x2, 0x0, 0x0, 0xf,
0x1, 0x2, 0x0, 0x0, 0xe,
0x1, 0x2, 0x0, 0x0, 0xe,
0x0, 0x2, 0x0, 0x0, 0xe,
0x0, 0x3, 0x0, 0x0, 0xe,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x2, 0x0, 0x0, 0xc,
0x0, 0x2, 0x0, 0x0, 0xc,
0x0, 0x0, 0x0, 0x0, 0xc,
0x1, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0xc,
0x7, 0x0, 0x0, 0x0, 0xe,
0x1f, 0x0, 0x0, 0x0, 0xf,
0xff, 0x1, 0x0, 0x80, 0xf,
0xff, 0x3, 0x0, 0x80, 0xf,
0xff, 0x3, 0x0, 0x80, 0xf,
0xff, 0x3, 0x70, 0x80, 0xf,

0xff, 0x7f, 0x3c, 0xfe, 0xf,
0xff, 0x3f, 0x10, 0xfc, 0xf,
0xff, 0x1f, 0x0, 0xf8, 0xf,
0xff, 0x1f, 0x0, 0xe0, 0xf,
0xff, 0xf, 0x0, 0xc0, 0xf,
0xff, 0xf, 0x0, 0x0, 0xf,
0xff, 0x7, 0x0, 0x0, 0xf,
0xff, 0x7, 0x0, 0x0, 0xe,
0xff, 0x7, 0x0, 0x0, 0xe,
0xff, 0x3, 0x0, 0x0, 0xc,
0xff, 0x3, 0x0, 0x0, 0xc,
0xff, 0x3, 0x0, 0x0, 0x8,
0xff, 0x3, 0x0, 0x0, 0x0,
0xff, 0x3, 0x0, 0x0, 0x0,
0xff, 0x3, 0x0, 0x0, 0x8,
0xff, 0x3, 0x0, 0x0, 0x8,
0xff, 0x7, 0x0, 0x0, 0x8,
0xff, 0x7, 0x0, 0x0, 0xe,
0x7, 0x3, 0x0, 0x0, 0xf,
0x3, 0x2, 0x0, 0x0, 0xf,
0x1, 0x2, 0x0, 0x0, 0xe,
0x1, 0x2, 0x0, 0x0, 0xe,
0x0, 0x2, 0x0, 0x0, 0xe,
0x0, 0x3, 0x0, 0x0, 0xe,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x3, 0x0, 0x0, 0xc,
0x0, 0x2, 0x0, 0x0, 0xc,
0x0, 0x2, 0x0, 0x0, 0xc,
0x0, 0x0, 0x0, 0x0, 0xc,
0x1, 0x0, 0x0, 0x0, 0xc,
0x3, 0x0, 0x0, 0x0, 0xc,
0x7, 0x0, 0x0, 0x0, 0xe,
0x1f, 0x0, 0x0, 0x0, 0xf,
0xff, 0x1, 0x0, 0x0, 0xe,
0xff, 0xf, 0x0, 0x0, 0xe,
0xff, 0x7, 0x0, 0x0, 0xe,
0xff, 0x7, 0x0, 0x0, 0xe
};


int32_t BM_ASSETS_CAT_JUMP_LEFT_WIDTH = 41;
int32_t BM_ASSETS_CAT_JUMP_LEFT_HEIGHT = 41;
uint8_t BM_ASSETS_CAT_JUMP_LEFT[] = {
    0x0, 0x80, 0x7, 0x0, 0x0, 0x0,
    0x0, 0xc0, 0xec, 0x1, 0x0, 0x0,
    0x0, 0xf8, 0xf8, 0x3, 0x0, 0x0,
    0x0, 0xfe, 0x1f, 0x7, 0x0, 0x0,
    0x0, 0xf, 0x5f, 0x6, 0x0, 0x0,
    0x80, 0x3, 0x8c, 0xe, 0x0, 0x0,
    0x80, 0x1, 0x80, 0xc, 0x0, 0x0,
    0xc0, 0x78, 0x0, 0xd, 0x0, 0x0,
    0xe0, 0x30, 0x80, 0x1d, 0x0, 0x0,
    0x10, 0x30, 0x0, 0x1c, 0x0, 0x0,
    0xf8, 0x0, 0x0, 0x1c, 0x0, 0x0,
    0x78, 0x8, 0x0, 0x1c, 0x0, 0x0,
    0x10, 0xf, 0x0, 0x18, 0x0, 0x0,
    0x30, 0x38, 0x0, 0x18, 0x0, 0x0,
    0x20, 0xcc, 0x0, 0x18, 0x0, 0x0,
    0xc0, 0x37, 0x0, 0x18, 0x0, 0x0,
    0xc0, 0xe1, 0x1f, 0x1c, 0x0, 0x0,
    0xc0, 0xc0, 0x39, 0xe, 0x0, 0x0,
    0x60, 0xc0, 0x30, 0x7, 0x0, 0x0,
    0x60, 0xc0, 0x30, 0x6, 0x0, 0x0,
    0x30, 0xc0, 0x30, 0xe, 0x0, 0x0,
    0x30, 0xc0, 0x30, 0xc, 0x0, 0x0,
    0xf0, 0xc0, 0x30, 0xc, 0x0, 0x0,
    0x38, 0xc7, 0x30, 0xc, 0x3c, 0x0,
    0x18, 0xd8, 0x30, 0xc, 0x7c, 0x0,
    0x18, 0x20, 0x0, 0xc, 0xe6, 0x1,
    0x18, 0x20, 0x0, 0xc, 0x86, 0x1,
    0x18, 0x40, 0x0, 0xc, 0x86, 0x1,
    0x18, 0x40, 0x0, 0xc, 0x83, 0x1,
    0x18, 0x40, 0x0, 0x8c, 0x83, 0x1,
    0x18, 0x40, 0x0, 0xcc, 0x81, 0x1,
    0x38, 0x40, 0x0, 0xfc, 0xc0, 0x1,
    0x38, 0x60, 0x0, 0x1c, 0xe0, 0x0,
    0x3e, 0x20, 0x0, 0x0, 0x70, 0x0,
    0x7f, 0x10, 0x0, 0x0, 0x38, 0x0,
    0xff, 0x18, 0x0, 0xff, 0x1f, 0x0,
    0xff, 0x21, 0x0, 0xff, 0x7, 0x0,
    0xff, 0xff, 0x0, 0x3, 0x0, 0x0,
    0xfe, 0xff, 0x3, 0x3, 0x0, 0x0,
    0x0, 0x0, 0xff, 0x3, 0x0, 0x0,
    0x0, 0x0, 0xfc, 0x3, 0x0, 0x0};

uint8_t BM_ASSETS_CAT_JUMP_LEFT_MASK[] = {
    0xff, 0x7f, 0xf8, 0xff, 0xff, 0x1,
    0xff, 0x3f, 0x10, 0xfe, 0xff, 0x1,
    0xff, 0x7, 0x0, 0xfc, 0xff, 0x1,
    0xff, 0x1, 0x0, 0xf8, 0xff, 0x1,
    0xff, 0x0, 0x0, 0xf8, 0xff, 0x1,
    0x7f, 0x0, 0x0, 0xf0, 0xff, 0x1,
    0x7f, 0x0, 0x0, 0xf0, 0xff, 0x1,
    0x3f, 0x0, 0x0, 0xf0, 0xff, 0x1,
    0x1f, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0xf, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0x7, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0x7, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0xf, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0xf, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0x1f, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0x3f, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0x3f, 0x0, 0x0, 0xe0, 0xff, 0x1,
    0x3f, 0x0, 0x0, 0xf0, 0xff, 0x1,
    0x1f, 0x0, 0x0, 0xf8, 0xff, 0x1,
    0x1f, 0x0, 0x0, 0xf8, 0xff, 0x1,
    0xf, 0x0, 0x0, 0xf0, 0xff, 0x1,
    0xf, 0x0, 0x0, 0xf0, 0xff, 0x1,
    0xf, 0x0, 0x0, 0xf0, 0xff, 0x1,
    0x7, 0x0, 0x0, 0xf0, 0xc3, 0x1,
    0x7, 0x0, 0x0, 0xf0, 0x83, 0x1,
    0x7, 0x0, 0x0, 0xf0, 0x1, 0x0,
    0x7, 0x0, 0x0, 0xf0, 0x1, 0x0,
    0x7, 0x0, 0x0, 0xf0, 0x1, 0x0,
    0x7, 0x0, 0x0, 0xf0, 0x0, 0x0,
    0x7, 0x0, 0x0, 0x70, 0x0, 0x0,
    0x7, 0x0, 0x0, 0x30, 0x0, 0x0,
    0x7, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x7, 0x0, 0x0, 0x0, 0x0, 0x1,
    0x1, 0x0, 0x0, 0x0, 0x80, 0x1,
    0x0, 0x0, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x0, 0x0, 0x0, 0xe0, 0x1,
    0x0, 0x0, 0x0, 0x0, 0xf8, 0x1,
    0x0, 0x0, 0x0, 0xfc, 0xff, 0x1,
    0x1, 0x0, 0x0, 0xfc, 0xff, 0x1,
    0xff, 0xff, 0x0, 0xfc, 0xff, 0x1,
    0xff, 0xff, 0x3, 0xfc, 0xff, 0x1};

int32_t BM_ASSETS_CAT_JUMP_RIGHT_WIDTH = 41;
int32_t BM_ASSETS_CAT_JUMP_RIGHT_HEIGHT = 41;
uint8_t BM_ASSETS_CAT_JUMP_RIGHT[] =
    {
        0x0, 0x0, 0xc0, 0x3, 0x0, 0x0,
        0x0, 0x0, 0x6f, 0x6, 0x0, 0x0,
        0x0, 0x80, 0x3f, 0x3e, 0x0, 0x0,
        0x0, 0xc0, 0xf1, 0xff, 0x0, 0x0,
        0x0, 0xc0, 0xf4, 0xe1, 0x1, 0x0,
        0x0, 0xe0, 0x62, 0x80, 0x3, 0x0,
        0x0, 0x60, 0x2, 0x0, 0x3, 0x0,
        0x0, 0x60, 0x1, 0x3c, 0x6, 0x0,
        0x0, 0x70, 0x3, 0x18, 0xe, 0x0,
        0x0, 0x70, 0x0, 0x18, 0x10, 0x0,
        0x0, 0x70, 0x0, 0x0, 0x3e, 0x0,
        0x0, 0x70, 0x0, 0x20, 0x3c, 0x0,
        0x0, 0x30, 0x0, 0xe0, 0x11, 0x0,
        0x0, 0x30, 0x0, 0x38, 0x18, 0x0,
        0x0, 0x30, 0x0, 0x66, 0x8, 0x0,
        0x0, 0x30, 0x0, 0xd8, 0x7, 0x0,
        0x0, 0x70, 0xf0, 0xf, 0x7, 0x0,
        0x0, 0xe0, 0x38, 0x7, 0x6, 0x0,
        0x0, 0xc0, 0x19, 0x6, 0xc, 0x0,
        0x0, 0xc0, 0x18, 0x6, 0xc, 0x0,
        0x0, 0xe0, 0x18, 0x6, 0x18, 0x0,
        0x0, 0x60, 0x18, 0x6, 0x18, 0x0,
        0x0, 0x60, 0x18, 0x6, 0x1e, 0x0,
        0x78, 0x60, 0x18, 0xc6, 0x39, 0x0,
        0x7c, 0x60, 0x18, 0x36, 0x30, 0x0,
        0xcf, 0x60, 0x0, 0x8, 0x30, 0x0,
        0xc3, 0x60, 0x0, 0x8, 0x30, 0x0,
        0xc3, 0x60, 0x0, 0x4, 0x30, 0x0,
        0x83, 0x61, 0x0, 0x4, 0x30, 0x0,
        0x83, 0x63, 0x0, 0x4, 0x30, 0x0,
        0x3, 0x67, 0x0, 0x4, 0x30, 0x0,
        0x7, 0x7e, 0x0, 0x4, 0x38, 0x0,
        0xe, 0x70, 0x0, 0xc, 0x38, 0x0,
        0x1c, 0x0, 0x0, 0x8, 0xf8, 0x0,
        0x38, 0x0, 0x0, 0x10, 0xfc, 0x1,
        0xf0, 0xff, 0x1, 0x30, 0xfe, 0x1,
        0xc0, 0xff, 0x1, 0x8, 0xff, 0x1,
        0x0, 0x80, 0x1, 0xfe, 0xff, 0x1,
        0x0, 0x80, 0x81, 0xff, 0xff, 0x0,
        0x0, 0x80, 0xff, 0x1, 0x0, 0x0,
        0x0, 0x80, 0x7f, 0x0, 0x0, 0x0};

uint8_t BM_ASSETS_CAT_JUMP_RIGHT_MASK[] = {
    0xff, 0xff, 0x3f, 0xfc, 0xff, 0x1,
    0xff, 0xff, 0x10, 0xf8, 0xff, 0x1,
    0xff, 0x7f, 0x0, 0xc0, 0xff, 0x1,
    0xff, 0x3f, 0x0, 0x0, 0xff, 0x1,
    0xff, 0x3f, 0x0, 0x0, 0xfe, 0x1,
    0xff, 0x1f, 0x0, 0x0, 0xfc, 0x1,
    0xff, 0x1f, 0x0, 0x0, 0xfc, 0x1,
    0xff, 0x1f, 0x0, 0x0, 0xf8, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xf0, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xe0, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xc0, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xc0, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xe0, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xe0, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xf0, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xf8, 0x1,
    0xff, 0xf, 0x0, 0x0, 0xf8, 0x1,
    0xff, 0x1f, 0x0, 0x0, 0xf8, 0x1,
    0xff, 0x3f, 0x0, 0x0, 0xf0, 0x1,
    0xff, 0x3f, 0x0, 0x0, 0xf0, 0x1,
    0xff, 0x1f, 0x0, 0x0, 0xe0, 0x1,
    0xff, 0x1f, 0x0, 0x0, 0xe0, 0x1,
    0xff, 0x1f, 0x0, 0x0, 0xe0, 0x1,
    0x87, 0x1f, 0x0, 0x0, 0xc0, 0x1,
    0x83, 0x1f, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x1f, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x1f, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x1f, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x1e, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x1c, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x18, 0x0, 0x0, 0xc0, 0x1,
    0x0, 0x0, 0x0, 0x0, 0xc0, 0x1,
    0x1, 0x0, 0x0, 0x0, 0xc0, 0x1,
    0x3, 0x0, 0x0, 0x0, 0x0, 0x1,
    0x7, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xf, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x3f, 0x0, 0x0, 0x0, 0x0, 0x0,
    0xff, 0x7f, 0x0, 0x0, 0x0, 0x0,
    0xff, 0x7f, 0x0, 0x0, 0x0, 0x1,
    0xff, 0x7f, 0x0, 0xfe, 0xff, 0x1,
    0xff, 0x7f, 0x80, 0xff, 0xff, 0x1};


int32_t BM_ASSETS_CAT_FACE_LEFT_WIDTH = 36;
int32_t BM_ASSETS_CAT_FACE_LEFT_HEIGHT = 39;
uint8_t BM_ASSETS_CAT_FACE_LEFT[] =
    {
        0x0, 0x38, 0x1c, 0x0, 0x0,
        0x0, 0x4c, 0x3f, 0x0, 0x0,
        0x0, 0x86, 0x73, 0x0, 0x0,
        0x80, 0xff, 0x65, 0x0, 0x0,
        0xc0, 0xff, 0xe8, 0x0, 0x0,
        0xf0, 0x60, 0xc8, 0x0, 0x0,
        0x30, 0x0, 0xd0, 0x1, 0x0,
        0x18, 0x0, 0x90, 0x1, 0x0,
        0x98, 0x7, 0x8c, 0x1, 0x0,
        0xc, 0x3, 0x80, 0x3, 0x0,
        0xc, 0x3, 0x80, 0x3, 0x0,
        0x2, 0x0, 0x0, 0x3, 0x0,
        0x9f, 0x0, 0x0, 0x3, 0x0,
        0xf, 0x1, 0x0, 0x3, 0x0,
        0xe2, 0x7, 0x0, 0x3, 0x0,
        0x2, 0x19, 0x0, 0x3, 0x0,
        0x86, 0x1, 0x80, 0x1, 0x0,
        0x78, 0x6, 0xc0, 0x1, 0x0,
        0x30, 0x8, 0x80, 0xf3, 0x1,
        0x30, 0x0, 0x0, 0xfb, 0x3,
        0x18, 0x0, 0x0, 0x1b, 0x7,
        0x78, 0x60, 0x18, 0x1b, 0x6,
        0x98, 0x63, 0x18, 0x3b, 0xe,
        0x18, 0x6c, 0x18, 0x33, 0xe,
        0xc, 0x70, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x1b, 0xc,
        0xc, 0x60, 0x18, 0x1b, 0xc,
        0x1c, 0xe0, 0xc, 0xf, 0xe,
        0x18, 0xd0, 0xf, 0x6, 0x7,
        0x18, 0x8, 0x0, 0xc0, 0x3,
        0x30, 0x7, 0x0, 0xf8, 0x1,
        0xf0, 0x10, 0xc0, 0x7f, 0x0,
        0xe0, 0x23, 0xc0, 0x7, 0x0,
        0xe0, 0x7f, 0x60, 0x0, 0x0,
        0xc0, 0xdf, 0x7f, 0x0, 0x0,
        0x80, 0xf, 0x1f, 0x0, 0x0};

int32_t BM_ASSETS_CAT_TALK_WIDTH = 36;
int32_t BM_ASSETS_CAT_TALK_HEIGHT = 39;
uint8_t BM_ASSETS_CAT_TALK[] =
    {
        0x0, 0x1c, 0x78, 0x0, 0x0,
        0x0, 0x26, 0xfe, 0x0, 0x0,
        0x0, 0x43, 0xe7, 0x0, 0x0,
        0x80, 0xff, 0xc3, 0x0, 0x0,
        0xc0, 0xff, 0xd1, 0x0, 0x0,
        0xf0, 0x0, 0x90, 0x1, 0x0,
        0x30, 0x0, 0xa0, 0x1, 0x0,
        0x18, 0x0, 0xa0, 0x1, 0x0,
        0x98, 0x7, 0x98, 0x1, 0x0,
        0xc, 0x3, 0x80, 0x3, 0x0,
        0xc, 0x3, 0x80, 0x3, 0x0,
        0x2, 0x0, 0x0, 0x3, 0x0,
        0x9f, 0x0, 0x0, 0x3, 0x0,
        0xcf, 0x7, 0x0, 0x3, 0x0,
        0x2, 0x19, 0x0, 0x3, 0x0,
        0x2, 0x7, 0x0, 0x3, 0x0,
        0x86, 0x9, 0x80, 0x1, 0x0,
        0xf8, 0x0, 0xc0, 0x1, 0x0,
        0xf0, 0x8, 0x80, 0xf3, 0x1,
        0x38, 0x4, 0x0, 0xfb, 0x3,
        0x8, 0x3, 0x0, 0x1b, 0x7,
        0x78, 0x60, 0x18, 0x1b, 0x6,
        0x98, 0x63, 0x18, 0x3b, 0xe,
        0x18, 0x6c, 0x18, 0x33, 0xe,
        0xc, 0x70, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x33, 0xc,
        0xc, 0x60, 0x18, 0x1b, 0xc,
        0xc, 0x60, 0x18, 0x1b, 0xc,
        0x1c, 0xe0, 0xc, 0xf, 0xe,
        0x18, 0xd0, 0xf, 0x6, 0x7,
        0x18, 0x8, 0x0, 0xc0, 0x3,
        0x30, 0x7, 0x0, 0xf8, 0x1,
        0xf0, 0x10, 0xc0, 0x7f, 0x0,
        0xe0, 0x23, 0xc0, 0x7, 0x0,
        0xe0, 0x7f, 0x60, 0x0, 0x0,
        0xc0, 0xdf, 0x7f, 0x0, 0x0,
        0x80, 0xf, 0x1f, 0x0, 0x0};


uint8_t BM_ASSETS_CAT_TALK_MASK[] =
{
    0xff, 0xe3, 0x87, 0xff, 0xf,
0xff, 0xc1, 0x1, 0xff, 0xf,
0xff, 0x80, 0x0, 0xff, 0xf,
0x7f, 0x0, 0x0, 0xff, 0xf,
0x3f, 0x0, 0x0, 0xff, 0xf,
0xf, 0x0, 0x0, 0xfe, 0xf,
0xf, 0x0, 0x0, 0xfe, 0xf,
0x7, 0x0, 0x0, 0xfe, 0xf,
0x7, 0x0, 0x0, 0xfe, 0xf,
0x3, 0x0, 0x0, 0xfc, 0xf,
0x3, 0x0, 0x0, 0xfc, 0xf,
0x1, 0x0, 0x0, 0xfc, 0xf,
0x0, 0x0, 0x0, 0xfc, 0xf,
0x0, 0x0, 0x0, 0xfc, 0xf,
0x1, 0x0, 0x0, 0xfc, 0xf,
0x1, 0x0, 0x0, 0xfc, 0xf,
0x1, 0x0, 0x0, 0xfe, 0xf,
0x7, 0x0, 0x0, 0xfe, 0xf,
0xf, 0x0, 0x0, 0xc, 0xe,
0x7, 0x0, 0x0, 0x4, 0xc,
0x7, 0x0, 0x0, 0x4, 0x8,
0x7, 0x0, 0x0, 0x4, 0x8,
0x7, 0x0, 0x0, 0x4, 0x0,
0x7, 0x0, 0x0, 0xc, 0x0,
0x3, 0x0, 0x0, 0xc, 0x0,
0x3, 0x0, 0x0, 0xc, 0x0,
0x3, 0x0, 0x0, 0xc, 0x0,
0x3, 0x0, 0x0, 0xc, 0x0,
0x3, 0x0, 0x0, 0x4, 0x0,
0x3, 0x0, 0x0, 0x4, 0x0,
0x3, 0x0, 0x0, 0x0, 0x0,
0x7, 0x0, 0x0, 0x0, 0x8,
0x7, 0x0, 0x0, 0x0, 0xc,
0xf, 0x0, 0x0, 0x0, 0xe,
0xf, 0x0, 0x0, 0x80, 0xf,
0x1f, 0x0, 0x0, 0xf8, 0xf,
0x1f, 0x0, 0x80, 0xff, 0xf,
0x3f, 0x20, 0x80, 0xff, 0xf,
0x7f, 0xf0, 0xe0, 0xff, 0xf
};