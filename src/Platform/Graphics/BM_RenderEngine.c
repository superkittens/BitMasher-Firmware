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

#include "BM_RenderEngine.h"

#define BM_RE_NUM_FRAME_BUFFERS 2

volatile MW_LS013B7DH03_Params _displayParams;
static uint8_t frameBuffer[BM_RE_NUM_FRAME_BUFFERS][BM_DISPLAY_WIDTH * BM_DISPLAY_HEIGHT / 8];

static uint32_t displayInitialized = 0;

//  Used for drawing rectangles (and maybe other shapes)
static uint8_t _displayRowBuffer[8 * (((BM_DISPLAY_WIDTH - 1) / 8) + 1)];

static int32_t _currentFrameBuffer = 0;



static void BM_RE_setupTimer(float32_t frameRate)
{
  TIMER_Init_TypeDef init = TIMER_INIT_DEFAULT;

  //  Don't start the timer after initialization yet
  init.enable = false;
  init.prescale = timerPrescale1024;

  TIMER_Init(TIMER1, &init);

  //  Calculate the number of pulses needed for each sampling period
  float32_t samplingPeriod = 1 / frameRate;
  float32_t numPulsesf = samplingPeriod * ((float32_t)CMU_ClockFreqGet(cmuClock_TIMER1) / 1024.f);
  uint32_t numPulses = (uint32_t)numPulsesf;

  TIMER_TopSet(TIMER1, numPulses);

  NVIC_EnableIRQ(TIMER1_IRQn);
  TIMER_IntEnable(TIMER1, TIMER_IF_OF);
}



BM_Error BM_RE_init(float32_t frameRate)
{
  //  Enable 2V5 to 3V0 level shifter IC
  #ifndef HW_P1
  GPIO_PinModeSet(BM_DISP_ENABLE_PORT, BM_DISP_ENABLE_PIN, gpioModePushPull, 1);
  GPIO_PinOutClear(BM_DISP_ENABLE_PORT, BM_DISP_ENABLE_PIN);
  #endif

  _displayParams.comExtPort = gpioPortD;
  _displayParams.comExtPin = 13;
  _displayParams.comExtLoc = LETIMER_ROUTELOC0_OUT0LOC_LOC21;
  _displayParams.usartModule = USART1;
  _displayParams.spiSCKPort = gpioPortC;
  _displayParams.spiSCKPin = 8;
  _displayParams.spiSCKLoc = USART_ROUTELOC0_CLKLOC_LOC11;
  _displayParams.spiCSPort = gpioPortD;
  _displayParams.spiCSPin = 14;
  _displayParams.spiCSLoc = USART_ROUTELOC0_CSLOC_LOC19;
  _displayParams.spiCOPIPort = gpioPortC;
  _displayParams.spiCOPIPin = 6;
  _displayParams.spiCOPILoc = USART_ROUTELOC0_TXLOC_LOC11;
  _displayParams.ldmaChannel = BM_LDMA_CH_DISP;
  _displayParams.VCOMTimer = LETIMER0;
  _displayParams.VCOMLETimerOutputPEN = LETIMER_ROUTEPEN_OUT0PEN;
  _displayParams.signallingTimer = TIMER0;
  _displayParams.signallingTimerIRQnType = TIMER0_IRQn;
  _displayParams.signallingTimerClock = cmuClock_TIMER0;
  

  int32_t success = MW_LS013B7DH03_init((MW_LS013B7DH03_Params*)&_displayParams);
  MW_LS013B7DH03_start((MW_LS013B7DH03_Params*)&_displayParams);

  BM_ISR_LDMA_registerDisplayISR(BM_RE_displayUpdateISR);
  BM_ISR_Timer_registerTIMER0ISR(BM_RE_displaySignallingTimerISR);
  BM_ISR_Timer_registerTIMER1ISR(BM_RE_VSYNCISR);

  displayInitialized = 1;
  _currentFrameBuffer = 0;
  
  BM_RE_setupTimer(frameRate);

  return BM_NO_ERR;
}



//  Start the timer that will send BM_SERVICE_UI messages into the queue
BM_Error BM_RE_startVSYNC()
{
  if (displayInitialized == 0)
    return BM_RE_NOT_INITIALIZED;

  BM_RE_setupTimer(FRAME_RATE);
  TIMER_Enable(TIMER1, true);

  return BM_NO_ERR;
}


