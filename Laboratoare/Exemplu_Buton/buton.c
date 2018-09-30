/*
 * buton.c
 *
 *  Created on: 23 nov. 2016
 *      Author: Adrian
 */
#include"buton.h"
#include"stm32f4xx_hal.h"
#include"gpio.h"

uint8_t CitireStareButon(void)
{
  static uint8_t buton_apasat = 0;
  static uint8_t buton_eliberat = 1;
  uint8_t StareButon = ELIBERAT;

  /* Se citeste si verifica starea pinului PA0 de pe GPIOA */
  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == APASAT)
  {
    /* Butonul este apasat si se asteapta o perioada scurta (1ms) pentru stabilizarea frontului crescator */
    HAL_Delay(1);
    /* Se citeste starea pinului PA0 de pe GPIOA si se verifica starea anterioara */
    if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == APASAT) && (buton_eliberat == 1))
    {
      buton_eliberat = 0;
      buton_apasat = 1;
      /* Starea butonului este 1 logic - apasat */
      StareButon = APASAT;
    }
    else
    {
      /* Posibil ca butonul sa fie eliberat */
      /* Se citeste starea pinului PA0 de pe GPIOA si se verifica starea anterioara */
      if((HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == ELIBERAT) && (buton_apasat == 1))
      {
        buton_apasat = 0;
        buton_eliberat = 1;
        /* Starea butonului este 0 logic - eliberat */
        StareButon = ELIBERAT;
      }
    }
  }
  return StareButon;
}
