/*
 * DDS_TWO.c
 *
 * Created: 14.11.2012 12:55:29
 *  Author: Norrin
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "DDS_TWO.h"
#include "buttons.h"
#include "drehgeber.h"
#include "ks0108/ks0108.h"
#include "page_misc.h"
#include "page_main.h"
#include "ad9910.h"

#define OUTPUT_LED PA4

#define NUM_PAGES 2
static struct menuitem menu[] = {
    {NULL,0, update_main, main_drehgeber, main_button_pressed, page_main, NULL,0,0},
    {NULL,1, update_main, NULL, main_button_pressed, page_main, NULL,0,0}
};

void io_init(void)
{

	DDRA = (1<<OUTPUT_LED);
	// PULLUPS
	PORTA = (1<<KEY0) | (1<<KEY1) | (1<<KEY2) | (1<<KEY3) | (1<<KEY4);
	PORTA |= (1<<DREHGEBER_A) | (1<<DREHGEBER_B);

	//LED BL
	DDRF = (1<<PF3);
	PORTF = (1<<PF3);

	//LCD RESET
	DDRC |= (1<<PC2);
	
}

int main(void)
{
    int8_t drehgeber_delta = 0;
    int8_t menu_position = 0;
    int8_t old_menu_position = 0;
    uint8_t focus_here = 1;

	DDRC |= (1<<PC2);
    menu[0].name = PSTR("DDS TWO");
    menu[1].name = PSTR("DDS TWO 2");

	io_init();
    ks0108Init();
    drehgeber_init();
    draw_page_header(&menu[menu_position]);
	menu[menu_position].draw_func(&menu[menu_position]);
	ad9910_init();

	sei();
	
    while(1)
    {
    //draw_page_header(&menu[0]);
        //TODO:: Please write your application code 
        if((drehgeber_delta = drehgeber_read())){ // Drehgeber in action

            if(focus_here){
                menu_position += drehgeber_delta;
                if(menu_position > (NUM_PAGES-1)) menu_position = NUM_PAGES-1;
                if(menu_position < 0) menu_position = 0;

                if(menu_position != old_menu_position)
                {
                    if(menu[menu_position].draw_func){
                        menu[menu_position].draw_func(&menu[menu_position]);
                    }
                    draw_page_header(&menu[menu_position]);
                    old_menu_position = menu_position;
                }
            }else{
                if(menu[menu_position].drehgeber_func){
                    menu[menu_position].drehgeber_func(&menu[menu_position], drehgeber_delta);
                }
            }
        }
        if(get_key_press(1<<KEY0)){ //button left
            if(menu[menu_position].taster_func){
                menu[menu_position].taster_func(&menu[menu_position],0);
            }
        }
        if(get_key_press(1<<KEY1)){ //button left +1
            if(menu[menu_position].taster_func){
                menu[menu_position].taster_func(&menu[menu_position],1);
            }
        }
        if(get_key_press(1<<KEY2)){ //button right -1
            if(menu[menu_position].taster_func){
                menu[menu_position].taster_func(&menu[menu_position],2);
            }
        }
        if(get_key_press(1<<KEY3)){ //button right
            if(menu[menu_position].taster_func){
                menu[menu_position].taster_func(&menu[menu_position],3);
            }
			PORTA ^= (1<<OUTPUT_LED);
        }
        if(get_key_press(1<<KEY4)){ //button encoder
            if(menu[menu_position].taster_func){
                focus_here = menu[menu_position].taster_func(&menu[menu_position],4);
            }
        }
    }
}

ISR(TIMER0_COMP_vect) // 1ms
{
    cli();
    static uint16_t prescaler = 1000;
    
    drehgeber_work();

    if(--prescaler == 0){
        //refreshFlags |= (1<<SEC);
        prescaler = 1000;
    }else if(!(prescaler % 10)){ // 10ms
        buttons_every_10_ms();
    }
    sei();
}