//  Stop the timer that will send BM_SERVICE_UI messages into the queue
BM_Error BM_RE_stopRenderEngine()
{
  TIMER_Enable(TIMER1, false);
  MW_LS013B7DH03_stop(&_displayParams);

  return BM_NO_ERR;
}


void BM_RE_drawBackground(uint8_t *backgroundBitmap)
{
  if (backgroundBitmap == NULL)
    memset(frameBuffer, 0xFF, BM_DISPLAY_WIDTH * BM_DISPLAY_HEIGHT / 8);

  memcpy(&frameBuffer[_currentFrameBuffer], backgroundBitmap, sizeof(uint8_t) * BM_DISPLAY_WIDTH * BM_DISPLAY_HEIGHT / 8);
}


void BM_RE_drawLightBackground()
{
  memset(&frameBuffer[_currentFrameBuffer], 0xFF, BM_DISPLAY_WIDTH * BM_DISPLAY_HEIGHT / 8);
}


void BM_RE_drawDarkBackground()
{
  memset(&frameBuffer[_currentFrameBuffer], 0x00, BM_DISPLAY_WIDTH * BM_DISPLAY_HEIGHT / 8);
}


void BM_RE_drawSprite(uint8_t *spriteBitmap, uint8_t *spriteMask, int32_t xPos, int32_t yPos, int32_t width, int32_t height)
{
  BM_RE_drawSpriteOptimized(spriteBitmap, spriteMask, xPos, yPos, width, height);
}


void BM_RE_drawPixel(const int32_t xPos, const int32_t yPos, const int32_t colour)
{
  if (xPos < 0 || xPos >= BM_DISPLAY_WIDTH)
    return;

  if (yPos < 0 || yPos >= BM_DISPLAY_HEIGHT)
    return;

  if (colour < 0 || colour > 1)
    return;

  //  Get the frame buffer byte associated with (xPos, yPos)
  const int32_t NUM_BYTES_PER_LINE = ((BM_DISPLAY_WIDTH - 1) / 8) + 1;
  const int32_t byteIndex = (yPos * NUM_BYTES_PER_LINE) + (xPos / 8);

  uint8_t * const frameByte = &frameBuffer[_currentFrameBuffer][byteIndex];

  //  Draw black pixel if 1, white otherwise
  if (colour)
    *frameByte &= (0xFF ^ (1 << (xPos % 8)));
  
  else
    *frameByte |= (1 << (xPos % 8));
}


void BM_RE_drawTime(const BM_RTCModule_TimeBCD time, const int32_t yPos)
{
      //  Hour Digits
    BM_RE_drawSprite(BM_ASSETS_CLOCK_DIGITS[time.hourT],
                    NULL,
                    4,
                    yPos,
                    BM_ASSETS_CLOCK_DIGIT_WIDTH,
                    BM_ASSETS_CLOCK_DIGIT_HEIGHT
    );

    BM_RE_drawSprite(BM_ASSETS_CLOCK_DIGITS[time.hourU],
                    NULL,
                    32,
                    yPos,
                    BM_ASSETS_CLOCK_DIGIT_WIDTH,
                    BM_ASSETS_CLOCK_DIGIT_HEIGHT
    );

    //  Colon Separator
    BM_RE_drawRectangleFilled(60, yPos + 5, 8, 8);
    BM_RE_drawRectangleFilled(60, yPos + 32 - 5, 8, 8);

    //  Minute Digits
    BM_RE_drawSprite(BM_ASSETS_CLOCK_DIGITS[time.minuteT],
                    NULL,
                    72,
                    yPos,
                    BM_ASSETS_CLOCK_DIGIT_WIDTH,
                    BM_ASSETS_CLOCK_DIGIT_HEIGHT
    );

    BM_RE_drawSprite(BM_ASSETS_CLOCK_DIGITS[time.minuteU],
                    NULL,
                    100,
                    yPos,
                    BM_ASSETS_CLOCK_DIGIT_WIDTH,
                    BM_ASSETS_CLOCK_DIGIT_HEIGHT
    );
}


