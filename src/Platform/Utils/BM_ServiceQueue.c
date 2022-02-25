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


#include "BM_ServiceQueue.h"

volatile static BM_ServiceQueueItem serviceQueue[BM_SERVICE_QUEUE_NUM_ITEMS];
volatile static uint32_t queueHead = 0;
volatile static uint32_t queueTail = 0;
volatile static uint32_t numItemsInQueue = 0;


void BM_ServiceQueue_addItemToServiceQueue(BM_ServiceItemType itemType, void *data)
{
  //  If queue is full, simply exit
  if (numItemsInQueue < BM_SERVICE_QUEUE_NUM_ITEMS)
  {
    serviceQueue[queueTail].itemType = itemType;
    serviceQueue[queueTail].data = data;

    numItemsInQueue++;

    queueTail = (queueTail + 1) % BM_SERVICE_QUEUE_NUM_ITEMS;
  }
}


BM_Error BM_ServiceQueue_getItemFromServiceQueue(BM_ServiceQueueItem *item)
{
  if (numItemsInQueue > 0)
  {
    item->itemType = serviceQueue[queueHead].itemType;
    item->data = serviceQueue[queueHead].data;

    numItemsInQueue--;

    queueHead = (queueHead + 1) % BM_SERVICE_QUEUE_NUM_ITEMS;

    return BM_NO_ERR;
  }

  return BM_SERVICE_QUEUE_QUEUE_EMPTY;
}
