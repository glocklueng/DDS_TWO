/*
 * Copyright (C) 2014 Bjoern Biesenbach <bjoern at bjoern-b.de>
 *               2014 homerj00			<homerj00 at web.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <string.h>
#include "menu.h"
#include "ks0108/ks0108.h"
#include "page_misc.h"

char *intToString(int8_t value, uint8_t one_leading_zero)
{
    static char buf[10];
    uint8_t pos = 0;

    if (value < 0) {
        buf[pos++] = '-';
        value = -value;
    }
    if (value < 10) {
        if (one_leading_zero) {
            buf[pos++] = '0';
        }
        buf[pos++] = value + 48;
        buf[pos] = '\0';
    } else if (value < 100) {
        buf[pos++] = value / 10 + 48;
        buf[pos++] = value % 10 + 48;
        buf[pos++] = '\0';
    } else if (value < 1000) {
        buf[pos++] = value / 100 + 48;
        value %= 100;
        buf[pos++] = value / 10 + 48;
        buf[pos++] = value % 10 + 48;
        buf[pos++] = '\0';
    }
    return buf;
}

void clear_page_main()
{
    ks0108FillRect(0, 16, 127, 47, WHITE);
}

void clear_page_header()
{
    ks0108FillRect(0, 0, 127, 16, WHITE);
}

void draw_page_header(struct menuitem *self)
{
    clear_page_header();
    //ks0108DrawCircle(7,7,7,BLACK);
    ks0108DrawHoriLine(0, 13, 127, BLACK);
    ks0108GotoXY(2, 3);
    ks0108SelectFont(2, BLACK);
    ks0108Puts_P(self->name);
    ks0108SelectFont(1, BLACK);
    //if(self->num>8){
    //    ks0108GotoXY(4,2);
    //}else{
    //    ks0108GotoXY(6,2);
    //}
    //page_num = intToString(self->num+1,0);
    //ks0108Puts(page_num);

}

void draw_arrow(uint8_t x, uint8_t y)
{
    ks0108DrawHoriLine(x, y, 5, BLACK);
    ks0108SetDot(x + 3, y - 2, BLACK);
    ks0108SetDot(x + 4, y - 1, BLACK);
    ks0108SetDot(x + 3, y + 2, BLACK);
    ks0108SetDot(x + 4, y + 1, BLACK);
}

void draw_settings_line(PGM_P name, char *value, uint8_t position)
{
    ks0108GotoXY(2, 17 + position * 11);
    ks0108Puts_P(name);
    ks0108FillRect(100, 17 + position * 11, 27, 10, WHITE);
    ks0108GotoXY(100, 17 + position * 11);
    //ks0108FillRect(100, 17 + position * 11, 27, 10, WHITE);
    //ks0108GotoXY(100, 17 + position * 11);
    ks0108Puts(value);
}

void draw_settings_line_variable_space(PGM_P name, char *value,
                                       uint8_t position, uint8_t space)
{
    ks0108GotoXY(2, 17 + position * 11);
    ks0108Puts_P(name);
    ks0108FillRect(space, 17 + position * 11, 27, 10, WHITE);
    ks0108GotoXY(space, 17 + position * 11);
    ks0108Puts(value);
}

void draw_centered_string(char *value, uint8_t position)
{
    uint16_t string_width_on_lcd;
    uint8_t string_width;
    uint16_t start_position;

    ks0108FillRect(0, 17 + position * 11, 127, 10, WHITE);

    string_width_on_lcd = ks0108StringWidth(value);
    while (string_width_on_lcd > 128) {
        string_width = strlen(value);
        value[string_width - 1] = '\0'; //shorten string by 1 char
        string_width_on_lcd = ks0108StringWidth(value);
    }
    start_position = (128 - string_width_on_lcd) / 2;
    ks0108GotoXY(start_position, 17 + position * 11);
    ks0108Puts(value);
}
