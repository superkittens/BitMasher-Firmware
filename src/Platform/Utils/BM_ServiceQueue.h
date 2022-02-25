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

#ifndef SRC_BM_SERVICEQUEUE_H_
#define SRC_BM_SERVICEQUEUE_H_

#include "arm_math.h"
#include "BM_ErrorCodes.h"
#include "BM_DebugServices.h"

#define BM_SERVICE_QUEUE_NUM_ITEMS 10

typedef enum
{
  BM_SERVICE_UI     = 0,
  BM_SERVICE_AUDIO,
  BM_SERVICE_UNKNOWN
}BM_ServiceItemType;


typedef struct
{
  BM_ServiceItemType itemType;
  void *data;

}BM_ServiceQueueItem;


void        BM_ServiceQueue_addItemToServiceQueue(BM_ServiceItemType itemType, void *data);
BM_Error    BM_ServiceQueue_getItemFromServiceQueue(BM_ServiceQueueItem *item);
//int32_t     isServiceQueueEmpty();

#endif /* SRC_BM_SERVICEQUEUE_H_ */
