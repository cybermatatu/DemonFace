#include <Arduino.h>

static const uint8_t PROGMEM // Bitmaps are stored in program memory

  bootImg[4][8] = { // Startup image
  { 0b00011000,     // Eyes
    0b00011000,
    0b00011000,
    0b11111111,
    0b11111111,
    0b00011000,
    0b00011000,
    0b00011000 },
  { 0b00000000,     // Mouth: left matrix
    0b00000000,
    0b00001111,
    0b00111111,
    0b01111000,
    0b11100000,
    0b01000000,
    0b00000000 },
  { 0b00000000,     // Mouth: left matrix
    0b11111111,
    0b11111111,
    0b10000001,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000 },
  { 0b00000000,     // Mouth: right matrix
    0b00000000,
    0b11110000,
    0b11111100,
    0b00011110,
    0b00000111,
    0b00000010,
    0b00000000 } },

  blinkImg[8][8] = {   // Eye animation frames
  { 0b00001111,
    0b00111111,
    0b01111111,
    0b01111111,
    0b11111111,
    0b11111110,
    0b11111110,
    0b11111000 },
  { 0b00000111,
    0b00011111,
    0b00111111,
    0b01111111,
    0b01111111,
    0b11111110,
    0b11111100,
    0b11111000 },
  { 0b00000011,
    0b00001111,
    0b00011111,
    0b00111111,
    0b01111111,
    0b01111110,
    0b11111100,
    0b11111000 },
  { 0b00000001,
    0b00000111,
    0b00011111,
    0b00111111,
    0b00111110,
    0b01111110,
    0b01111100,
    0b11110000 },
  { 0b00000001,
    0b00000011,
    0b00000111,
    0b00001111,
    0b00011110,
    0b00111110,
    0b01111100,
    0b11110000 },
  { 0b00000001,
    0b00000001,
    0b00000011,
    0b00000010,
    0b00000110,
    0b00001100,
    0b00111000,
    0b11100000 },
  { 0b00000001,
    0b00000001,
    0b00000001,
    0b00000010,
    0b00000010,
    0b00000100,
    0b00011000,
    0b11100000 },
  { 0b00000000,         // Fully closed eye
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000 } },

  blinkIndex[] = { 0,1,2,3,4,5,6,7,6,5,3,2,1 }, // Blink bitmap sequence

  lidImg[7][8] = {   // Upper lid overlay when looking around (see code)
  { 0b11110000,
    0b11000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000 },
  { 0b11110000,
    0b11000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000 },
  { 0b11111000,
    0b11100000,
    0b11000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000 },
  { 0b11111100,
    0b11110000,
    0b11100000,
    0b11000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000 },
  { 0b11111110,
    0b11111000,
    0b11110000,
    0b11100000,
    0b11000000,
    0b10000000,
    0b10000000,
    0b00000000 },
  { 0b11111110,
    0b11111100,
    0b11111000,
    0b11110000,
    0b11100000,
    0b11000000,
    0b10000000,
    0b00000000 },
  { 0b11111110,
    0b11111110,
    0b11111100,
    0b11111000,
    0b11110000,
    0b11100000,
    0b11000000,
    0b00000000 } },

  pupilImg[7] = {     // Pupil bitmap
    0b0000000,        // IMPORTANT: THIS IS 7X7, NOT 8X8,
    0b0000000,        // so it has a distinct center point
    0b0011000,
    0b0011100,        // <-- pupil center point is on this line
    0b0001100,
    0b0000000,
    0b0000000 },
  landingImg[8] = {   // This is a bitmask of where
    0b00000000,       // the pupil can safely "land" when
    0b00001100,       // a new random position is selected,
    0b00111110,       // so it doesn't run too far off the
    0b00111110,       // edge and look bad.  If you edit the
    0b01111110,       // eye or pupil bitmaps, you may want
    0b01111100,       // to adjust this...use '1' for valid
    0b00111000,       // pupil positions, '0' for off-limits
    0b00000000 },     // points.

  //  ........  Using the langing map, this is X+Y-4,
  //  ....12..  used for the pupil-tracking upper lid
  //  ..01234.
  //  ..12345.
  //  .123456.
  //  .23456..
  //  ..456...
  //  ........

  mouthImg[8][3][8] = {               // Mouth animation frames
  { { 0b00000000,                     // Mouth position 0 (closed)
      0b00000001,
      0b00111110,
      0b11000000,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },
            { 0b11000011,
              0b01000010,
              0b01111110,
              0b00000000,
              0b00000000,
              0b00000000,
              0b00000000,
              0b00000000 },
                    { 0b00000000,
                      0b10000000,
                      0b01111100,
                      0b00000011,
                      0b00000000,
                      0b00000000,
                      0b00000000,
                      0b00000000 } },
  { { 0b00000000,                     // Mouth position 1
      0b00000000,
      0b00000011,
      0b00111110,
      0b11000000,
      0b00000000,
      0b00000000,
      0b00000000 },
            { 0b00000000,
              0b11000011,
              0b01111110,
              0b00000000,
              0b00000000,
              0b00000000,
              0b00000000,
              0b00000000 },
                    { 0b00000000,
                      0b00000000,
                      0b11000000,
                      0b01111100,
                      0b00000011,
                      0b00000000,
                      0b00000000,
                      0b00000000 } },
  { { 0b00000000,                     // Mouth position 2
      0b00000011,
      0b00011011,
      0b01111110,
      0b00000000,
      0b00000000,
      0b00000000,
      0b00000000 },
            { 0b00000000,
              0b11111111,
              0b01111110,
              0b01111110,
              0b00000000,
              0b00000000,
              0b00000000,
              0b00000000 },
                    { 0b00000000,
                      0b11000000,
                      0b11011000,
                      0b01111110,
                      0b00000000,
                      0b00000000,
                      0b00000000,
                      0b00000000 } },
  { { 0b00000000,                     // Mouth position 3
      0b00000011,
      0b01111011,
      0b00011111,
      0b00001110,
      0b00000000,
      0b00000000,
      0b00000000 },
            { 0b00000000,
              0b11111111,
              0b11111111,
              0b01111110,
              0b01111110,
              0b00000000,
              0b00000000,
              0b00000000 },
                    { 0b00000000,
                      0b11000000,
                      0b11011110,
                      0b11111000,
                      0b01110000,
                      0b00000000,
                      0b00000000,
                      0b00000000 } },
  { { 0b00000000,                     // Mouth position 4
      0b00000001,
      0b01111001,
      0b00111101,
      0b00011111,
      0b00000110,
      0b00000000,
      0b00000000 },
            { 0b00000000,
              0b11111111,
              0b11111111,
              0b11111111,
              0b01111110,
              0b01111110,
              0b00000000,
              0b00000000 },
                    { 0b00000000,
                      0b10000000,
                      0b10011110,
                      0b10111100,
                      0b11111000,
                      0b01100000,
                      0b00000000,
                      0b00000000 } },
  { { 0b00000000,                     // Mouth position 5
      0b00000001,
      0b00111001,
      0b00011101,
      0b00011111,
      0b00001111,
      0b00000110,
      0b00000000 },
            { 0b00000000,
              0b11111111,
              0b11111111,
              0b11111111,
              0b11111111,
              0b01111110,
              0b01111110,
              0b00000000 },
                    { 0b00000000,
                      0b10000000,
                      0b10011100,
                      0b10111000,
                      0b11111000,
                      0b11110000,
                      0b01100000,
                      0b00000000 } },
  { { 0b00000000,                     // Mouth position 6
      0b00000001,
      0b00111001,
      0b00111101,
      0b00011111,
      0b00011111,
      0b00001111,
      0b00000110 },
            { 0b00000000,
              0b11111111,
              0b11111111,
              0b11111111,
              0b11111111,
              0b11111111,
              0b01111110,
              0b01111110 },
                    { 0b00000000,
                      0b10000000,
                      0b10011100,
                      0b10111100,
                      0b11111100,
                      0b11111000,
                      0b11110000,
                      0b01100000 } },
  { { 0b00000000,                     // Mouth position 7 (fully open)
      0b00001001,
      0b00011001,
      0b00011101,
      0b00011111,
      0b00011111,
      0b00001111,
      0b00000110 },
            { 0b01111110,
              0b11111111,
              0b11111111,
              0b11111111,
              0b11111111,
              0b11111111,
              0b01111110,
              0b01111110 },
                    { 0b00000000,
                      0b10010000,
                      0b10011000,
                      0b10111000,
                      0b11111000,
                      0b11111000,
                      0b11110000,
                      0b01100000 } } },

