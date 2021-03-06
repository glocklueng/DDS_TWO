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
#include <math.h>
#include <stdio.h>
#include "page_misc.h"
#include "ks0108/ks0108.h"
#include "page_graph.h"
#include "ad9910.h"
#include "adc.h"
#include "settings.h"

void page_graph_settings_init()
{
    settings.graph_settings.lower_frequency = 1e6;
    settings.graph_settings.upper_frequency = 10e6;
    settings.graph_settings.power = 8;
    settings.graph_settings.frequency_step = 0; // auto calculated anyway
}

void drawCoordinateSystem(void)
{
    //for (char counter = 0; counter < 26; counter++) {
    //    ks0108DrawLine(counter * 5 + 1, 60, counter * 5 + 1, 63, BLACK);
    //}
    for (char counter = 0; counter < 11; counter++) {
        ks0108DrawLine(counter * 12+4, 60, counter * 12+4, 63, BLACK);
    }
    ks0108DrawHoriLine(4, 60, 120, BLACK);
}

void clearGraph(void)
{
    ks0108FillRect(0, 20, 127, 43, WHITE);
}

void drawXLegend()
{
    char *buf;
    ks0108GotoXY(1, 57);
    buf = intToString(settings.graph_settings.lower_frequency/1000000ul, 0);
    ks0108Puts(buf);
    ks0108GotoXY(120, 57);
    buf = intToString(settings.graph_settings.upper_frequency/1000000ul, 0);
    ks0108Puts(buf);
}

void draw_graph(struct menuitem *self,uint8_t event)
{
    uint16_t power_val = 0;
    uint16_t power_val_last;
    uint32_t frequency,f_step;

    drawCoordinateSystem();

    f_step = (settings.graph_settings.upper_frequency -
                settings.graph_settings.lower_frequency) / 120ul;

    for (uint8_t i = 0; i < 120; i++) {
        frequency = settings.graph_settings.lower_frequency + f_step*i;
        if(settings.output_active){
            dds_set_single_tone_frequency(10, frequency);
        }
        if (self->num == 2) {
            power_val = getPDValue(PD_REFLECT);
        } else if (self->num == 3) {
            power_val = getPDValue(PD_TRANSMISSION);
        }
        if (i == 0) {
            ks0108FillRect(i + 4, 15, 1, 44, WHITE);
            ks0108SetDot(i + 4, 60 - power_val / 2, BLACK);
        } else {
            ks0108FillRect(i + 5, 15, 1, 44, WHITE);
            ks0108DrawLine(i+4, 60 - power_val_last / 2, i + 5,
                           60 - power_val / 2, BLACK);
        }
        power_val_last = power_val;
    }
    //drawXLegend();
}

void page_graph(struct menuitem *self)
{
    clear_page_main();
    draw_graph(self,0);
}
