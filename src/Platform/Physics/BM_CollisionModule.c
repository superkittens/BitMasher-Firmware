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

#include "BM_CollisionModule.h"


BM_DirectionPair BM_CM_detectCollisions(BM_Sprite *pc, BM_Sprite *npc)
{
  BM_DirectionPair directions;
  directions.first = BM_DIR_NONE;
  directions.second = BM_DIR_NONE;

  uint32_t pcTopEdge = pc->currentYPos + pc->colliderBoundsOriginDy;
  uint32_t pcBottomEdge = pc->currentYPos + pc->colliderBoundsOriginDy + pc->colliderBoundsHeight;
  uint32_t pcLeftEdge = pc->currentXPos + pc->colliderBoundsOriginDx;
  uint32_t pcRightEdge = pc->currentXPos + pc->colliderBoundsOriginDx + pc->colliderBoundsWidth;

  uint32_t npcTopEdge = npc->currentYPos + npc->colliderBoundsOriginDy;
  uint32_t npcBottomEdge = npc->currentYPos + npc->colliderBoundsOriginDy + npc->colliderBoundsHeight;
  uint32_t npcLeftEdge = npc->currentXPos + npc->colliderBoundsOriginDx;
  uint32_t npcRightEdge = npc->currentXPos + npc->colliderBoundsOriginDx + npc->colliderBoundsWidth;

  int32_t pcPrevXPos = (int32_t)pc->prevXPos + pc->colliderBoundsOriginDx;
  int32_t pcPrevYPos = (int32_t)pc->prevYPos + pc->colliderBoundsOriginDy;

  /*
   * To check for collisions, first check to see if there is any overlap between the PC and the NPC.
   * If a general overlap check passes, then find the specific contacting edges
   */
  uint32_t yOverlapCheck =  ((pcTopEdge <= npcBottomEdge) && (pcTopEdge >= npcTopEdge)) ||
                            ((pcBottomEdge >= npcTopEdge) && (pcBottomEdge <= npcBottomEdge));

  uint32_t xOverlapCheck =  ((pcLeftEdge <= npcRightEdge) && (pcLeftEdge >= npcLeftEdge)) ||
                            ((pcRightEdge >= npcLeftEdge) && (pcRightEdge <= npcRightEdge));


  if (yOverlapCheck && xOverlapCheck)
  {
    //  Find the specific edge(s) that contact each other
    uint32_t edgeFlag = 0;

    //  Check for collision at the PC's top
    if (pcTopEdge <= npcBottomEdge && pcTopEdge >= npcTopEdge)
      edgeFlag |= 0x01;

    //  Check for collision at the PC's bottom
    if (pcBottomEdge >= npcTopEdge && pcBottomEdge <= npcBottomEdge)
      edgeFlag |= 0x02;

    //  Check for collision at the PC's left edge
    if (pcLeftEdge <= npcRightEdge && pcLeftEdge >= npcLeftEdge)
        edgeFlag |= 0x04;

    //  Check for collision at the PC's right side
    if (npcRightEdge >= npcLeftEdge && pcRightEdge <= npcRightEdge)
      edgeFlag |= 0x08;

    switch (edgeFlag)
    {
      //  Top Collisions
      case 0x01:
      {
        directions.first = BM_DIR_UP;
        directions.second = BM_DIR_DOWN;
        break;
      }

      case 0x0D:
      {
        directions.first = BM_DIR_UP;
        directions.second = BM_DIR_DOWN;
        break;
      }

      //  Bottom Collisions
      case 0x02:
      {
        directions.first = BM_DIR_DOWN;
        directions.second = BM_DIR_UP;
        break;
      }

      case 0x0E:
      {
        directions.first = BM_DIR_DOWN;
        directions.second = BM_DIR_UP;
        break;
      }

      //  Left Collisions
      case 0x04:
      {
        directions.first = BM_DIR_LEFT;
        directions.second = BM_DIR_RIGHT;
        break;
      }

      case 0x07:
      {
        directions.first = BM_DIR_LEFT;
        directions.second = BM_DIR_RIGHT;
        break;
      }

      //  Right Collisions
      case 0x08:
      {
        directions.first = BM_DIR_RIGHT;
        directions.second = BM_DIR_LEFT;
        break;
      }

      case 0x0B:
      {
        directions.first = BM_DIR_RIGHT;
        directions.second = BM_DIR_LEFT;
        break;
      }

      /*
       * Handle corner collisions
       *
       * These are FIENDISHLY tricky to sort out (at least, for me)
       * The main problem to figure out with corner collisions is determining which edge struck first
       * We are using the previous sprite's location to try to predict the first colliding edge
       * and then assigning the collision location to that edge - instead of having a dedicated
       * corner as the return value.
       */

      //  Top Left Collision
      case 0x05:
      {
        directions.first = BM_DIR_LEFT;
        directions.second = BM_DIR_RIGHT;

        if (pcPrevXPos < npcRightEdge)
        {
            directions.first = BM_DIR_UP;
            directions.second = BM_DIR_DOWN;
        }

        break;
      }

      //  Top Right Collision
      case 0x09:
      {
        directions.first = BM_DIR_RIGHT;
        directions.second = BM_DIR_LEFT;

        if (pcPrevXPos + pc->colliderBoundsWidth > npcLeftEdge)
        {
          directions.first = BM_DIR_UP;
          directions.second = BM_DIR_DOWN;
        }

        break;
      }

      //  Bottom Left Collision
      case 0x06:
      {
        directions.first = BM_DIR_LEFT;
        directions.second = BM_DIR_RIGHT;

        if (pcPrevXPos < npcRightEdge)
        {
          directions.first = BM_DIR_DOWN;
          directions.second = BM_DIR_UP;
        }

        break;
      }

      //  Bottom Right Collision
      case 0x0A:
      {
        directions.first = BM_DIR_RIGHT;
        directions.second = BM_DIR_LEFT;

        if (pcPrevXPos + npc->colliderBoundsWidth > npcLeftEdge)
        {
          directions.first = BM_DIR_DOWN;
          directions.second = BM_DIR_UP;
        }

        break;
      }

      //  PC inside sprite
      //  Do we really need this??
      case 0x0F:
      {
        directions.first = BM_DIR_INSIDE;
        directions.second = BM_DIR_INSIDE;
        break;
      }

      default:
        break;
    }
  }

  return directions;
}
