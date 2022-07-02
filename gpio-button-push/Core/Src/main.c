#include "main.h"
#include <stdint.h>

uint8_t flag = 0;

void gpio_button_init();
uint8_t gpio_button_read();
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
	gpio_button_init();
	gpio_d12_init();
	gpio_d13_init();
	gpio_d14_init();
	gpio_d15_init();

	while (1)
	{
		/* nhan nut -> led sang, nha nut -> led tat*/
		/*
		if((flag % 2) == 1)
		 gpio_d12_write(1);
		else
		 gpio_d12_write(0);
		*/

		/* Moi lan nhan nut -> led dao trang thai*/
		/*if(gpio_button_read() == 1)
		 {
			 HAL_Delay(200);	// delay chong nhieu nut nhan
			 flag++;
		 }

		 if((flag % 2) == 1)
			gpio_d12_write(1);
		 else
			gpio_d12_write(0);*/


		 if(gpio_button_read() == 1)
		 {
			 HAL_Delay(200);	// delay chong nhieu nut nhan
			 flag = 1 - flag;
		 }

		 if(flag == 1)
			gpio_d12_write(1);
		 else
			gpio_d12_write(0);
	}

	return 0;
}

void gpio_button_init()
{
	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Input Level */
	uint32_t *GPIOA_MODER = (uint32_t *)(0x40020000);
	*GPIOA_MODER &= ~(0b11<<0);	// Set PA0 pin Input

	uint32_t *GPIOA_PUPDR = (uint32_t *)(0x4002000c);
	*GPIOA_PUPDR &= ~(0b11<<0);	// Set PA0 pin Floating
}

/*
 * @brief read button state
 * @param None
 * @retval uint8_t:
 * 		@arg 1 button in active
 * 		@arg 2 button in in-active
 */
uint8_t gpio_button_read()
{
	uint32_t *GPIOA_IDR = (uint32_t *)(0x40020010);
	if(((*GPIOA_IDR >> 1) & 1) == 1)
		return 1;
	else
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
