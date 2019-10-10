/*
**  Copyright (c) 2019, Kasey Koscianski.
*****************************************************************************
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU Affero General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU Affero General Public License for more details.
**
**  You should have received a copy of the GNU Affero General Public License
**  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************
*/

#include "color.hpp"

Color::Color() 
{
    brightness_val = 0xFF;
    red_val        = 0x00;
    green_val      = 0x00;
    blue_val       = 0x00;
}

Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness /*=255*/)
{
    brightness_val = brightness;
    red_val        = red;
    green_val      = green;
    blue_val       = blue;
}

Color::Color(uint32_t color_hex)
{
    brightness_val = (uint8_t)((color_hex >> 24) & 255);
    red_val        = (uint8_t)((color_hex >> 16) & 255);
    green_val      = (uint8_t)((color_hex >> 8) & 255);
    blue_val       = (uint8_t)(color_hex & 255);
}

uint8_t Color::get_brightness() const
{
    return brightness_val;
}

uint8_t Color::get_red() const
{
    return red_val;
}

uint8_t Color::get_green() const
{
    return green_val;
}

uint8_t Color::get_blue() const
{
    return blue_val;
}

uint32_t Color::get_hex() const
{
    return (brightness_val << 24) | (red_val << 16) | (green_val << 8) | blue_val;
}

bool Color::operator==(const Color&)
{
    return false;
}