
#include <stdint.h>

#define CLK LL_GPIO_PIN_0
#define DIO LL_GPIO_PIN_1

const char segmentMap[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, // 0-7
    0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, // 8-9, A-F
    0x00
};

void start_cmd(void){
	LL_GPIO_SetOutputPin(GPIOA,CLK);
	LL_GPIO_SetOutputPin(GPIOA,DIO);
	LL_mDelay(1);
	LL_GPIO_ResetOutputPin(GPIOA,DIO);
	LL_mDelay(1);
}

void stop_cmd(void){
	LL_GPIO_ResetOutputPin(GPIOA,DIO);
	LL_GPIO_ResetOutputPin(GPIOA,CLK);
	LL_mDelay(1);
	LL_GPIO_SetOutputPin(GPIOA,CLK);
	LL_mDelay(1);
	LL_GPIO_SetOutputPin(GPIOA,DIO);
}

void cmd(uint8_t data){
	uint8_t i;
	uint8_t data_cmd = data;
	for(i =0 ; i<8 ; i++){
		LL_GPIO_ResetOutputPin(GPIOA,CLK);
		
		if(data_cmd & 0x01)
			LL_GPIO_SetOutputPin(GPIOA,DIO);
		else
			LL_GPIO_ResetOutputPin(GPIOA,DIO);
		
		LL_mDelay(1);
		LL_GPIO_SetOutputPin(GPIOA,CLK);
		LL_mDelay(1);
		data_cmd = data_cmd >>1;	
	}
	//send ACK
	LL_GPIO_ResetOutputPin(GPIOA,CLK);
	LL_GPIO_ResetOutputPin(GPIOA,DIO);
	LL_mDelay(1);
	LL_GPIO_SetOutputPin(GPIOA,CLK);
	LL_mDelay(1);
}

void write_data_display(){
	start_cmd();
	LL_mDelay(1);
	cmd(0x40);
	stop_cmd();
}

void get_address_segment(uint8_t num){
	start_cmd();
	cmd(0xC0 | num);
	LL_GPIO_ResetOutputPin(GPIOA,CLK);
	LL_mDelay(1);
}

void Display_ON(uint8_t brightness){
	cmd( 0x80 | ( (0x07 & brightness)|0x08) ); 
}

void Show_Digit(uint8_t num,uint8_t segment, uint8_t brightness){
	write_data_display();
	
	get_address_segment(segment);
	
	cmd(num);
	stop_cmd();
	
	start_cmd();
	Display_ON(brightness);
	stop_cmd();
}

uint8_t return_segment_map(uint8_t number){
	return segmentMap[(number - '0')];
}

void Show_Number(char *number ,uint8_t start, uint8_t brightness){
	write_data_display();
	
	get_address_segment(start);
	
	uint8_t j = 0;
	while(number[j] !=0){
		cmd( return_segment_map( number[j] ) );
		j++;
	}
	stop_cmd();
	
	start_cmd();
	Display_ON(brightness);
	stop_cmd();
}