void BM_RE_updateDisplay(const bool asyncWrite)
{
  if (asyncWrite)
    MW_LS013B7DH03_drawAsync((MW_LS013B7DH03_Params*)&_displayParams, (uint8_t*)&frameBuffer[_currentFrameBuffer]);
  else
    MW_LS013B7DH03_drawBlocking((MW_LS013B7DH03_Params*)&_displayParams, (uint8_t*)&frameBuffer[_currentFrameBuffer]);
    
  _currentFrameBuffer = (_currentFrameBuffer + 1) % BM_RE_NUM_FRAME_BUFFERS;
}


uint8_t* BM_RE_getFrameBuffer()
{
  return (uint8_t*)&frameBuffer[_currentFrameBuffer];
}


void BM_RE_drawRectangleFilled(int32_t x, int32_t y, int32_t width, int32_t height)
{
  if (width <= 0 || height <= 0)
    return;

  uint8_t byte = 0xFF;
  int32_t numBytes = ((width - 1) / 8) + 1;

  //  Create the last 'block' of pixels to draw for one row
  uint8_t lastByte = 0;
  int32_t remainingPixels = width - ((numBytes - 1) * 8);
  for (int32_t i = 0; i < remainingPixels; ++i)
    lastByte |= (1 << i);

  //  Draw the rectangle one row at a time
  for (int32_t row = 0; row < height; ++row)
  {
    //  When drawing a row, draw 8 columns (byte) at a time
    for (int32_t block = 0; block < numBytes - 1; ++block)
      BM_RE_drawSprite(&byte, NULL, x + (block * 8), y + row, 8, 1);

    BM_RE_drawSprite(&lastByte, NULL, x + ((numBytes - 1) * 8), y + row, remainingPixels, 1);
  }
}


void BM_RE_drawRectangleNotFilled(const int32_t x, const int32_t y, const int32_t width, const int32_t height)
{
  if (width <= 0 || height <= 0)
    return;

  int32_t numBytesPerRow = ((width - 1) / 8) + 1;
  
  //  Draw top and bottom edges
  for (int32_t i = 0; i < numBytesPerRow; ++i)
    _displayRowBuffer[i] = 0xFF;

  BM_RE_drawSprite(_displayRowBuffer, NULL, x, y, width, 1);
  BM_RE_drawSprite(_displayRowBuffer, NULL, x, y + height - 1, width, 1);

  //  Draw the sides
  memset(_displayRowBuffer, 0, numBytesPerRow);
  _displayRowBuffer[0] = 0x01;
  _displayRowBuffer[numBytesPerRow - 1] |= (1 << (8 - ((numBytesPerRow * 8) - width) - 1));

  for (int32_t i = 1; i < height - 1; ++i)
    BM_RE_drawSprite(_displayRowBuffer, NULL, x, y + i, width, 1);
}


void BM_RE_drawString(const int32_t xPos, const int32_t yPos, const char* str)
{
  //  For now, we will constrain any text to within the display bounds
  //  It would be nice to eventually have the option to be able to write text at xPos/yPos < 0
  if (xPos < 0 || xPos >= BM_DISPLAY_WIDTH)
    return;
  
  if (yPos < 0 || yPos >= BM_DISPLAY_HEIGHT)
    return;

  if (str == NULL)
    return;

  int32_t index = 0;
  char c = str[index];

  while (c != '\0')
  {
    for (int32_t col = 0; col < FONT_WIDTH; ++col)
    {
      uint8_t byte = fontBitmap[c * 5 + col];
      for (int32_t row = 0; row < FONT_HEIGHT + 1; ++row)
      {
        if (byte & 0x01)
          BM_RE_drawPixel(xPos + ((FONT_WIDTH + 1) * index) + col, yPos + row, 1);
        
        byte = byte >> 1;
      }
    }

    c = str[++index];
  }
}


/*
 *  Draw 8 pixels at a time instead of each pixel individually
 */
