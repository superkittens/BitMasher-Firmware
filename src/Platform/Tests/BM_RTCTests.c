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


#include "BM_RTCTests.h"


static BM_Error BM_RTCTests_HMSToBCDConversionTest()
{
    //  Invalid parameter check
    BM_RTCModule_TimeHMS hmsTime;
    BM_RTCModule_TimeBCD bcdTime;

    //  Null pointer
    BM_TEST_FOR_FAIL(BM_RTCModule_convertHMSToBCD(hmsTime, NULL));

    //  Invalid hour and minute
    hmsTime.hour = 25;
    BM_TEST_FOR_FAIL(BM_RTCModule_convertHMSToBCD(hmsTime, &bcdTime));

    hmsTime.hour = 0;
    hmsTime.minute = 63;
    BM_TEST_FOR_FAIL(BM_RTCModule_convertHMSToBCD(hmsTime, &bcdTime));

    //  Expect correct BCD values
    hmsTime.hour = 10;
    hmsTime.minute = 15;
    hmsTime.second = 33;
    BM_TEST_FOR_PASS(BM_RTCModule_convertHMSToBCD(hmsTime, &bcdTime));

    BM_TEST_EXPECT(bcdTime.hourT == 1);
    BM_TEST_EXPECT(bcdTime.hourU == 0);
    BM_TEST_EXPECT(bcdTime.minuteT == 1);
    BM_TEST_EXPECT(bcdTime.minuteU == 5);
    BM_TEST_EXPECT(bcdTime.secondT == 3);
    BM_TEST_EXPECT(bcdTime.secondU == 3);

    return BM_NO_ERR;
}


static BM_Error BM_RTCTests_BCDToHMSConversionTest()
{
    //  Invalid parameter check
    BM_RTCModule_TimeHMS hmsTime;
    BM_RTCModule_TimeBCD bcdTime;

    //  Null pointer
    BM_TEST_FOR_FAIL(BM_RTCModule_convertBCDToHMS(bcdTime, NULL));

    //  Invalid inputs
    bcdTime.hourT = 3;
    bcdTime.hourU = -2;
    bcdTime.minuteT = 6;
    bcdTime.minuteU = 10;
    bcdTime.secondT = -2;
    bcdTime.secondU = 20;

    BM_TEST_FOR_FAIL(BM_RTCModule_convertBCDToHMS(bcdTime, &hmsTime));
    bcdTime.hourT = 1;
    BM_TEST_FOR_FAIL(BM_RTCModule_convertBCDToHMS(bcdTime, &hmsTime));
    bcdTime.hourU = 0;
    BM_TEST_FOR_FAIL(BM_RTCModule_convertBCDToHMS(bcdTime, &hmsTime));
    bcdTime.minuteT = 1;
    BM_TEST_FOR_FAIL(BM_RTCModule_convertBCDToHMS(bcdTime, &hmsTime));
    bcdTime.minuteU = 5;
    BM_TEST_FOR_FAIL(BM_RTCModule_convertBCDToHMS(bcdTime, &hmsTime));
    bcdTime.secondT = 3;
    BM_TEST_FOR_FAIL(BM_RTCModule_convertBCDToHMS(bcdTime, &hmsTime));
    bcdTime.secondU = 3;

    BM_TEST_FOR_PASS(BM_RTCModule_convertBCDToHMS(bcdTime, &hmsTime));

    BM_TEST_EXPECT(hmsTime.hour == 10);
    BM_TEST_EXPECT(hmsTime.minute == 15);
    BM_TEST_EXPECT(hmsTime.second == 33);
}


BM_Error BM_RTCTests_runUnitTests()
{
    BM_UNIT_TEST_ASSERT(BM_RTCTests_HMSToBCDConversionTest());
    return BM_NO_ERR;
}