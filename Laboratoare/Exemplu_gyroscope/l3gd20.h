#ifndef __L3GD20_H
#define __L3GD20_H
#include <stdint.h>
typedef enum {
  SCALE_250 = 0,  // 250
  SCALE_500,      // 500
  SCALE_2000      // 2000
}L3GD20_SCALE_t;




typedef struct {
  int16_t x_axes;
  int16_t y_axes;
  int16_t z_axes;
}L3GD20_t;





typedef struct {
  float faktor;
  int16_t x_axes;
  int16_t y_axes;
  int16_t z_axes;
}L3GD20_RAW_t;


//--------------------------------------------------------------
// L3GD20 Defines
//-------------------------------------------------------------- 
#define  L3GD20_ID           0xD4   // ID  L3GD20 IC
#define  L3GD20_PWR_ON       0x3F   // L3GD20 On
#define  L3GD20_SCALE_250    0x00
#define  L3GD20_SCALE_500    0x10
#define  L3GD20_SCALE_2000   0x20
#define  L3GD20_FILTER       0x00
#define  L3GD20_FILTER_ON    0x10   // Filter On

#define  L3GD20_SPI_PAUSE      1   //

#define  L3GD20_250_FAKTOR    114.285  // 8.75mdps per Digit
#define  L3GD20_500_FAKTOR    57.1429  // 17.5mdps per Digit
#define  L3GD20_2000_FAKTOR   14.285   // 70mdps per Digit


//--------------------------------------------------------------
// Register Addresses
//--------------------------------------------------------------
#define L3GD20_ADR_ID             0x0F
#define L3GD20_ADR_REG1           0x20
#define L3GD20_ADR_REG2           0x21
#define L3GD20_ADR_REG4           0x23
#define L3GD20_ADR_REG5           0x24
#define L3GD20_ADR_X_LO           0x28
#define L3GD20_ADR_X_HI           0x29
#define L3GD20_ADR_Y_LO           0x2A
#define L3GD20_ADR_Y_HI           0x2B
#define L3GD20_ADR_Z_LO           0x2C
#define L3GD20_ADR_Z_HI           0x2D



uint8_t L3GD20_Init(L3GD20_SCALE_t scale);
void L3GD20_Read(L3GD20_t *L3GD20);




//--------------------------------------------------------------
#endif // __L3GD20_H
