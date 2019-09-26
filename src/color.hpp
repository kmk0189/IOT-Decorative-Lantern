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

#pragma once

#include <stdint.h>

class Color
{
public:
    Color();
    Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness = 255);
    explicit Color(uint32_t hex);

    uint8_t  get_brightness();
    uint8_t  get_red();
    uint8_t  get_green();
    uint8_t  get_blue();
    
    uint32_t get_hex();

    bool operator==(const Color&);

private:
    uint8_t  brightness_val;
    uint8_t  red_val;
    uint8_t  green_val;
    uint8_t  blue_val;
};