void BM_RE_drawSpriteOptimized(uint8_t * const spriteBitmap, uint8_t * const spriteMask, int32_t xPos, int32_t yPos, int32_t width, int32_t height)
{
  if (spriteBitmap == NULL)
    return;

  if (xPos + width < 0 || xPos >= BM_DISPLAY_WIDTH)
    return;

  if (yPos + height < 0 || yPos >= BM_DISPLAY_HEIGHT)
    return;

  if (width == 0 || height == 0)
    return;

  if (spriteMask == NULL)
    BM_RE_drawSpriteOptimizedNoMask(spriteBitmap, xPos, yPos, width, height);

  const int32_t BLOCK_SIZE = 8;
  const int32_t NUM_SPRITE_BYTES_PER_LINE = ((width - 1) / BLOCK_SIZE) + 1;

  //  These parameters are initially set if the sprite is positioned within the display bounds
  //  They will change if the sprite is positioned outside the bounds!
  int32_t effectiveWidth = width;
  int32_t effectiveHeight = height;
  int32_t effectiveNumBytesPerLine = NUM_SPRITE_BYTES_PER_LINE;
  int32_t effectiveXPos = xPos;
  int32_t effectiveYPos = yPos;

  //  If xPos < 0, the sprite no longer is drawn starting at pixel 0!
  int32_t spriteByteOffset = 0;
  int32_t spritePixelOffset = 0;
  int32_t spriteYOffset = 0;

  if (xPos < 0)
  {
    effectiveWidth = width + xPos;
    effectiveXPos = 0;
    effectiveNumBytesPerLine = ((effectiveWidth - 1) / BLOCK_SIZE) + 1;
    spriteByteOffset = -xPos / BLOCK_SIZE;
    spritePixelOffset = -xPos % BLOCK_SIZE;
  }

  if (xPos + width >= BM_DISPLAY_WIDTH)
  {
    effectiveWidth = BM_DISPLAY_WIDTH - xPos;
    effectiveNumBytesPerLine = ((effectiveWidth - 1) / BLOCK_SIZE) + 1;
  }

  if (yPos < 0)
  {
    effectiveHeight = effectiveHeight + yPos;
    effectiveYPos = 0;
    spriteYOffset = -yPos;
  }

  if (yPos + height >= BM_DISPLAY_HEIGHT)
    effectiveHeight = BM_DISPLAY_HEIGHT - yPos;


  //  If xPos is divisible by 8, we have an easier time writing the sprite to the frame buffer
  if (((xPos % 8) == 0) || (xPos < 0))
  {
    for (int32_t y = 0; y < effectiveHeight; ++y)
    {
      int32_t numPixelsInRowRemaining = effectiveWidth;
      int32_t spriteByteIndex = ((y + spriteYOffset) * NUM_SPRITE_BYTES_PER_LINE) + spriteByteOffset;

      for (int32_t block = 0; block < effectiveNumBytesPerLine - 1; ++block)
      {
        int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + (((block * BLOCK_SIZE) + effectiveXPos) / 8);
        uint8_t spriteByte = ((spriteBitmap[spriteByteIndex + 1] << 8 | spriteBitmap[spriteByteIndex]) >> spritePixelOffset) ^ 0xFF;
        uint8_t maskByte = (spriteMask[spriteByteIndex + 1] << 8 | spriteMask[spriteByteIndex]) >> spritePixelOffset;
        uint8_t frameByte = frameBuffer[_currentFrameBuffer][frameBufferIndex];

        //  Apply sprite mask to spriteByte
        uint8_t maskedSpriteByte = spriteByte & (maskByte ^ 0xFFFF);
        uint8_t maskedFrameByte = frameByte & maskByte;

        frameBuffer[_currentFrameBuffer][frameBufferIndex] = maskedSpriteByte | maskedFrameByte;

        numPixelsInRowRemaining -= BLOCK_SIZE;
        spriteByteIndex++;
      }

      //  Draw last remaining pixels (if any)
      int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + ((((effectiveNumBytesPerLine - 1) * BLOCK_SIZE) + effectiveXPos) / 8);

      uint8_t spriteByte;
      uint8_t maskByte;

      if (spritePixelOffset != 0)
      {
        spriteByte = ((spriteBitmap[spriteByteIndex + 1] << 8 | spriteBitmap[spriteByteIndex]) >> spritePixelOffset) ^ 0xFF;
        maskByte = (spriteMask[spriteByteIndex + 1] << 8 | spriteMask[spriteByteIndex]) >> spritePixelOffset;   
      }
      
      else
      {
        spriteByte = (spriteBitmap[spriteByteIndex] >> spritePixelOffset) ^ 0xFF;
        maskByte = spriteMask[spriteByteIndex] >> spritePixelOffset;
      }

      maskByte = maskByte | (0xFF << numPixelsInRowRemaining); 

      uint8_t frameByte = frameBuffer[_currentFrameBuffer][frameBufferIndex];

      uint8_t maskByteInverted = maskByte ^ 0xFF;
      spriteByte = spriteByte & maskByteInverted;
      frameByte = frameByte & maskByte;

      frameBuffer[_currentFrameBuffer][frameBufferIndex] = spriteByte | frameByte;
    }
  }
  //  If xPos is NOT divisible by BLOCK_SIZE, then we have some more work to do
  else 
  {
    const int32_t numBitsToShift = xPos - ((xPos / BLOCK_SIZE) * BLOCK_SIZE);
    for (int32_t y = 0; y < effectiveHeight; ++y)
    {
      int32_t numPixelsInRowRemaining = effectiveWidth;
      uint16_t frameBufferMask = (0x00FF << numBitsToShift) ^ 0xFFFF;

      for (int32_t block = 0; block < effectiveNumBytesPerLine - 1; ++block)
      {
        int32_t spriteByteIndex = ((y + spriteYOffset) * NUM_SPRITE_BYTES_PER_LINE) + ((block * BLOCK_SIZE) / 8);
        int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + (((block * BLOCK_SIZE) + effectiveXPos) / 8);

        uint16_t spriteData = (spriteBitmap[spriteByteIndex] ^ 0xFF) << numBitsToShift;
        uint8_t spriteMaskData = spriteMask[spriteByteIndex];
        uint16_t Ms = (spriteMaskData ^ 0xFF) << numBitsToShift;
        uint16_t Mf = spriteMaskData << numBitsToShift;

        uint16_t frameBufferData = (frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] << 8) | frameBuffer[_currentFrameBuffer][frameBufferIndex];

        uint16_t A = spriteData & Ms;
        uint16_t B = frameBufferData & Mf;
        uint16_t C = A | B;

        frameBuffer[_currentFrameBuffer][frameBufferIndex] = C | (frameBufferData & frameBufferMask);
        frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] = (C | (frameBufferData & frameBufferMask)) >> 8;

        numPixelsInRowRemaining -= BLOCK_SIZE;
      }

        int32_t spriteByteIndex = ((y + spriteYOffset) * NUM_SPRITE_BYTES_PER_LINE) + (((effectiveNumBytesPerLine - 1) * BLOCK_SIZE) / 8);
        int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + ((((effectiveNumBytesPerLine - 1) * BLOCK_SIZE) + effectiveXPos) / 8);

        uint16_t spriteData = (spriteBitmap[spriteByteIndex] ^ 0xFF) << numBitsToShift;
        uint8_t spriteMaskData = spriteMask[spriteByteIndex];
        spriteMaskData |= (0xFF << numPixelsInRowRemaining);
        uint16_t Ms = (spriteMaskData ^ 0xFF) << numBitsToShift;
        uint16_t Mf = spriteMaskData << numBitsToShift;

        uint16_t frameBufferData = (frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] << 8) | frameBuffer[_currentFrameBuffer][frameBufferIndex];

        uint16_t A = spriteData & Ms;
        uint16_t B = frameBufferData & Mf;
        uint16_t C = A | B;

        frameBuffer[_currentFrameBuffer][frameBufferIndex] = C | (frameBufferData & frameBufferMask);
        frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] = (C | (frameBufferData & frameBufferMask)) >> 8;
    }
  }
}


