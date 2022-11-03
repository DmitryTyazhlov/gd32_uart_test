#include "gd32f3x0.h"                   
#include "gd32f3x0_gpio.h"              
#include "gd32f3x0_usart.h"               
#include <stdint.h>

void uart_init(void);
void receive_uart(void);
void transmit_uart(uint8_t *buf, uint8_t size);

uint8_t rec_buf[32] = {0};
uint8_t transmit_buf[] = {0x55, 0x32, 0x04, 0x35, 0x61};

int main(){

	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_GPIOB);

	gpio_mode_set(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_8);
	gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);

	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_15);
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_15);

    uart_init();

	while(1){
		//receive_uart();
        for(uint32_t x = 0; x < 15000000; x++);
        transmit_uart (transmit_buf, sizeof(transmit_buf));
	}
}

void transmit_uart (uint8_t *buf, uint8_t size){
    gpio_bit_set(GPIOA, GPIO_PIN_4);
    for (uint8_t x = 0; x < size; x++){
        while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
        usart_data_transmit (USART1, *(buf + x));          
    }
    while(RESET == usart_flag_get(USART1, USART_FLAG_TC));
    gpio_bit_reset(GPIOA, GPIO_PIN_4);
}

void receive_uart(void){
	static uint32_t x = 0;
    while(RESET == usart_flag_get(USART1, USART_FLAG_RBNE)){
    }
	rec_buf[x] = usart_data_receive(USART1);
	x++;
    if (x == 32) x = 0;
}

void uart_init(void){
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);

	/*RE_DE*/
	gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_4);		 
	gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_4);
	gpio_bit_reset(GPIOA, GPIO_PIN_4);

    /* connect port to USARTx_Tx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_2);

    /* connect port to USARTx_Rx */
    gpio_af_set(GPIOA, GPIO_AF_1, GPIO_PIN_3);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_2);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_2);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_3);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, GPIO_PIN_3);



    /* USART configure */
    usart_deinit(USART1);
    usart_word_length_set(USART1, USART_WL_8BIT);
    usart_stop_bit_set(USART1, USART_STB_1BIT);
    usart_parity_config(USART1, USART_PM_NONE);
    usart_baudrate_set(USART1, 1000000U);
    usart_receive_config(USART1, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART1, USART_TRANSMIT_ENABLE);

    usart_enable(USART1);

}