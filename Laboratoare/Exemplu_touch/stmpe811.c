#include "STMPE811.h"
#include "i2c.h"
#include "stm32f4xx_hal.h"
#include "ili9341.h"

void STMPE811_init()
{

  STMPE811_write_byte(STMPE811_SYS_CTRL1, 0x02);
  HAL_Delay(6);
  STMPE811_write_byte(STMPE811_SYS_CTRL1, 0x00);
  HAL_Delay(4);
  STMPE811_write_byte(STMPE811_SYS_CTRL2, 0x04);
  STMPE811_write_byte(STMPE811_SPI_CFG, 0x00);
  STMPE811_write_byte(STMPE811_FIFO_STA, 0x01);
  STMPE811_write_byte(STMPE811_FIFO_STA, 0x00);
  STMPE811_write_byte(STMPE811_ADC_CTRL1, 0x08);
  STMPE811_write_byte(STMPE811_ADC_CTRL2, 0x02);
  STMPE811_write_byte(STMPE811_GPIO_AF, 0x0F);
  STMPE811_write_byte(STMPE811_TSC_CFG, 0xC9);
  STMPE811_write_byte(STMPE811_FIFO_TH, 0x01);
  STMPE811_write_byte(STMPE811_FIFO_STA, 0x01);
  STMPE811_write_byte(STMPE811_FIFO_STA, 0x00);
  STMPE811_write_byte(STMPE811_TSC_FRACTION_Z, 0x01);
  STMPE811_write_byte(STMPE811_TSC_I_DRIVE, 0x01);
  STMPE811_write_byte(STMPE811_TSC_CTRL, 0x03);
  STMPE811_write_byte(STMPE811_INT_STA, 0xFF);
  HAL_Delay(4);
}

void STMPE811_write_byte(unsigned char addr, unsigned char value)
{
  HAL_I2C_Mem_Write(&hi2c3, STMPE811_device_address, (uint16_t) addr, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
}

unsigned char STMPE811_read_byte(unsigned char addr)
{
  unsigned char data = 0;

  HAL_I2C_Mem_Read(&hi2c3, STMPE811_device_address, (uint16_t) addr, I2C_MEMADD_SIZE_8BIT, &data, 1, 10);
  return data;
}

void STMPE811_write_word(unsigned char addr, uint16_t value)
{
  unsigned char data_values[2] =
  { 0x00, 0x00 };

  data_values[1] = (value & 0x00FF);
  data_values[0] = (value >> 0x08);

  HAL_I2C_Mem_Write(&hi2c3, STMPE811_device_address,(uint16_t)addr,I2C_MEMADD_SIZE_8BIT, &data_values[0], 2, 10);
}

uint16_t STMPE811_read_word(unsigned char addr)
{
  unsigned char value_d[2] =
  { 0x00, 0x00 };

  uint16_t value = 0x0000;

  HAL_I2C_Mem_Read(&hi2c3, STMPE811_device_address, (uint16_t)addr,I2C_MEMADD_SIZE_8BIT,&value_d[0], 2, 10);


  value = value_d[0];
  value <<= 0x08;
  value |= value_d[1];

  return value;
}

uint16_t STMPE811_read_x()
{
  uint16_t value = 0x0000;

  value = STMPE811_read_word(STMPE811_TSC_DATA_X);

  return value;
}

uint16_t STMPE811_read_y()
{
  uint16_t value = 0x0000;

  value = STMPE811_read_word(STMPE811_TSC_DATA_Y);

  return value;
}

void STMPE811_get_touch(uint16_t *x_pos, uint16_t *y_pos, unsigned char orientation)
{
  unsigned char temp = 0x00;

  int16_t xp = 0x0000;
  int16_t yp = 0x0000;
  int16_t tp = 0x0000;

  temp = STMPE811_read_byte(STMPE811_TSC_CTRL);

  if ((temp & 0x80) != 0x00)
  {
    xp = STMPE811_read_x();
    yp = STMPE811_read_y();

    STMPE811_write_byte(STMPE811_FIFO_STA, 0x01);
    STMPE811_write_byte(STMPE811_FIFO_STA, 0x00);

    switch (orientation)
    {
      case PORTRAIT_1:
      {
        tp = map(xp, RW_min, RW_max, W_min, W_max);
        *x_pos = constrain(tp, W_min, W_max);
        tp = map(yp, RL_max, RL_min, L_min, L_max);
        *y_pos = constrain(tp, L_min, L_max);
        break;
      }
      case PORTRAIT_2:
      {
        tp = map(xp, RW_max, RW_min, W_min, W_max);
        *x_pos = constrain(tp, W_min, W_max);
        tp = map(yp, RL_min, RL_max, L_min, L_max);
        *y_pos = constrain(tp, L_min, L_max);
        break;
      }
      case LANDSCAPE_1:
      {
        tp = map(yp, RL_max, RL_min, L_min, L_max);
        *x_pos = constrain(tp, L_min, L_max);
        tp = map(xp, RW_max, RW_min, 0, W_max);
        *y_pos = constrain(tp, W_min, W_max);
        break;
      }
      case LANDSCAPE_2:
      {
        tp = map(yp, RL_min, RL_max, L_min, L_max);
        *x_pos = constrain(tp, L_min, L_max);
        tp = map(xp, RW_min, RW_max, W_min, W_max);
        *y_pos = constrain(tp, W_min, W_max);
        break;
      }
    }
  }
}

float map(float value, float x_min, float x_max, float y_min, float y_max)
{
  return (y_min + (((value - x_min)*(y_max-y_min)) / (x_max - x_min)));
}

float constrain(float value, float value_min, float value_max)
{
  if (value >= value_max)
  {
    return value_max;
  }
  else if (value <= value_min)
  {
    return value_min;
  }
  else
  {
    return value;
  }
}