void BM_RE_drawSpriteOptimizedNoMask(uint8_t * const spriteBitmap, int32_t xPos, int32_t yPos, int32_t width, int32_t height)
{
  if (spriteBitmap == NULL)
    return;

  if (xPos + width < 0 || xPos >= BM_DISPLAY_WIDTH)
    return;

  if (yPos + height < 0 || yPos >= BM_DISPLAY_HEIGHT)
    return;

  if (width == 0 || height == 0)
    return;

  const int32_t BLOCK_SIZE = 8;
  const int32_t NUM_SPRITE_BYTES_PER_LINE = ((width - 1) / BLOCK_SIZE) + 1;

  //  These parameters are initially set if the sprite is positioned within the display bounds
  //  They will change if the sprite is positioned outside the bounds!
  int32_t effectiveWidth = width;
  int32_t effectiveHeight = height;
  int32_t effectiveNumBytesPerLine = NUM_SPRITE_BYTES_PER_LINE;
  int32_t effectiveXPos = xPos;
  int32_t effectiveYPos = yPos;

  //  If xPos < 0, the sprite no longer is drawn starting at pixel 0!
  int32_t spriteByteOffset = 0;
  int32_t spritePixelOffset = 0;
  int32_t spriteYOffset = 0;

  if (xPos < 0)
  {
    effectiveWidth = width + xPos;
    effectiveXPos = 0;
    effectiveNumBytesPerLine = ((effectiveWidth - 1) / BLOCK_SIZE) + 1;
    spriteByteOffset = -xPos / BLOCK_SIZE;
    spritePixelOffset = -xPos % BLOCK_SIZE;
  }

  if (xPos + width >= BM_DISPLAY_WIDTH)
  {
    effectiveWidth = BM_DISPLAY_WIDTH - xPos;
    effectiveNumBytesPerLine = ((effectiveWidth - 1) / BLOCK_SIZE) + 1;
  }

  if (yPos < 0)
  {
    effectiveHeight = effectiveHeight + yPos;
    effectiveYPos = 0;
    spriteYOffset = -yPos;
  }

  if (yPos + height >= BM_DISPLAY_HEIGHT)
    effectiveHeight = BM_DISPLAY_HEIGHT - yPos;


  //  If xPos is divisible by 8, we have an easier time writing the sprite to the frame buffer
  if (((xPos % 8) == 0) || (xPos < 0))
  {
    for (int32_t y = 0; y < effectiveHeight; ++y)
    {
      int32_t numPixelsInRowRemaining = effectiveWidth;
      int32_t spriteByteIndex = ((y + spriteYOffset) * NUM_SPRITE_BYTES_PER_LINE) + spriteByteOffset;

      for (int32_t block = 0; block < effectiveNumBytesPerLine - 1; ++block)
      {
        int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + (((block * BLOCK_SIZE) + effectiveXPos) / 8);
        uint8_t spriteByte = ((spriteBitmap[spriteByteIndex + 1] << 8 | spriteBitmap[spriteByteIndex]) >> spritePixelOffset) ^ 0xFF;

        frameBuffer[_currentFrameBuffer][frameBufferIndex] = spriteByte;

        numPixelsInRowRemaining -= BLOCK_SIZE;
        spriteByteIndex++;
      }

      //  Draw last remaining pixels (if any)
      int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + ((((effectiveNumBytesPerLine - 1) * BLOCK_SIZE) + effectiveXPos) / 8);

      uint8_t spriteByte;
      uint8_t maskByte;

      if (spritePixelOffset != 0)
        spriteByte = ((spriteBitmap[spriteByteIndex + 1] << 8 | spriteBitmap[spriteByteIndex]) >> spritePixelOffset) ^ 0xFF;   
      
      else
        spriteByte = (spriteBitmap[spriteByteIndex] >> spritePixelOffset) ^ 0xFF;

      spriteByte &= (0xFF >> (BLOCK_SIZE - numPixelsInRowRemaining));

      uint8_t frameByte = (frameBuffer[_currentFrameBuffer][frameBufferIndex]) & (0xFF << numPixelsInRowRemaining);
      frameBuffer[_currentFrameBuffer][frameBufferIndex] = spriteByte | frameByte;
    }
  }
  //  If xPos is NOT divisible by BLOCK_SIZE, then we have some more work to do
  else 
  {
    const int32_t numBitsToShift = xPos - ((xPos / BLOCK_SIZE) * BLOCK_SIZE);
    for (int32_t y = 0; y < effectiveHeight; ++y)
    {
      int32_t numPixelsInRowRemaining = effectiveWidth;
      uint16_t frameBufferMask = (0x00FF << numBitsToShift) ^ 0xFFFF;

      for (int32_t block = 0; block < effectiveNumBytesPerLine - 1; ++block)
      {
        int32_t spriteByteIndex = ((y + spriteYOffset) * NUM_SPRITE_BYTES_PER_LINE) + ((block * BLOCK_SIZE) / 8);
        int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + (((block * BLOCK_SIZE) + effectiveXPos) / 8);

        uint16_t spriteData = (spriteBitmap[spriteByteIndex] ^ 0xFF) << numBitsToShift;
        uint16_t frameBufferData = (frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] << 8) | frameBuffer[_currentFrameBuffer][frameBufferIndex];

        frameBuffer[_currentFrameBuffer][frameBufferIndex] = spriteData | (frameBufferData & frameBufferMask);
        frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] = (spriteData | (frameBufferData & frameBufferMask)) >> 8;

        numPixelsInRowRemaining -= BLOCK_SIZE;
      }

        int32_t spriteByteIndex = ((y + spriteYOffset) * NUM_SPRITE_BYTES_PER_LINE) + (((effectiveNumBytesPerLine - 1) * BLOCK_SIZE) / 8);
        int32_t frameBufferIndex = ((y + effectiveYPos) * 16) + ((((effectiveNumBytesPerLine - 1) * BLOCK_SIZE) + effectiveXPos) / 8);

        uint16_t spriteData = (spriteBitmap[spriteByteIndex] ^ 0xFF);
        spriteData &= (0xFF >> (BLOCK_SIZE - numPixelsInRowRemaining));
        spriteData = spriteData << numBitsToShift;

        uint16_t frameBufferData = (frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] << 8) | frameBuffer[_currentFrameBuffer][frameBufferIndex];

        frameBufferMask |= (0xFF << (numBitsToShift + numPixelsInRowRemaining));

        frameBuffer[_currentFrameBuffer][frameBufferIndex] = spriteData | (frameBufferData & frameBufferMask);
        frameBuffer[_currentFrameBuffer][frameBufferIndex + 1] = (spriteData | (frameBufferData & frameBufferMask)) >> 8;
    }
  }
}


