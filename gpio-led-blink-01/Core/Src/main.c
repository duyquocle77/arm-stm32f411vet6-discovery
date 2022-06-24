#include "main.h"
#include <stdint.h>

void gpio_d12_init();
void gpio_d13_init();
void gpio_d14_init();
void gpio_d15_init();
void gpio_d12_write(uint8_t value);
void gpio_d13_write(uint8_t value);
void gpio_d14_write(uint8_t value);
void gpio_d15_write(uint8_t value);

int main(void)
{
	HAL_Init();
	gpio_d12_init();
	gpio_d13_init();
	gpio_d14_init();
	gpio_d15_init();

	while (1)
	{
		uint8_t i = 200;

		while (i > 0)
		{
			gpio_d12_write(1);
			HAL_Delay(i);
			gpio_d12_write(0);
			HAL_Delay(i);

			gpio_d13_write(1);
			HAL_Delay(i);
			gpio_d13_write(0);
			HAL_Delay(i);

			gpio_d14_write(1);
			HAL_Delay(i);
			gpio_d14_write(0);
			HAL_Delay(i);

			gpio_d15_write(1);
			HAL_Delay(i);
			gpio_d15_write(0);
			HAL_Delay(i);

			i -= 20;
		}
	}

  return 0;
}

void gpio_d12_init()
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  uint32_t *GPIOD_MODER = (uint32_t *)(0x40020c00);
  *GPIOD_MODER &= ~(1<<24);
  *GPIOD_MODER &= ~(1<<25);
  *GPIOD_MODER |= (1<<24);
}

void gpio_d13_init()
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  uint32_t *GPIOD_MODER = (uint32_t *)(0x40020c00);
  *GPIOD_MODER &= ~(1<<26);
  *GPIOD_MODER &= ~(1<<27);
  *GPIOD_MODER |= (1<<26);
}

void gpio_d14_init()
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  uint32_t *GPIOD_MODER = (uint32_t *)(0x40020c00);
  *GPIOD_MODER &= ~(1<<28);
  *GPIOD_MODER &= ~(1<<29);
  *GPIOD_MODER |= (1<<28);
}

void gpio_d15_init()
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  uint32_t *GPIOD_MODER = (uint32_t *)(0x40020c00);
  *GPIOD_MODER &= ~(1<<30);
  *GPIOD_MODER &= ~(1<<31);
  *GPIOD_MODER |= (1<<30);
}

void gpio_d12_write(uint8_t value)
{
  uint32_t *GPIOD_ODR = (uint32_t *)(0x40020c14);
  if (value == 1)
	  *GPIOD_ODR |= (1<<12);
  else
	  *GPIOD_ODR &= ~(1<<12);
}

void gpio_d13_write(uint8_t value)
{
  uint32_t *GPIOD_ODR = (uint32_t *)(0x40020c14);
  if (value == 1)
	  *GPIOD_ODR |= (1<<13);
  else
	  *GPIOD_ODR &= ~(1<<13);
}

void gpio_d14_write(uint8_t value)
{
  uint32_t *GPIOD_ODR = (uint32_t *)(0x40020c14);
  if (value == 1)
	  *GPIOD_ODR |= (1<<14);
  else
	  *GPIOD_ODR &= ~(1<<14);
}

void gpio_d15_write(uint8_t value)
{
  uint32_t *GPIOD_ODR = (uint32_t *)(0x40020c14);
  if (value == 1)
	  *GPIOD_ODR |= (1<<15);
  else
	  *GPIOD_ODR &= ~(1<<15);
}