breathTable[256] = { // PWM duty cycles for breathing effect
  0x03,0x03,0x03,0x03,0x03,0x03,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,
  0x05,0x05,0x05,0x05,0x06,0x06,0x06,0x07,0x07,0x07,0x08,0x08,0x09,0x09,0x0a,
  0x0a,0x0b,0x0c,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
  0x19,0x1a,0x1c,0x1d,0x1f,0x21,0x22,0x24,0x26,0x28,0x2a,0x2d,0x2f,0x31,0x34,
  0x36,0x39,0x3b,0x3e,0x41,0x44,0x47,0x4a,0x4d,0x50,0x54,0x57,0x5a,0x5e,0x61,
  0x65,0x69,0x6c,0x70,0x74,0x78,0x7c,0x80,0x84,0x88,0x8c,0x90,0x94,0x98,0x9c,
  0xa0,0xa4,0xa8,0xac,0xb0,0xb4,0xb8,0xbb,0xbf,0xc3,0xc7,0xca,0xce,0xd1,0xd5,
  0xd8,0xdb,0xde,0xe1,0xe4,0xe7,0xe9,0xec,0xee,0xf0,0xf2,0xf4,0xf6,0xf8,0xf9,
  0xfa,0xfb,0xfc,0xfd,0xfe,0xfe,0xff,0xff,0xff,0xff,0xfe,0xfe,0xfd,0xfc,0xfb,
  0xfa,0xf9,0xf8,0xf6,0xf4,0xf2,0xf0,0xee,0xec,0xe9,0xe7,0xe4,0xe1,0xde,0xdb,
  0xd8,0xd5,0xd1,0xce,0xca,0xc7,0xc3,0xbf,0xbb,0xb8,0xb4,0xb0,0xac,0xa8,0xa4,
  0xa0,0x9c,0x98,0x94,0x90,0x8c,0x88,0x84,0x80,0x7c,0x78,0x74,0x70,0x6c,0x69,
  0x65,0x61,0x5e,0x5a,0x57,0x54,0x50,0x4d,0x4a,0x47,0x44,0x41,0x3e,0x3b,0x39,
  0x36,0x34,0x31,0x2f,0x2d,0x2a,0x28,0x26,0x24,0x22,0x21,0x1f,0x1d,0x1c,0x1a,
  0x19,0x17,0x16,0x15,0x14,0x13,0x12,0x11,0x10,0x0f,0x0e,0x0d,0x0c,0x0c,0x0b,
  0x0a,0x0a,0x09,0x09,0x08,0x08,0x07,0x07,0x07,0x06,0x06,0x06,0x05,0x05,0x05,
  0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x03,0x03,0x03,0x03,0x03,
  0x03 };
