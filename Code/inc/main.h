#ifndef MAIN
#define MAIN


#define SYSCLCK 16000000


uint64_t get_time_ms();
void RCC_Init (void);
void GPIO_Init (void);
void TIM4_Init (void);
void TIM1_PWM_Init(void);
void delay (uint32_t counter);

#endif