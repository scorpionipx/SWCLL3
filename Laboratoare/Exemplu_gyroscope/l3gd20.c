//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "l3gd20.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "spi.h"

void L3GD20_InitChip(L3GD20_SCALE_t scale);
void L3GD20_WriteReg(uint8_t adr, uint8_t data);
uint8_t L3GD20_ReadReg(uint8_t adr);


L3GD20_RAW_t L3GD20_RAW;


uint8_t L3GD20_Init(L3GD20_SCALE_t scale)
{
  uint8_t ret_data = ERROR;
  uint8_t data;


  L3GD20_RAW.faktor = 1.0;
  L3GD20_RAW.x_axes = 0;
  L3GD20_RAW.y_axes = 0;
  L3GD20_RAW.z_axes = 0;

  data = L3GD20_ReadReg(L3GD20_ADR_ID);
  if (data != L3GD20_ID)
  {

    ret_data = ERROR;
  }
  else
  {
    L3GD20_InitChip(scale);
    ret_data = SUCCESS;
  }
  return ret_data;
}

void L3GD20_Read(L3GD20_t *L3GD20)
{
  uint8_t data_lo, data_hi;
  int16_t raw_data;
  float f;

  data_lo = L3GD20_ReadReg(L3GD20_ADR_X_LO);
  data_hi = L3GD20_ReadReg(L3GD20_ADR_X_HI);
  raw_data = (int16_t) ((data_hi << 8) | (data_lo));
  L3GD20_RAW.x_axes = raw_data;

  f = (float) (raw_data) / L3GD20_RAW.faktor;
  L3GD20->x_axes = (int16_t) (f);

  data_lo = L3GD20_ReadReg(L3GD20_ADR_Y_LO);
  data_hi = L3GD20_ReadReg(L3GD20_ADR_Y_HI);
  raw_data = (int16_t) ((data_hi << 8) | (data_lo));
  L3GD20_RAW.y_axes = raw_data;

  f = (float) (raw_data) / L3GD20_RAW.faktor;
  L3GD20->y_axes = (int16_t) (f);

  data_lo = L3GD20_ReadReg(L3GD20_ADR_Z_LO);
  data_hi = L3GD20_ReadReg(L3GD20_ADR_Z_HI);
  raw_data = (int16_t) ((data_hi << 8) | (data_lo));
  L3GD20_RAW.z_axes = raw_data;

  f = (float) (raw_data) / L3GD20_RAW.faktor;
  L3GD20->z_axes = (int16_t) (f);
}

void L3GD20_InitChip(L3GD20_SCALE_t scale)
{

  L3GD20_WriteReg(L3GD20_ADR_REG1, L3GD20_PWR_ON);

  if (scale == SCALE_250)
  {
    L3GD20_WriteReg(L3GD20_ADR_REG4, L3GD20_SCALE_250);
    L3GD20_RAW.faktor = L3GD20_250_FAKTOR;
  }
  else if (scale == SCALE_500)
  {
    L3GD20_WriteReg(L3GD20_ADR_REG4, L3GD20_SCALE_500);
    L3GD20_RAW.faktor = L3GD20_500_FAKTOR;
  }
  else
  {
    L3GD20_WriteReg(L3GD20_ADR_REG4, L3GD20_SCALE_2000);
    L3GD20_RAW.faktor = L3GD20_2000_FAKTOR;
  }

// Filter
  L3GD20_WriteReg(L3GD20_ADR_REG2, L3GD20_FILTER);

// Filter enable
  L3GD20_WriteReg(L3GD20_ADR_REG5, L3GD20_FILTER_ON);
}

void L3GD20_WriteReg(uint8_t adr, uint8_t data)
{
// ChipSelect
  HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_RESET);

  HAL_SPI_Transmit(&hspi5, &adr, 1, 1);

  HAL_SPI_Transmit(&hspi5, &data, 1, 1);

  HAL_Delay(L3GD20_SPI_PAUSE);

// ChipSelect
  HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);
}

uint8_t L3GD20_ReadReg(uint8_t adr)
{
  uint8_t ret_data = 0;
  uint8_t cmd = 0x55;
// ChipSelect
  HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_RESET);

  adr |= 0x80;
  HAL_SPI_Transmit(&hspi5, &adr, 1, 1);

  HAL_SPI_Transmit(&hspi5, &cmd, 1, 1);
  HAL_SPI_Receive(&hspi5, &ret_data, 1, 1);

  HAL_Delay(L3GD20_SPI_PAUSE);

// ChipSelect
  HAL_GPIO_WritePin(NCS_MEMS_SPI_GPIO_Port, NCS_MEMS_SPI_Pin, GPIO_PIN_SET);

  return (ret_data);
}