/*
 *  The following drawSprite functions are brute-force implmentations of writing pixels to the frame buffer
 *  ie. The frame buffer is modified pixel by pixel rather than by blocks of pixels
 *  Drawing sprites this way is quite slow but it worked well during the proof of concept stage of the project
 *  I will keep the code here for study or debugging purposes
 */
void BM_RE_drawSpriteBruteForceNoMask(uint8_t *spriteBitmap, int32_t xPos, int32_t yPos, int32_t width, int32_t height)
{
  if (spriteBitmap == NULL)
    return;
    
  if (width == 0 || height == 0)
    return;

  if (xPos + width < 0 || xPos >= BM_DISPLAY_WIDTH)
    return;

  if (yPos + height < 0 || yPos >= BM_DISPLAY_HEIGHT)
    return;
  
  int32_t spriteOriginXOffset = 0;
  int32_t spriteOriginYOffset = 0;
  int32_t endX = width;
  int32_t endY = height;
  int32_t frameBufferStartX = xPos;
  int32_t frameBufferStartY = yPos;

  //  Some bounds checking here
  //  If part of the sprite is outside the display then we need to adjust how many pixels
  //  across or down we need to write.
  //  Moreover, if xPos or yPos is < 0, then we need to clip those values to 0 (assigned to frameBufferStartX/Y)
  //  in order to find which byte in the frame buffer needs to be modified
  if (xPos < 0)
  {
    spriteOriginXOffset = -xPos;
    frameBufferStartX = 0;
    endX = width + xPos;
  }

  if (yPos < 0)
  {
    spriteOriginYOffset = -yPos;
    frameBufferStartY = 0;
    endY = height + yPos;
  }

  if ((xPos + width) > BM_DISPLAY_WIDTH)
    endX = BM_DISPLAY_WIDTH - xPos;

  if ((yPos + height) > BM_DISPLAY_HEIGHT)
    endY = BM_DISPLAY_HEIGHT - yPos;

  if (displayInitialized == 1)
  {
    uint32_t bytesPerLine = ((width - 1) / 8) + 1;

    //  Write sprite data to the frame buffer
    for (uint32_t y = 0; y < endY; y++)
    {
      for (uint32_t x = 0; x < endX; x++)
      {
        int32_t index = (bytesPerLine * (y + spriteOriginYOffset)) + ((spriteOriginXOffset + x) / 8);
        uint8_t spriteByte = spriteBitmap[index] ^ 0xFF;
        uint8_t spriteXOffset = (x + spriteOriginXOffset) % 8;

        int32_t frameBufferIndex = ((x + frameBufferStartX) / 8) + ((y + frameBufferStartY) * 16);
        uint8_t *frameBufferByte = &frameBuffer[_currentFrameBuffer][frameBufferIndex];
        uint8_t frameBufferXOffset = (x + frameBufferStartX) % 8;

        if (spriteByte & (1 << spriteXOffset))
          *frameBufferByte |= (1 << frameBufferXOffset);

        else
          *frameBufferByte &= (0xFF ^ (1 << frameBufferXOffset));
      }
    }
  }
}


