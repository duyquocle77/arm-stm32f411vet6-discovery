#include "main.h"
#include <stdint.h>
#include <string.h>

void vectortable_move();

void tim4_pwm_ch1_init();
void tim4_pwm_ch1(uint8_t duty_cycle);

void tim1_ic_ch1_init();
void tim1_ic_ch2_init();

void tim_systick_init();
void tim_systick_handler();
void tim_systick_delay_ms(uint32_t time_milisec);

uint32_t time;
uint32_t cnt;
float freq;
uint32_t duty;

int main(void)
{
	HAL_Init();

	vectortable_move();
	tim4_pwm_ch1_init();
	tim_systick_init();
	tim1_ic_ch1_init();
	tim1_ic_ch2_init();
	tim4_pwm_ch1(57);
	uint32_t *TIM1_CCR1 = (uint32_t *)(0x40010000 + 0x34);
	uint32_t *TIM1_CCR2 = (uint32_t *)(0x40010000 + 0x38);



	while (1)
	{

		time = *TIM1_CCR1 + 1;
		freq = (float)(1 / time);
		cnt = *TIM1_CCR2 + 1;
		duty = (*TIM1_CCR2 + 1) *100 / time;
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

void tim4_pwm_ch1_init()
{
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();

	/*alternate mode*/
	uint32_t *GPIOD_MODER = (uint32_t *)(0x40020c00 + 0x00);
	*GPIOD_MODER &= ~(0b11<<24);
	*GPIOD_MODER |= (1<<25);

	/*alternate function 2*/
	uint32_t *GPIOD_AFRH = (uint32_t *)(0x40020c00 + 0x24);
	*GPIOD_AFRH &= ~(0b1111<<16);
	*GPIOD_AFRH |= (1<<17);
}

void tim4_pwm_ch1(uint8_t duty_cycle)
{


	/*
	 * T = 0.001s = 1000 x ns
	 * N = 1000 < 65535
	 * t = 10^-6ms -> f = 1000000hz
	 * F = 16 000 000hz -> pre-scaler = F/f = 16
	 * */
	uint32_t *ARR = (uint32_t *)(0x40000800 + 0x2c);
	*ARR = 1000 - 1;
	uint32_t *PCR = (uint32_t *)(0x40000800 + 0x28);
	*PCR = 1600 - 1;			//prescaler  = (PCR + 1)

	uint32_t *CCR1 = (uint32_t *)(0x40000800 + 0x34);
	*CCR1 = (duty_cycle * (*ARR + 1)) / 100;

	/*start count*/
	/*enable counter CEN bit*/
	uint32_t *CR1 = (uint32_t *)(0x40000800 + 0x00);
	*CR1 |= (1 << 0);
	/*up-counter mode*/
	*CR1 &= ~(1 << 4);

	/*pwm mode 1: CNT < CCR1 -> active*/
	uint32_t *CCMR1 = (uint32_t *)(0x40000800 + 0x18);
	*CCMR1 &= ~(0b111<<4);
	*CCMR1 |= (0b11 << 5);

	/*enable OC1 chanel 1*/
	uint32_t *CCER = (uint32_t *)(0x40000800 + 0x20);
	*CCER |= (1 << 0);
}

void tim1_ic_ch1_init()
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_TIM1_CLK_ENABLE();

	/*alternate mode*/
	uint32_t *GPIOE_MODER = (uint32_t *)(0x40021000 + 0x00);
	*GPIOE_MODER &= ~(0b11<<18);
	*GPIOE_MODER |= (1<<19);

	/*alternate function 1*/
	uint32_t *GPIOE_AFRH = (uint32_t *)(0x40021000 + 0x24);
	*GPIOE_AFRH &= ~(0b1111<<4);
	*GPIOE_AFRH |= (1<<4);

	uint32_t *ARR = (uint32_t *)(0x40010000 + 0x2c);
	*ARR = 0xFFFF;
	uint32_t *PCR = (uint32_t *)(0x40010000 + 0x28);
	*PCR = 1600 - 1;			//prescaler  = (PCR + 1)

	/*IC1 mapped on TI1*/
	uint32_t *CCMR1 = (uint32_t *)(0x40010000 + 0x18);
	*CCMR1 &= ~(0b11<<0);
	*CCMR1 |=  (0b01<<0);

	/*select TRGI as TI1*/
	uint32_t *SMCR = (uint32_t *)(0x40010000 + 0x08);
	*SMCR &= ~(0b111<<4);
	*SMCR |=  (0b101<<4);

	/*slave mode as Reset mode*/
	*SMCR &= ~(0b111<<0);
	*SMCR |=  (0b100<<0);

	/*enable CC1 chanel 1*/
	uint32_t *CCER = (uint32_t *)(0x40010000 + 0x20);
	*CCER |= (1 << 0);

	/*capture rising-edge*/
	*CCER &= ~(1<<1);

	*CCER |= (1 << 0);

	/*start count*/
	/*enable counter CEN bit*/
	uint32_t *CR1 = (uint32_t *)(0x40010000 + 0x00);
	*CR1 |= (1 << 0);
}

void tim1_ic_ch2_init()
{
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_TIM1_CLK_ENABLE();

	/*alternate mode*/
	uint32_t *GPIOE_MODER = (uint32_t *)(0x40021000 + 0x00);
	*GPIOE_MODER &= ~(0b11<<22);
	*GPIOE_MODER |= (1<<23);

	/*alternate function 1*/
	uint32_t *GPIOE_AFRH = (uint32_t *)(0x40021000 + 0x24);
	*GPIOE_AFRH &= ~(0b1111<<12);
	*GPIOE_AFRH |= (1<<12);

	uint32_t *ARR = (uint32_t *)(0x40010000 + 0x2c);
	*ARR = 0xFFFF;
	uint32_t *PCR = (uint32_t *)(0x40010000 + 0x28);
	*PCR = 1600 - 1;			//prescaler  = (PCR + 1)

	/** F = 16 000 000 hz
	 *
	 * 16    ->  1 000 000 hz -> 10^-6 s
	 * 160   ->    100 000 hz -> 10^-5 s
	 * 1600  ->     10 000 hz -> 10^-4 s
	 * 16000 ->      1 000 hz -> 10^-3 s
	 */

	/*IC2 mapped on TI1*/
	uint32_t *CCMR1 = (uint32_t *)(0x40010000 + 0x18);
	*CCMR1 &= ~(0b11<<8);
	*CCMR1 |=  (0b10<<8);

	/*select TRGI as TI1*/
	uint32_t *SMCR = (uint32_t *)(0x40010000 + 0x08);
	*SMCR &= ~(0b111<<4);
	*SMCR |=  (0b101<<4);

	/*slave mode as Reset mode*/
	*SMCR &= ~(0b111<<0);
	*SMCR |=  (0b100<<0);

	/*enable CC1 chanel 2*/
	uint32_t *CCER = (uint32_t *)(0x40010000 + 0x20);
	*CCER |= (1 << 4);

	/*capture falling-edge*/
	*CCER &= ~(1<<5);
	*CCER |=  (1<<5);

	*CCER |= (1 << 4);

	/*start count*/
	/*enable counter CEN bit*/
	uint32_t *CR1 = (uint32_t *)(0x40010000 + 0x00);
	*CR1 |= (1 << 0);
}
