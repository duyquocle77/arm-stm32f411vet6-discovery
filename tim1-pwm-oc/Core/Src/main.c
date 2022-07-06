#include "main.h"
#include <stdint.h>
#include <string.h>

void vectortable_move();

void tim4_init();
void tim4_pwm_ch1(uint8_t duty_cycle);

void tim_systick_init();
void tim_systick_handler();
void tim_systick_delay_ms(uint32_t time_milisec);

uint8_t flag = 0;	/*this flag is status of button*/
uint8_t cnt = 0;	/*this flag is to debug*/

int main(void)
{
	HAL_Init();

	vectortable_move();
	tim4_init();
	tim_systick_init();

	while (1)
	{
		tim4_pwm_ch1(50);
	}

	return 0;
}

/*
 * @brief	: move vector table from FLASH to RAM
 * @param	: None
 * @retval	: None
 */
void vectortable_move()
{
	/*
	 * size(vector_table) = 0x194 + 0x4 - 0x00 = 0x198
	 * */
	/* move vector table from flash to ram */
	void *volatile dst = (void *volatile)0x20000000;	// RAM_address
	void *volatile src = (void *volatile)0x08000000;	// FLASH_address
	memcpy(dst, src, 0x198);

	/**/
	uint32_t *VTOR = (uint32_t *)(0xE000ED08);
	*VTOR = 0x20000000;
}

void tim_systick_init()
{
	uint32_t *CSR = (uint32_t *)(0xe000e010 + 0x00);
	/*enable clock*/
	*CSR |= (1<<2);
	/*enable the counter*/
	*CSR |= (1<<0);


	/*
	 * F = 16 000 000hz
	 * prescaler = 1 -> f = 16000000 hz -> t = 16*10^-6 s
	 * T = 0.01s
	 * T = N*t -> N = 160 000;
	 * */
	uint32_t *RVR = (uint32_t *)(0xe000e010 + 0x00);
	*RVR = 160000 - 1;
}

void tim_systick_delay_ms(uint32_t time_milisec)
{
	uint32_t *CSR = (uint32_t *)(0xe000e010 + 0x00);
	for(uint32_t i = 0; i <= time_milisec; i++)
		while(((*CSR>>16)&1)==0);
}

void tim4_init()
{
	__HAL_RCC_TIM4_CLK_ENABLE();

	/*alternate mode*/
	uint32_t *GPIOD_MODER = (uint32_t *)(0x40020c00 + 0x00);
	*GPIOD_MODER &= ~(0b11<<24);
	*GPIOD_MODER |= (1<<25);

	/*function 2*/
	uint32_t *GPIOD_AFRH = (uint32_t *)(0x40020c00 + 0x24);
	*GPIOD_AFRH &= ~(0b1111<<16);
	*GPIOD_AFRH |= (1<<17);
}

void tim4_pwm_ch1(uint8_t duty_cycle)
{
	/*
	 * T = 1s = 1000 x 1ms
	 * N = 1000 < 65535
	 * t = 1ms -> f = 1000hz
	 * F = 16 000 000hz -> pre-scaler = F/f = 16 000
	 * */
	uint32_t *ARR = (uint32_t *)(0x40000800 + 0x2c);
	*ARR = 1000 - 1;
	uint32_t *PCR = (uint32_t *)(0x40000800 + 0x28);
	*PCR = 16000 - 1;			//prescaler  = f/(PCR + 1)

	uint32_t *CCR = (uint32_t *)(0x40000800 + 0x34);
	*CCR = duty_cycle * 10;

	/*start count*/
	/*enable counter CEN bit*/
	uint32_t *CR1 = (uint32_t *)(0x40000800 + 0x00);
	*CR1 |= (1 << 0);

	uint32_t *CCMR = (uint32_t *)(0x40000800 + 0x18);
	*CCMR |= (1 << 0);
}