void BM_RE_drawSpriteBruteForce(uint8_t *spriteBitmap, uint8_t *spriteMask, int32_t xPos, int32_t yPos, int32_t width, int32_t height)
{
  if (spriteBitmap == NULL)
    return;
    
  if (width == 0 || height == 0)
    return;

  if (xPos + width < 0 || xPos >= BM_DISPLAY_WIDTH)
    return;

  if (yPos + height < 0 || yPos >= BM_DISPLAY_HEIGHT)
    return;

  if (spriteMask == NULL)
    return BM_RE_drawSpriteBruteForceNoMask(spriteBitmap, xPos, yPos, width, height);
  

  int32_t spriteOriginXOffset = 0;
  int32_t spriteOriginYOffset = 0;
  int32_t endX = width;
  int32_t endY = height;
  int32_t frameBufferStartX = xPos;
  int32_t frameBufferStartY = yPos;

  //  Some bounds checking here
  //  If part of the sprite is outside the display then we need to adjust how many pixels
  //  across or down we need to write.
  //  Moreover, if xPos or yPos is < 0, then we need to clip those values to 0 (assigned to frameBufferStartX/Y)
  //  in order to find which byte in the frame buffer needs to be modified
  if (xPos < 0)
  {
    spriteOriginXOffset = -xPos;
    frameBufferStartX = 0;
    endX = width + xPos;
  }

  if (yPos < 0)
  {
    spriteOriginYOffset = -yPos;
    frameBufferStartY = 0;
    endY = height + yPos;
  }

  if ((xPos + width) > BM_DISPLAY_WIDTH)
    endX = BM_DISPLAY_WIDTH - xPos;

  if ((yPos + height) > BM_DISPLAY_HEIGHT)
    endY = BM_DISPLAY_HEIGHT - yPos;


  if (displayInitialized == 1)
  {
    uint32_t bytesPerLine = ((width - 1) / 8) + 1;

    //  Write sprite data to the frame buffer
    for (uint32_t y = 0; y < endY; y++)
    {
      for (uint32_t x = 0; x < endX; x++)
      {
        int32_t index = (bytesPerLine * (y + spriteOriginYOffset)) + ((spriteOriginXOffset + x) / 8);
        uint8_t spriteByte = spriteBitmap[index] ^ 0xFF;
        uint8_t spriteMaskByte = spriteMask[index];
        uint8_t spriteXOffset = (x + spriteOriginXOffset) % 8;

        int32_t frameBufferIndex = ((x + frameBufferStartX) / 8) + ((y + frameBufferStartY) * 16);
        uint8_t *frameBufferByte = &frameBuffer[_currentFrameBuffer][frameBufferIndex];
        uint8_t frameBufferXOffset = (x + frameBufferStartX) % 8;

        if (!(spriteMaskByte & (1 << spriteXOffset)))
        {
          if (spriteByte & (1 << spriteXOffset))
            *frameBufferByte |= (1 << frameBufferXOffset);

          else
            *frameBufferByte &= (0xFF ^ (1 << frameBufferXOffset));
        }
      }
    }
  }
}



void BM_RE_displayUpdateISR()
{
  MW_LS013B7DH03_ldmaISR(&_displayParams);
}


void BM_RE_displaySignallingTimerISR()
{
  MW_LS013B7DH03_signallingTimerISR(&_displayParams);
}


void BM_RE_VSYNCISR()
{
  TIMER_IntClear(TIMER1, TIMER_IFC_OF);
  BM_ServiceQueue_addItemToServiceQueue(BM_SERVICE_UI, NULL);
}



