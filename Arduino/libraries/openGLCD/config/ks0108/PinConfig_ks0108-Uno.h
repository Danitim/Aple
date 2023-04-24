/*
 * ks0108_Uno.h - User specific configuration for openGLCD library
 *
 * Use this file to set io pins
 * when using an Uno, 2009, or any AVR m168/m328 based board
 *
 */

#ifndef GLCD_PIN_CONFIG_H
#define GLCD_PIN_CONFIG_H

/*
 * define name for pin configuration
 */
#define glcd_PinConfigName "ks0108-Uno"

/*********************************************************/
/*  Configuration for assigning LCD bits to Arduino Pins */
/*********************************************************/

/*
 * Pins can be assigned using Arduino pin numbers 0-n
 * Pins can also be assigned using AVRPIN_Pb 
 *    where P is port A-L and b is bit 0-7
 *    Example: port D bit 3 is AVRPIN_D3
 *
 * Pins can also be assigned using 0xPb for ports A to F
 *     Example: port D bit 3 is 0xD3
 */

/* Data pin definitions
 */
#define glcdPinData0        13
#define glcdPinData1        12
#define glcdPinData2        11
#define glcdPinData3        10
#define glcdPinData4        9
#define glcdPinData5        8
#define glcdPinData6        7
#define glcdPinData7        6

/* Arduino pins used for Control
 * default assignment uses the first five analog pins
 */

#define glcdPinCSEL1        5
#define glcdPinCSEL2        4

#if NBR_CHIP_SELECT_PINS > 2
#define glcdPinCSEL3         3   // third chip select if needed
#endif

#if NBR_CHIP_SELECT_PINS > 3
#define glcdPinCSEL4         2   // fourth chip select if needed
#endif

#define glcdPinRW           A1
#define glcdPinDI           A0
#define glcdPinEN           A2 // (A4 is also I2C SDA)
// Reset  - uncomment the next line if glcd module reset is connected to an Arduino pin
#define glcdPinRES          A3    // optional s/w Reset control (A5 is also I2C SCL)

//#define glcdPinBL	XX // optional backlight control pin controls BL circuit

#endif //GLCD_PIN_CONFIG_H
