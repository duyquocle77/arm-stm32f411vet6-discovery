#include "main.h"
#include <stdint.h>
#include <string.h>

#define LED_4 12
#define LED_3 13
#define LED_5 14
#define LED_6 15

void custom_delay(uint32_t time);

void gpio_led_init();
void gpio_led_write(uint8_t LED_x, uint8_t value);

void vectortable_move();
void exti0_init();
void exti0_handler();


uint8_t flag = 0;	/*this flag is status of button*/
uint8_t cnt = 0;	/*this flag is to debug*/

int main(void)
{
	HAL_Init();
	gpio_led_init();
	vectortable_move();
	exti0_init();

	while (1)
	{
		 if(flag == 1)
			gpio_led_write(LED_6, 1);
		 else
			gpio_led_write(LED_6, 0);
	}

	return 0;
}

/*
 * @brief	: creat delay time
 * @param	: time
 * 		@arg time can be in range (0...2^32)
 *
 * @retval	: None
 */
void custom_delay(uint32_t time)
{
	for(uint32_t i = 0; i < time; i++)
	{
		__asm("NOP");
	}
}

/*
 * @brief	: initialize D.12, D.13, D.14, D.15 as leds on-board
 * @param	: None
 * @retval	: None
 */
void gpio_led_init()
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  uint32_t *GPIOD_MODER = (uint32_t *)(0x40020c00 + 0x00);
  *GPIOD_MODER &= ~(0b11111111<<24);
  *GPIOD_MODER |= (0b01010101<<24);
}

/*
 * @brief	: set or reset led
 * @param	: LED_x
 * 		x can be (x: 3...6)
 * @param	: state
 * 		@arg 1 : SET
 * 		@arg 0 : RESET
 * @retval	: None
 */
void gpio_led_write(uint8_t LED_x, uint8_t state)
{
  uint32_t *GPIOD_ODR = (uint32_t *)(0x40020c00 + 0x14);
  if (state == 1)
	  *GPIOD_ODR |= (1 << LED_x);
  else
	  *GPIOD_ODR &= ~(1 << LED_x);
}

/*
 * @brief	: move vector table from FLASH to RAM
 * @param	: None
 * @retval	: None
 */
void vectortable_move()
{
	/* move vector table from flash to ram */
	void *dst = (void *)0x20000000;	// RAM
	void *src = (void *)0x00000000;	// FLASH
	memcpy(dst, src, 0x198);

	uint32_t *VTOR = (uint32_t *)(0xE000ED08);
	*VTOR = 0x2000000;

	/* theo mặc định tại địa chỉ 0x20000058 chứa địa chỉ handler mặc định EXTI0_IRQHandler
	 * nên ta gán địa chỉ handler tự định nghĩa
	 * theo quy định của tập lệnh thumb, ta phải |1 với địa chỉ hàm handler
	 *
	 * hàm [void exti0_handler()] có con trỏ hàm là [void(*)()] chứa địa chỉ của hàm
	 * gọi địa chỉ hàm bằng cách gọi tên hàm exti0_handler
	 * */
	/* assign address of user's handler */
	*((uint32_t *)(0x20000058)) = ((uint32_t)exti0_handler | 1);
}

/*
 * @brief	: initialize external interrupt EXTI0
 * @param	: None
 * @retval	: None
 */
void exti0_init()
{
	/*Configure External Interrupt pin   */
	uint32_t *EXTI_IMR = (uint32_t *)(0x40013c00 + 0x00); //enable mask interrupt
	*EXTI_IMR |= (1<<0);

	uint32_t *EXTI_RTSR = (uint32_t *)(0x40013c00 + 0x08); //rising-mode
	*EXTI_RTSR |= (1<<0);

	uint32_t *NVIC_ISER0 = (uint32_t *)(0xe000e100 + 0x00); //enable vector interrupt position 6
	*NVIC_ISER0 |= (1<<6);
}

/*
 * @brief	: handle vector interrupt EXTI0
 * @param	: None
 * @retval	: None
 */
void EXTI0_IRQHandler()
{
	/*custom_delay(500000);
	flag = 1 - flag;
	cnt++;*/

	/* clear interrupt flag */
	uint32_t *EXTI_PR = (uint32_t *)(0x40013c00 + 0x14);
	*EXTI_PR |= (1<<0);
}

/*
 * @brief	: initialize external interrupt EXTI0
 * @param	: None
 * @retval	: None
 */
void exti0_handler()
{
	custom_delay(500000);
	flag = 1 - flag;
	cnt++;

	/* clear interrupt flag */
	uint32_t *PR = (uint32_t *)(0x40013c00 + 0x14);
	*PR |= (1<<0);
}
